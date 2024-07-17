"""
Utility functions
"""

import numpy

# Compute distance with 3 parameters
def distance_3d(x, y, z, x0, y0, z0):
    """
    3d distance from a point and a line
    """
    dx = x - x0
    dy = y - y0
    dz = z - z0
    d = numpy.sqrt(dx**2 + dy**2 + dz**2)
    return d

# find closest sensor from the catalog
def get_closest_sensor(selected_params, image_width, image_height, pixel_size):
    distance = distance_3d(image_width, image_height, pixel_size, selected_params[0], selected_params[1], selected_params[2])
    distance = numpy.round(distance, 5)
    glob_min_idxs = numpy.where(distance == numpy.min(distance))
    return glob_min_idxs

# get sensor parameters from the catalog
def get_params_from_catalog(pop, image_width, image_height, pixel_size, sol_per_pop):
    idx = numpy.empty(sol_per_pop)

    for sol in range(sol_per_pop):
        closet_sensor_idx = get_closest_sensor(pop[sol, 2:5], image_width, image_height, pixel_size)
        closet_sensor_idx = [a for b in closet_sensor_idx for a in b]
        pop[sol, 2] = float(image_width[closet_sensor_idx[0]])
        pop[sol, 3] = float(image_height[closet_sensor_idx[0]])
        pop[sol, 4] = float(pixel_size[closet_sensor_idx[0]])
        idx[sol] = closet_sensor_idx[0]

    return pop, idx

# convert pixel size to number of pixels
def pixel_size_2_res(pop, idx, resX, resY, sol_per_pop):
    current_pop = numpy.empty((sol_per_pop, 6))
    current_pop[:, 0:4] = pop[:, 0:4]

    for sol in range(sol_per_pop):
        current_pop[sol, 4] = float(resX[idx[sol]])
        current_pop[sol, 5] = float(resY[idx[sol]])

    return current_pop

#  get parameters from catalog based on sensor index
def get_params_with_idx(pop, image_sensor_catalog, sol_per_pop):
    pop_values = numpy.empty((sol_per_pop, 6))
    pop_values[:, 0:2] = pop[:, 0:2]

    for sol in range(sol_per_pop):
        pop_values[sol, 2] = image_sensor_catalog.loc[pop[sol, 2]].at["Width"]
        pop_values[sol, 3] = image_sensor_catalog.loc[pop[sol, 2]].at["Height"]
        pop_values[sol, 4] = image_sensor_catalog.loc[pop[sol, 2]].at["ResX"]
        pop_values[sol, 5] = image_sensor_catalog.loc[pop[sol, 2]].at["ResY"]

    return pop_values

# check obstacle detection accuracy
def get_threshold_accuracy(data):
    lower_count = (data[0] + data[1])

    if lower_count > 0:
        return data[0] / (data[0] + data[1])
    else:
        return 0.5
