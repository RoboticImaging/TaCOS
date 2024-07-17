# RGB camera class

import carla
import numpy as np
import queue

class Camera:
    def __init__(self, world):
        self.world = world
        self.img_x = 512
        self.img_y = 256
        self.fov = 75
        self.baseline = 0.1
        self.camera_bp = self.world.get_blueprint_library().find('sensor.camera.rgb')
        self.depth_camera_bp = self.world.get_blueprint_library().find('sensor.camera.depth')

    # Set RGB camera
    def __set_cam_geometry(self):
        self.camera_bp.set_attribute('image_size_x', str(self.img_x))
        self.camera_bp.set_attribute('image_size_y', str(self.img_y))
        self.camera_bp.set_attribute('fov', str(self.fov))
        self.camera_bp.set_attribute('motion_blur_intensity', '1.0')

    # Set depth camera for ground truth
    def __set_depth_cam_geometry(self):
        self.depth_camera_bp.set_attribute('image_size_x', str(self.img_x))
        self.depth_camera_bp.set_attribute('image_size_y', str(self.img_y))
        self.depth_camera_bp.set_attribute('fov', str(self.fov))

    # Spawn the stereo camera and the depth camera
    def spawn_cam(self, vehicle):
        # spawn camera
        cameras = []

        self.__set_cam_geometry()
        self.__set_depth_cam_geometry()

        left_camera_init_trans = carla.Transform(carla.Location(x=2, z=2))
        left_camera = self.world.spawn_actor(self.camera_bp, left_camera_init_trans, attach_to=vehicle)
        depth_camera = self.world.spawn_actor(self.depth_camera_bp, left_camera_init_trans, attach_to=vehicle)

        right_camera_init_trans = carla.Transform(carla.Location(x=2, y=self.baseline, z=2))
        right_camera = self.world.spawn_actor(self.camera_bp, right_camera_init_trans, attach_to=vehicle)

        cameras.append(left_camera)
        cameras.append(right_camera)
        cameras.append(depth_camera)
        return cameras

    # Update camera FOVs and baseline
    def update_config(self, fov, baseline):
        self.fov = fov
        self.baseline = baseline

    # Get image dimension
    def get_cam_params(self):
        # Get the attributes from the camera
        image_w = self.camera_bp.get_attribute("image_size_x").as_int()
        image_h = self.camera_bp.get_attribute("image_size_y").as_int()

        return image_w, image_h

    def add_noise(self, img):
        # FLIR (IMX172) Noise Characteristics
        sig_p = [0.24, 0.12, 0.29]
        sig_t = [1.82, 0.91, 3.2]

        im_noisy = np.zeros((self.img_y, self.img_x, 3))

        for c in range(3):
            img_channel = img[:, :, c]

            im_std = np.sqrt(sig_p[c] * img_channel + sig_t[c])
            generated_noisy = np.random.normal(img_channel, im_std)

            generated_noisy[generated_noisy[:] > 255] = 255
            generated_noisy[generated_noisy[:] < 0] = 0

            im_noisy[:, :, c] = generated_noisy

        im_noisy = im_noisy.astype(np.uint8)

        return im_noisy

    # Listen to camera queues
    def listen_to_queue(self, cameras):
        # Create a queue to store and retrieve the sensor data
        left_queue = queue.Queue()
        right_queue = queue.Queue()
        depth_queue = queue.Queue()

        cameras[0].listen(left_queue.put)
        cameras[1].listen(right_queue.put)
        cameras[2].listen(depth_queue.put)

        return left_queue, right_queue, depth_queue

    # Convert depth to disparity map
    def depth_to_disparity(self, depth):
        r = depth[:, :, 0]
        g = depth[:, :, 1]
        b = depth[:, :, 2]

        # Compute the depth in meters
        depth = (r + 256.0 * g + 256.0 ** 2 * b) / (256.0 ** 3 - 1.0) * 1000
        disparity = ((self.img_x / (2.0 * np.tan(self.fov * np.pi / 360.0))) * self.baseline) / depth

        return disparity

    # Convert disparity to depth map
    def diparity_to_depth(self, disparity):
        depth = ((self.img_x / (2.0 * np.tan(self.fov * np.pi / 360.0))) * self.baseline) / (disparity + 1e-6)
        depth = depth.cpu().numpy()
        depth = np.clip(depth / np.max(depth) * 255, 0, 255).astype(np.uint8)

        return depth