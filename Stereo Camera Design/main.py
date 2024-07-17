# Main file for optimizing the stereo camera design
# Run this file to start the optimization process
# This code is based on CARLA python example for environment setup (https://carla.org//)
# This code is based on the training and inference code of PSMNet (Chang et al., 2015), sourced from
# https://github.com/JiaRenChang/PSMNet

import glob
import os
import sys
import time

try:
    sys.path.append(glob.glob('../carla/dist/carla-*%d.%d-%s.egg' % (
        sys.version_info.major,
        sys.version_info.minor,
        'win-amd64' if os.name == 'nt' else 'linux-x86_64'))[0])
except IndexError:
    pass

import carla
import argparse
import logging
from numpy import random
import numpy as np
import cv2
from decimal import Decimal, ROUND_HALF_UP
import torch
import torch.nn as nn
import torch.nn.functional as F
from PSMNet import PSMNet
from generate_traffic import generate_traffic, destroy_traffic
from camera import Camera
from ga_utils import GAUtils
from dataset import get_item

def main():
    argparser = argparse.ArgumentParser(
        description=__doc__)
    argparser.add_argument(
        '--host',
        metavar='H',
        default='127.0.0.1',
        help='IP of the host server (default: 127.0.0.1)')
    argparser.add_argument(
        '-p', '--port',
        metavar='P',
        default=2000,
        type=int,
        help='TCP port to listen to (default: 2000)')
    argparser.add_argument(
        '-n', '--number-of-vehicles',
        metavar='N',
        default=1,
        type=int,
        help='Number of vehicles (default: 30)')
    argparser.add_argument(
        '-w', '--number-of-walkers',
        metavar='W',
        default=0,
        type=int,
        help='Number of walkers (default: 10)')
    argparser.add_argument(
        '--safe',
        action='store_true',
        default='true',
        help='Avoid spawning vehicles prone to accidents')
    argparser.add_argument(
        '--filterv',
        metavar='PATTERN',
        default='vehicle.*',
        help='Filter vehicle model (default: "vehicle.*")')
    argparser.add_argument(
        '--generationv',
        metavar='G',
        default='All',
        help='restrict to certain vehicle generation (values: "1","2","All" - default: "All")')
    argparser.add_argument(
        '--filterw',
        metavar='PATTERN',
        default='walker.pedestrian.*',
        help='Filter pedestrian type (default: "walker.pedestrian.*")')
    argparser.add_argument(
        '--generationw',
        metavar='G',
        default='2',
        help='restrict to certain pedestrian generation (values: "1","2","All" - default: "2")')
    argparser.add_argument(
        '--tm-port',
        metavar='P',
        default=8000,
        type=int,
        help='Port to communicate with TM (default: 8000)')
    argparser.add_argument(
        '--asynch',
        action='store_true',
        help='Activate asynchronous mode execution')
    argparser.add_argument(
        '--hybrid',
        action='store_true',
        help='Activate hybrid mode for Traffic Manager')
    argparser.add_argument(
        '-s', '--seed',
        metavar='S',
        type=int,
        help='Set random device seed and deterministic mode for Traffic Manager')
    argparser.add_argument(
        '--seedw',
        metavar='S',
        default=0,
        type=int,
        help='Set the seed for pedestrians module')
    argparser.add_argument(
        '--car-lights-on',
        action='store_true',
        default=False,
        help='Enable automatic car light management')
    argparser.add_argument(
        '--hero',
        action='store_true',
        default=False,
        help='Set one of the vehicles as hero')
    argparser.add_argument(
        '--respawn',
        action='store_true',
        default=False,
        help='Automatically respawn dormant vehicles (only in large maps)')
    argparser.add_argument(
        '--no-rendering',
        action='store_true',
        default=False,
        help='Activate no rendering mode')

    args = argparser.parse_args()

    logging.basicConfig(format='%(levelname)s: %(message)s', level=logging.INFO)

    # Initialization
    vehicles_list = []
    walkers_list = []
    all_id = []
    client = carla.Client(args.host, args.port)
    client.set_timeout(10.0)
    synchronous_master = False
    random.seed(args.seed if args.seed is not None else int(time.time()))
    buffer = {"left": [], "right": [], "gt": []}
    maxdisp = 192
    model = PSMNet(192)
    model = nn.DataParallel(model)
    ckpt = torch.load('./pretrained_model_KITTI2015.tar')
    model.load_state_dict(ckpt['state_dict'])
    best_fitness = 0
    best_loss = 999
    sol_per_pop = 5
    num_generation = 50
    if_model_train = True
    if_ga = True
    ga = GAUtils()

    device = torch.device("cuda:0" if torch.cuda.is_available() else "cpu")
    print(device)

    # Optimizer for PSMNet
    optimizer = torch.optim.Adam(model.parameters(), lr=0.001, betas=(0.9, 0.999))

    # Generate traffic
    try:
        world = client.get_world()

        traffic_manager = client.get_trafficmanager(args.tm_port)
        traffic_manager.set_global_distance_to_leading_vehicle(3)
        if args.respawn:
            traffic_manager.set_respawn_dormant_vehicles(True)
        if args.hybrid:
            traffic_manager.set_hybrid_physics_mode(True)
            traffic_manager.set_hybrid_physics_radius(70.0)
        if args.seed is not None:
            traffic_manager.set_random_device_seed(args.seed)

        settings = world.get_settings()
        if not args.asynch:
            traffic_manager.set_synchronous_mode(True)
            if not settings.synchronous_mode:
                synchronous_master = True
                settings.synchronous_mode = True
                settings.fixed_delta_seconds = 0.05
            else:
                synchronous_master = False
        else:
            print("You are currently in asynchronous mode. If this is a traffic simulation, \
            you could experience some issues. If it's not working correctly, switch to synchronous \
            mode by using traffic_manager.set_synchronous_mode(True)")

        if args.no_rendering:
            settings.no_rendering_mode = True
        world.apply_settings(settings)

        vehicles_list, walkers_list, all_id, all_actors = generate_traffic(world, client, args, traffic_manager,
                                                                           synchronous_master)

        # Spawn Camera
        all_vehicle_actors = world.get_actors(vehicles_list)
        vehicle = all_vehicle_actors[0]
        cam = Camera(world)
        cameras = cam.spawn_cam(vehicle)

        # Create a queue to store and retrieve the sensor data
        left_queue, right_queue, depth_queue = cam.listen_to_queue(cameras)

        # wait for a tick to ensure client receives the last transform of the walkers we have just created
        if args.asynch or not synchronous_master:
            world.wait_for_tick()
        else:
            world.tick()

        print('spawned %d vehicles and %d walkers, press Ctrl+C to exit.' % (len(vehicles_list), len(walkers_list)))

        # Set traffic manager parameters
        traffic_manager.global_percentage_speed_difference(30.0)

        # Get the first generation
        new_population = ga.get_population(sol_per_pop, if_ga)

        model = model.to(device)

        # Every iteration
        for generation in range(num_generation):
            print("Generation : ", generation)

            fitness = np.empty(sol_per_pop)
            disp_loss = np.empty(sol_per_pop)

            for i in range(sol_per_pop):
                for camera in cameras:
                    camera.destroy()

                # Update camera parameters
                cam.update_config(new_population[i][0], new_population[i][1])
                cameras = cam.spawn_cam(vehicle)

                left_queue, right_queue, depth_queue = cam.listen_to_queue(cameras)

                if not args.asynch and synchronous_master:
                    world.tick()

                    # Get scene captures
                    left_img = left_queue.get()
                    left_img = np.reshape(np.copy(left_img.raw_data), (left_img.height, left_img.width, 4))
                    left_img_disp = left_img[:, :, :3]
                    left_img = left_img_disp[:, :, ::-1]
                    left_img = cam.add_noise(left_img)

                    right_img = right_queue.get()
                    right_img = np.reshape(np.copy(right_img.raw_data), (right_img.height, right_img.width, 4))
                    right_img_disp = right_img[:, :, :3]
                    right_img = right_img_disp[:, :, ::-1]
                    right_img = cam.add_noise(right_img)

                    depth_img = depth_queue.get()
                    depth_img.convert(carla.ColorConverter.Depth)
                    depth_img = np.array(depth_img.raw_data).reshape((depth_img.height, depth_img.width, 4))[:, :, :3]

                    # Convert depth to disparity
                    disparity = cam.depth_to_disparity(depth_img)

                    # Train PSMNet
                    if if_model_train:
                        model.train()

                        buffer["left"].append(left_img)
                        buffer["right"].append(right_img)
                        buffer["gt"].append(disparity)

                        BUFFER_SIZE = 6
                        if len(buffer["left"]) > BUFFER_SIZE:
                            buffer["left"].pop(0)
                            buffer["right"].pop(0)
                            buffer["gt"].pop(0)

                        total_loss = 0.0
                        EPOCHS = 2
                        for epoch in range(EPOCHS):
                            outputs1 = []
                            outputs2 = []
                            outputs3 = []
                            gts = []
                            for j in range(len(buffer["left"])):
                                left = buffer["left"][j]
                                right = buffer["right"][j]
                                gt = buffer["gt"][j]

                                left, right, gt = get_item(left, right, gt)

                                left = left.unsqueeze(0).to(device)
                                right = right.unsqueeze(0).to(device)
                                gt = gt.to(device)

                                mask = gt < maxdisp
                                mask.detach_()

                                output1, output2, output3 = model(left, right)
                                output1 = torch.squeeze(output1, 1)
                                output2 = torch.squeeze(output2, 1)
                                output3 = torch.squeeze(output3, 1)

                                outputs1.append(output1[mask])
                                outputs2.append(output2[mask])
                                outputs3.append(output3[mask])
                                gts.append(gt[mask])

                            gt_batch = torch.cat(gts, 0)
                            output1_batch = torch.cat(outputs1, 0)
                            output2_batch = torch.cat(outputs2, 0)
                            output3_batch = torch.cat(outputs3, 0)
                            optimizer.zero_grad()
                            loss = 0.5 * F.smooth_l1_loss(output1_batch, gt_batch,
                                                          size_average=True) + 0.7 * F.smooth_l1_loss(
                                output2_batch, gt_batch, size_average=True) + F.smooth_l1_loss(output3_batch, gt_batch,
                                                                                               size_average=True)
                            loss.backward()
                            optimizer.step()
                            total_loss += loss.item()
                        disp_loss[i] = total_loss / EPOCHS

                        if disp_loss[i] < best_loss:
                            # SAVE
                            savefilename = './checkpoint.tar'
                            torch.save({
                                'epoch': generation*5+i,
                                'state_dict': model.state_dict(),
                                'train_loss': disp_loss[i],
                            }, savefilename)

                    # ---- Calculate Fitness ---
                    model.eval()

                    imgL, imgR, disp_true = get_item(left_img, right_img, disparity)
                    imgL, imgR, disp_true = (imgL.unsqueeze(0).to(device), imgR.unsqueeze(0).to(device),
                                             disp_true.squeeze(0).to(device))

                    mask = disp_true < 192

                    if imgL.shape[2] % 16 != 0:
                        times = imgL.shape[2] // 16
                        top_pad = (times + 1) * 16 - imgL.shape[2]
                    else:
                        top_pad = 0

                    if imgL.shape[3] % 16 != 0:
                        times = imgL.shape[3] // 16
                        right_pad = (times + 1) * 16 - imgL.shape[3]
                    else:
                        right_pad = 0

                    imgL = F.pad(imgL, (0, right_pad, top_pad, 0))
                    imgR = F.pad(imgR, (0, right_pad, top_pad, 0))

                    with torch.no_grad():
                        output3 = model(imgL, imgR)
                        output3 = torch.squeeze(output3)

                    if top_pad != 0:
                        img = output3[:, top_pad:, :]
                    else:
                        img = output3

                    if len(disp_true[mask]) == 0:
                        loss = 0
                    else:
                        loss = F.l1_loss(img[mask],
                                         disp_true[
                                             mask])

                    if not if_model_train:
                        disp_loss[i] = loss.data.cpu()

                    fitness[i] = 1 / loss.data.cpu()

                    if fitness[i] > best_fitness:
                        best_fitness = fitness[i]

                    """ logging """
                    print("Generation: {}, Solution: {}, Fitness: {}, Best Fitness: {}, Loss: {}, FOV: {}, "
                          "Baseline: {}".format(generation, i, fitness[i], best_fitness, disp_loss[i],
                                           new_population[i][0], new_population[i][1]))

                else:
                    world.wait_for_tick()

            # Optimize camera parameters
            if if_ga:
                # Selecting the best parents in the population for mating.
                parents = ga.select_mating_pool(new_population, fitness, int(sol_per_pop/2))

                parent_neglect_num = int(Decimal(parents.shape[0] / 3).quantize(0, ROUND_HALF_UP))

                # Generating next generation using crossover.
                offspring_crossover = ga.crossover(parents, offspring_size=(sol_per_pop-parent_neglect_num-1, 2))

                # Adding some variations to the offspring using mutation.
                offspring_mutation = ga.mutation(offspring_crossover)

                # Creating the new population based on the parents and offspring.
                new_population[0:parents.shape[0], :] = parents[0:parents.shape[0], :]
                new_population[parents.shape[0]:, :] = offspring_mutation
            else:
                new_population = ga.get_population(sol_per_pop, if_ga)

    finally:
        if not args.asynch and synchronous_master:
            settings = world.get_settings()
            settings.synchronous_mode = False
            settings.no_rendering_mode = False
            settings.fixed_delta_seconds = None
            world.apply_settings(settings)

        destroy_traffic(vehicles_list, walkers_list, client, all_id, all_actors)

        time.sleep(0.5)

if __name__ == '__main__':
    try:
        main()
    except KeyboardInterrupt:
        pass
    finally:
        print('\ndone.')
