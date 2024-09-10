"""
Object detection code
"""

import pandas as pd # data processing, CSV file I/O (e.g. pd.read_csv)
import os
from PIL import Image
import metrics
import torch
import torchvision
from torchvision.models.detection.faster_rcnn import FastRCNNPredictor
from torchvision.models.detection import FasterRCNN_ResNet50_FPN_Weights
from torch.utils.data import Dataset, DataLoader
from torchvision.transforms import transforms

class ObjectDetectionData(Dataset):
    def __init__(self, root_dir, sol_num, resX, resY, transform=None):
        self.image_dir = os.path.join(root_dir, str(sol_num) + '_train')
        self.annot_dir = os.path.join(root_dir, str(sol_num) + '_annotations')
        self.resX = resX
        self.resY = resY
        self.transform = transform
        self.image_names = os.listdir(self.image_dir)
        self.image_names.sort()

    def __len__(self):
        return len(self.image_names)

    def __getitem__(self, idx):
        image_path = os.path.join(self.image_dir, f'train{idx}.tif')
        image = Image.open(image_path).convert("RGB")
        image = self.transform(image)
        target = {}
        annot_path = os.path.join(self.annot_dir, f'labels{idx}.csv')
        img_df = pd.read_csv(annot_path, header=None)

        boxes = []
        labels = []

        obj = 0
        while (obj < len(img_df.columns) - 1):
            # get object class
            labels.append(img_df.values[0][obj + 1])

            # get object bounding box
            xmin = max(0, img_df.values[0][obj + 2])
            xmin = min(self.resX, xmin)
            ymin = max(0, img_df.values[0][obj + 3])
            ymin = min(self.resY, ymin)
            xmax = max(0, img_df.values[0][obj + 4])
            xmax = min(self.resX, xmax)
            ymax = max(0, img_df.values[0][obj + 5])
            ymax = min(self.resY, ymax)
            boxes.append([int(xmin), int(ymin), int(xmax), int(ymax)])

            obj += 6

        boxes = torch.tensor(boxes, dtype=torch.float32)
        labels = torch.tensor(labels)
        target['boxes'] = boxes
        target['labels'] = labels
        return image, target


class ObjectDetectionTrainer:
    def __init__(self, num_of_class):
        self.num_classes = num_of_class

        # Define the model
        self.model = torchvision.models.detection.fasterrcnn_resnet50_fpn(weights=FasterRCNN_ResNet50_FPN_Weights.DEFAULT)
        self.in_features = self.model.roi_heads.box_predictor.cls_score.in_features
        self.model.roi_heads.box_predictor = FastRCNNPredictor(self.in_features, self.num_classes)

        # Define the optimizer and learning rate scheduler
        self.optimizer = torch.optim.Adam([p for p in self.model.parameters() if p.requires_grad], lr=1e-4,
                                          amsgrad=True, weight_decay=1e-6)

        # and a learning rate scheduler
        self.lr_scheduler = torch.optim.lr_scheduler.StepLR(self.optimizer, step_size=5, gamma=0.5)

        # Define the device (GPU or CPU)
        self.device = torch.device('cuda:1') if torch.cuda.is_available() else torch.device('cpu')

        self.model.to(self.device)

        self.best_mAP = 0

    def train_model(self, resX, resY, sol_num, epoch, ifSaved, print_freq):
        data_folder = r'E:\train_data'
        collate_fn = lambda batch: tuple(zip(*batch))

        # Define transforms for the dataset
        train_transforms = transforms.Compose([
            transforms.ToTensor(),
            transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
        ])

        dataset = ObjectDetectionData(data_folder, sol_num, int(resX), int(resY), transform=train_transforms)

        # Randomly split data into train part and test part
        train_data_ratio = 0.9

        train_size = int(len(dataset) * train_data_ratio)
        val_size = len(dataset) - train_size  # test data shouldn't be appeared in training

        train_data, val_data = torch.utils.data.random_split(dataset, [train_size, val_size])

        batch_num = 8

        train_loader = DataLoader(train_data, batch_size=batch_num, shuffle=True, collate_fn=collate_fn)
        val_loader = DataLoader(val_data, batch_size=batch_num, shuffle=True, collate_fn=collate_fn)

        if ifSaved:
            self.model = torch.load('./saved_model.pth')

        self.model.to(self.device)
        self.model.train()

        for iteration in range(1):
            for batch_idx, (images, targets) in enumerate(train_loader):
                # Sending training data to CUDA
                images = list([image.to(self.device) for image in images])
                targets = [{k: v.to(self.device) for k, v in t.items()} for t in targets]
                loss_dict = self.model(images, targets)
                losses = sum(loss for loss in loss_dict.values())
                self.optimizer.zero_grad()
                losses.backward()
                self.optimizer.step()
                if batch_idx % print_freq == 0:
                    print(f'Epoch: {epoch}, Batch: {batch_idx}, Loss: {losses}')

            self.lr_scheduler.step()

        img_idx = 0
        pred_box = []
        true_box = []

        self.model.eval()

        with torch.no_grad():
            for batch_idx, (images, targets) in enumerate(val_loader):
                images = list([image.to(self.device) for image in images])
                targets = [{k: v.to(self.device) for k, v in t.items()} for t in targets]

                outputs = self.model(images)

                for i in range(len(outputs)):
                    for j in range(len(outputs[i]['labels'])):
                        current_pred_box = [0, 0, 0, 0, 0, 0, 0]
                        current_pred_box[0] = img_idx + i
                        current_pred_box[1] = outputs[i]['labels'][j].cpu()
                        current_pred_box[2] = outputs[i]['scores'][j].cpu()
                        current_pred_box[3:] = outputs[i]['boxes'][j].cpu()

                        pred_box.append(current_pred_box)

                for i in range(len(targets)):
                    for j in range(len(targets[i]['labels'])):
                        current_target_box = [0, 0, 0, 0, 0, 0]
                        current_target_box[0] = img_idx + i
                        current_target_box[1] = targets[i]['labels'][j].cpu()
                        current_target_box[2:] = targets[i]['boxes'][j].cpu()

                        true_box.append(current_target_box)

                img_idx += batch_num

            # For AP@0.5
            mAP = metrics.mean_average_precision(pred_box, true_box, iou_threshold=0.5, num_classes=self.num_classes)

            if mAP > self.best_mAP:
                self.best_mAP = mAP
                torch.save(self.model, './saved_model.pth')

        return mAP







