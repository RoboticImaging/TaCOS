# Data processing for the models 

import numpy as np
from torchvision import transforms

# Process captured images, convert to tensors
def get_item(left_img, right_img, disp):

    transform = transforms.Compose([
        transforms.ToPILImage(),
        transforms.ToTensor(),
        transforms.Normalize(mean=[0.485, 0.456, 0.406], std=[0.229, 0.224, 0.225])
    ])

    dataL = np.ascontiguousarray(disp, dtype=np.float32)

    left_img = transform(left_img)
    right_img = transform(right_img)

    transform = transforms.Compose([
        transforms.ToTensor()
    ])
    dataL = transform(dataL)

    return left_img, right_img, dataL
