# Genetic algorithm related functions

import numpy as np
import random

class GAUtils:
    def __init__(self):
        # Number of the weights we are looking to optimize.
        self.num_weights = 2

        self.params_max = [120, 5]
        self.params_min = [50, 0.01]

    # Get parameters
    def get_population(self, sol_per_pop, if_ga):
        # Creating the initial population.
        new_population = np.empty((sol_per_pop, self.num_weights))

        if if_ga:
            # FOV
            new_population[:, 0] = np.random.uniform(low=self.params_min[0], high=self.params_max[0], size=sol_per_pop)
            # Baseline
            new_population[:, 1] = np.random.uniform(low=self.params_min[1], high=self.params_max[1], size=sol_per_pop)
        else:
            # FOV
            new_population[:, 0] = 87
            # Baseline
            new_population[:, 1] = 0.095

        return new_population

    # Select parents
    def select_mating_pool(self, pop, fitness, num_parents):
        # Selecting the best individuals in the current generation as parents for producing the offspring of the next generation.
        parents = np.empty((num_parents, pop.shape[1]))
        for parent_num in range(num_parents):
            max_fitness_idx = np.where(fitness == np.max(fitness))
            max_fitness_idx = max_fitness_idx[0][0]
            parents[parent_num, :] = pop[max_fitness_idx, :]
            fitness[max_fitness_idx] = -99999999999
        return parents

    # Uniform crossover
    def crossover(self, parents, offspring_size):
        offspring = np.empty(offspring_size)
        idex_list = [i for i in range(0, parents.shape[0])]

        for i in range(offspring_size[1]):
            shuffled_list = random.sample(idex_list, len(idex_list))
            random_list = random.choices(idex_list, k=(offspring_size[0] - len(idex_list)))

            for j in range(offspring_size[0]):
                if j < len(idex_list):
                    offspring[j, i] = parents[shuffled_list[j], i]
                else:
                    offspring[j, i] = parents[random_list[j - len(idex_list)], i]

        return offspring

    # Mutation
    def mutation(self, offspring_crossover):
        # Mutation changes a single gene in each offspring randomly.
        for idx in range(offspring_crossover.shape[0]):

            # for fov
            random_mul = np.random.uniform(0.8, 1.2, 1)
            random_value = np.random.uniform(-5, 5, 1)
            offspring_crossover[idx, 0] = random_mul * offspring_crossover[idx, 0] + random_value

            if offspring_crossover[idx, 0] < self.params_min[0]:
                offspring_crossover[idx, 0] = self.params_min[0]
            elif offspring_crossover[idx, 0] > self.params_max[0]:
                offspring_crossover[idx, 0] = self.params_max[0]

            # for baseline
            random_mul = np.random.uniform(0.8, 1.2, 1)
            random_value = np.random.uniform(-0.2, 0.2, 1)
            offspring_crossover[idx, 1] = random_mul * offspring_crossover[idx, 1] + random_value

            if offspring_crossover[idx, 1] < self.params_min[1]:
                offspring_crossover[idx, 1] = self.params_min[1]
            elif offspring_crossover[idx, 1] > self.params_max[1]:
                offspring_crossover[idx, 1] = self.params_max[1]

        return offspring_crossover
