"""
Functions for genetic algorithm
"""

import random
import numpy

# Calculating the fitness value of each solution in the current population.
# The fitness function caulcuates the sum of products between each input and its corresponding weight.
def cal_pop_fitness(equation_inputs, pop):
    fitness = numpy.sum(pop*equation_inputs, axis=1)
    return fitness

# Selecting the best individuals in the current generation as parents for producing the offspring of the next generation.
def select_mating_pool(pop, fitness, num_parents):
    parents = numpy.empty((num_parents, pop.shape[1]))
    for parent_num in range(num_parents):
        max_fitness_idx = numpy.where(fitness == numpy.max(fitness))
        max_fitness_idx = max_fitness_idx[0][0]
        parents[parent_num, :] = pop[max_fitness_idx, :]
        fitness[max_fitness_idx] = -99999999999
    return parents

# The uniform crossover method
def crossover(parents, offspring_size):
    offspring = numpy.empty(offspring_size)
    idex_list = [i for i in range(0, parents.shape[0])]

    for i in range(offspring_size[1]):
        shuffled_list = random.sample(idex_list, len(idex_list))
        random_list = random.choices(idex_list, k=(offspring_size[0] - len(idex_list)))

        for j in range(offspring_size[0]):
            if j < len(idex_list):
                offspring[j, i] = parents[shuffled_list[j], i]
            else:
                offspring[j, i] = parents[random_list[j-len(idex_list)], i]

    return offspring

# Mutation changes a single gene in each offspring randomly.
def mutation(offspring_crossover, catalog_minimums, catalog_maximums, if_fully_discrete):
    for idx in range(offspring_crossover.shape[0]):

        # for camera orientation
        random_mul = numpy.random.uniform(0.8, 1.2, 1)
        random_value = numpy.random.uniform(-3.0, 3.0, 1)
        offspring_crossover[idx, 0] = random_mul * offspring_crossover[idx, 0] + random_value

        if offspring_crossover[idx, 0] < catalog_minimums[0]:
            offspring_crossover[idx, 0] = catalog_minimums[0]
        elif offspring_crossover[idx, 0] > catalog_maximums[0]:
            offspring_crossover[idx, 0] = catalog_maximums[0]

        # for focal length
        random_mul = numpy.random.uniform(0.8, 1.2, 1)
        random_value = numpy.random.uniform(-3.0, 3.0, 1)
        offspring_crossover[idx, 1] = random_mul * offspring_crossover[idx, 1] + random_value

        if offspring_crossover[idx, 1] < catalog_minimums[1]:
            offspring_crossover[idx, 1] = catalog_minimums[1]
        elif offspring_crossover[idx, 1] > catalog_maximums[1]:
            offspring_crossover[idx, 1] = catalog_maximums[1]

        if if_fully_discrete:
            # for catalog index
            random_value = numpy.random.uniform(-3.0, 3.0, 1)
            offspring_crossover[idx, 2] = int(offspring_crossover[idx, 2] + random_value)

            if offspring_crossover[idx, 2] < catalog_minimums[2]:
                offspring_crossover[idx, 2] = catalog_minimums[2]
            elif offspring_crossover[idx, 2] > catalog_maximums[2]:
                offspring_crossover[idx, 2] = catalog_maximums[2]
        else:
            # for width
            random_mul = numpy.random.uniform(0.8, 1.2, 1)
            random_value = numpy.random.uniform(-3.0, 3.0, 1)
            offspring_crossover[idx, 2] = random_mul * offspring_crossover[idx, 2] + random_value

            # for height
            random_mul = numpy.random.uniform(0.8, 1.2, 1)
            random_value = numpy.random.uniform(-3.0, 3.0, 1)
            offspring_crossover[idx, 3] = random_mul * offspring_crossover[idx, 3] + random_value

            # for pixel
            random_mul = numpy.random.uniform(0.8, 1.2, 1)
            random_value = numpy.random.uniform(-3.0, 3.0, 1)
            offspring_crossover[idx, 4] = random_mul * offspring_crossover[idx, 4] + random_value

            if offspring_crossover[idx, 2] < catalog_minimums[2]:
                offspring_crossover[idx, 2] = catalog_minimums[2]
            elif offspring_crossover[idx, 2] > catalog_maximums[2]:
                offspring_crossover[idx, 2] = catalog_maximums[2]

            if offspring_crossover[idx, 3] < catalog_minimums[3]:
                offspring_crossover[idx, 3] = catalog_minimums[3]
            elif offspring_crossover[idx, 3] > catalog_maximums[3]:
                offspring_crossover[idx, 3] = catalog_maximums[3]

            if offspring_crossover[idx, 4] < catalog_minimums[4]:
                offspring_crossover[idx, 4] = catalog_minimums[4]
            elif offspring_crossover[idx, 4] > catalog_maximums[4]:
                offspring_crossover[idx, 4] = catalog_maximums[4]

    return offspring_crossover
