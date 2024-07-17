"""
Get inlier number between consecutive frames
"""

import cv2
import glob
from natsort import natsorted
import numpy as np

# Extract ORB features and find linears
def get_matched_features(solution_num):
    orb = cv2.ORB_create(nfeatures=2000)
    bf = cv2.BFMatcher(cv2.NORM_HAMMING, crossCheck=True)

    path = r'E:\train_data/' + str(solution_num) + '/*.tif'

    filenames = glob.glob(path)
    filenames = natsorted(filenames)

    try:
        train_images = [cv2.imread(img) for img in filenames]

        total_inliers = 0
        total_features = 0

        for i in range(0, len(train_images)-1):
            current_img = train_images[i]
            next_img = train_images[i+1]

            kp1, des1 = orb.detectAndCompute(current_img, None)
            kp2, des2 = orb.detectAndCompute(next_img, None)

            if kp1 is not None:
                total_features += len(kp1)

            if des1 is not None and des2 is not None:
                orb_matches = bf.match(des1, des2)
                kp1_coords = np.asarray([kp1[m.queryIdx].pt for m in orb_matches]).reshape(-1, 1, 2)
                kp2_coords = np.asarray([kp2[m.trainIdx].pt for m in orb_matches]).reshape(-1, 1, 2)
                _, mask = cv2.findFundamentalMat(kp1_coords, kp2_coords, method=cv2.FM_RANSAC, ransacReprojThreshold=3.0)
                orb_inlier_matches = np.sum(mask)

                if orb_inlier_matches is not None:
                    total_inliers += orb_inlier_matches

                    ret_matches = []
                    ret_points1, ret_points2 = [], []
                    for inlier, match in zip(mask, orb_matches):
                        if inlier[0] > 0:
                            ret_matches.append(match)
                            ret_points1.append(kp1[match.queryIdx].pt)
                            ret_points2.append(kp2[match.trainIdx].pt)

        average_inliers = total_inliers/(len(train_images)-1)
        average_features = total_features/(len(train_images) - 1)

    except:
        print("File Size Too Big: Discard")
        average_inliers = 0
        average_features = 0

    return average_inliers, average_features












