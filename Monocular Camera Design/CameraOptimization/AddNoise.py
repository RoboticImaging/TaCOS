"""
Add noise to images
"""

import cv2
import glob
import numpy as np
import os
import math

def add_noise(path, saved_path, current_pixel):
    # FLIR (IMX172) Noise Characteristics
    sig_p = [0.24, 0.12, 0.29]
    sig_t = [1.82, 0.91, 3.2]
    pixel_size = 2.4
    gain0 = 15

    # Change gain value based on design scenario
    # # Day
    gain1 = 5
    # Night
    # gain1 = gain0

    # Compute the gain ratio depending on pixel size
    pixel_ratio = (current_pixel ** 2)/pixel_size
    gain = (1/pixel_ratio) * (10 ** (gain1/20))
    gain = 20 * (math.log(gain, 10))
    gain_ratio = 10 ** ((gain-gain0)/20)

    filenames = glob.glob(path)

    # Add image noise
    for name in filenames:
        img = cv2.imread(name)

        im_noisy = np.zeros((img.shape[0], img.shape[1], img.shape[2]))

        for c in range(3):
            img_channel = img[:, :, c]

            im_std = np.sqrt(gain_ratio * sig_p[c] * img_channel + (gain_ratio ** 2) * sig_t[c])
            generated_noisy = np.random.normal(img_channel, im_std)

            generated_noisy[generated_noisy[:] > 255] = 255
            generated_noisy[generated_noisy[:] < 0] = 0

            im_noisy[:, :, c] = generated_noisy

        im_noisy = im_noisy.astype(np.uint8)
        cv2.imwrite(os.path.join(saved_path, os.path.basename(name).replace('.tif', '.tif')), im_noisy)



