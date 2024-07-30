"""
Main script for optimizing camera parameters with genetic algorithm
"""

# echo-server.py
import numpy as np
import pandas as pd
import socket
import ga
import win32gui, win32com.client
import TCPCommun as TCP
import InlierMatches as Inlier
import utilities as ut
from ObjectDetect import ObjectDetectionTrainer
from AddNoise import add_noise
import os
import glob
from decimal import Decimal, ROUND_HALF_UP

# load the image sensor dataset
image_sensor_catalog = pd.read_excel('Sensors Catalog.xlsx')


# Genetic algorithm parameters:
sol_per_pop = 10
num_parents_mating = int(sol_per_pop/2)

# Tasks weights
threshold_weight = 1
object_weight = 1
inlier_weight = 0.005 * 0.5
inlier_ratio_weight = 0.5

# True for using the fully discrete method
if_fully_discrete = True

# if optimize discrete variable with fully discrete method
if if_fully_discrete:

    # Number of the weights we are looking to optimize.
    num_weights = 3

    # set design space
    catalog_maximums = [30, 20, len(image_sensor_catalog.index)-1]
    catalog_minimums = [-30, 1, 0]

    #Creating the initial population.
    new_population = np.empty((sol_per_pop, num_weights))
    # Camera Orientation
    new_population[:, 0] = np.random.uniform(low=catalog_minimums[0], high=catalog_maximums[0], size=sol_per_pop)
    # Focal Length
    new_population[:, 1] = np.random.uniform(low=catalog_minimums[1], high=catalog_maximums[1], size=sol_per_pop)
    # Index from Catalog
    new_population[:, 2] = np.random.randint(low=catalog_minimums[2], high=catalog_maximums[2], size=sol_per_pop)

    current_pop = ut.get_params_with_idx(new_population, image_sensor_catalog, sol_per_pop)

    print(current_pop)

# if optimize with quantized continuous method
else:

    # Number of the weights we are looking to optimize.
    num_weights = 5

    # set design space
    catalog_maximums = [30, 20, image_sensor_catalog["Width"].max(), image_sensor_catalog["Height"].max(), image_sensor_catalog["Pixel Size"].max()]
    catalog_minimums = [-30, 1, image_sensor_catalog["Width"].min(), image_sensor_catalog["Height"].min(), image_sensor_catalog["Pixel Size"].min()]

    # Creating the initial population.
    new_population = np.empty((sol_per_pop, num_weights))
    # Camera Orientation
    new_population[:, 0] = np.random.uniform(low=catalog_minimums[0], high=catalog_maximums[0], size=sol_per_pop)
    # Focal Length
    new_population[:, 1] = np.random.uniform(low=catalog_minimums[1], high=catalog_maximums[1], size=sol_per_pop)
    # Image Width
    new_population[:, 2] = np.random.uniform(low=catalog_minimums[2], high=catalog_maximums[2], size=sol_per_pop)
    # Image Height
    new_population[:, 3] = np.random.uniform(low=catalog_minimums[3], high=catalog_maximums[3], size=sol_per_pop)
    # Pixel Size
    new_population[:, 4] = np.random.randint(low=catalog_minimums[4], high=catalog_maximums[4], size=sol_per_pop)

    new_population, closest_idx = ut.get_params_from_catalog(new_population, image_sensor_catalog["Width"], image_sensor_catalog["Height"], image_sensor_catalog["Pixel Size"], sol_per_pop)
    current_pop = ut.pixel_size_2_res(new_population, closest_idx, image_sensor_catalog["ResX"], image_sensor_catalog["ResY"], sol_per_pop)

    print(current_pop)


HOST = "127.0.0.1"  # Standard loopback interface address (localhost)
PORT = 3457  # Port to listen on

# Change this to desired path
Path = r'E:\train_data/'

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
    s.bind((HOST, PORT))
    s.listen()
    conn, addr = s.accept()
    with conn:
        print(f"Connected by {addr}")

        fitness_array = []
        accuracy_array = []
        inlier_num_array = []
        inlier_ratio_array = []
        mAP_array = []
        num_generations = 20
        DetectionTrainer = ObjectDetectionTrainer(11)
        ifSaved = False

        for generation in range(num_generations):
            print("Generation : ", generation)

            fitness = np.empty(sol_per_pop)
            accuracy = np.empty(sol_per_pop)
            mAP = np.empty(sol_per_pop)
            inlier_num = np.empty(sol_per_pop)
            inlier_ratio = np.empty(sol_per_pop)

            if if_fully_discrete:
                current_pop = ut.get_params_with_idx(new_population, image_sensor_catalog, sol_per_pop)
            else:
                new_population, closest_idx = ut.get_params_from_catalog(new_population, image_sensor_catalog["Width"],
                                                                         image_sensor_catalog["Height"],
                                                                         image_sensor_catalog["Pixel Size"],
                                                                         sol_per_pop)
                current_pop = ut.pixel_size_2_res(new_population, closest_idx, image_sensor_catalog["ResX"],
                                                  image_sensor_catalog["ResY"], sol_per_pop)

            for i in range(sol_per_pop):

                print("Current Params: ", current_pop[i])

                files = glob.glob(Path + str(i) + '\*.tif')
                for f in files:
                    os.remove(f)

                files = glob.glob(Path + str(i) + '_annotations\*.csv')
                for f in files:
                    os.remove(f)

                files = glob.glob(Path + str(i) + '_train\*.tif')
                for f in files:
                    os.remove(f)

                thresholds = TCP.send_parameters(current_pop[i], conn, i)
                accuracy[i] = ut.get_threshold_accuracy(thresholds)

                ImgPath = Path + str(i) + '_train'
                trainCount = len([name for name in os.listdir(ImgPath) if os.path.isfile(os.path.join(ImgPath, name))])

                if trainCount >= 100:
                    # add noise
                    ImgPath = Path + str(i) + '\*.tif'
                    ImgSavedPath = Path + str(i)
                    add_noise(ImgPath, ImgSavedPath, ((current_pop[i][2] * 1000)/current_pop[i][4]))

                    ImgPath = Path + str(i) + '_train\*.tif'
                    ImgSavedPath = Path + str(i) + '_train'
                    add_noise(ImgPath, ImgSavedPath, ((current_pop[i][2] * 1000) / current_pop[i][4]))

                    features = Inlier.get_matched_features(i)
                    inlier_num[i] = features[0]
                    inlier_ratio[i] = features[0] / (features[0] + features[1])

                    # evaluate with metrics
                    mAP[i] = DetectionTrainer.train_model(current_pop[i][4], current_pop[i][5], i, generation, ifSaved, print_freq=10)

                    ifSaved = True

                else:
                    print("Too few inputs, FOV not suitable")
                    mAP[i] = 0
                    inlier_ratio[i] = 0
                    inlier_num[i] = 0

                fitness[i] = threshold_weight*accuracy[i] + object_weight*mAP[i] + inlier_weight*inlier_num[i] + inlier_ratio_weight*inlier_ratio[i]
                print("Threshold Detection: ", accuracy[i])
                print("Inlier Number: ", inlier_num[i])
                print("Inlier Ratio: ", inlier_ratio[i])
                print("Object Detection mAP: ", mAP[i])
                print("Fitness: ", fitness[i])

            print("Current Population: ", current_pop)
            print("Population Fitnesses: ", fitness)
            print("Population Threshold Detection: ", accuracy)
            print("Population Inlier Number: ", inlier_num)
            print("Population Inlier Ratio: ", inlier_ratio)
            print("Population Object Detection mAP: ", mAP)

            # Then return the index of that solution corresponding to the best fitness.
            best_match_idx = np.where(fitness == np.max(fitness))
            best_match_idx = [a for b in best_match_idx for a in b]

            # Selecting the best parents in the population for mating.
            parents = ga.select_mating_pool(new_population, fitness, num_parents_mating)

            parent_neglect_num = int(Decimal(parents.shape[0] / 3).quantize(0, ROUND_HALF_UP))

            # Generating next generation using crossover.
            offspring_crossover = ga.crossover(parents, offspring_size=(sol_per_pop-parent_neglect_num-1, num_weights))

            # Adding some variations to the offspring using mutation.
            offspring_mutation = ga.mutation(offspring_crossover, catalog_minimums, catalog_maximums, if_fully_discrete)

            # Creating the new population based on the parents and offspring.
            new_population[0:parents.shape[0]-parent_neglect_num, :] = parents[0:parents.shape[0]-parent_neglect_num, :]
            new_population[parents.shape[0]-parent_neglect_num:, :] = offspring_mutation

        print("Best solution : ", current_pop[best_match_idx[0], :])
        print("Best solution fitness : ", fitness[best_match_idx[0]])
        print("Best solution threshold accuracy : ", accuracy[best_match_idx[0]])
        print("Best solution ORB inlier : ", inlier_num[best_match_idx[0]])
        print("Best solution ORB inlier ratio : ", inlier_ratio[best_match_idx[0]])
        print("Best solution OD mAP : ", mAP[best_match_idx[0]])
