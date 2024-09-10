# Generate traffic in CARLA simulator
# Edited from CARLA python examples (https://carla.org//)

class TrafficGenerator:
    def __init__(self, world, traffic_manager):
        self.world = world
        self.traffic_manager = traffic_manager
        self.routes = []
        self.points = []

        self.vehicle_bp_route1 = []
        self.vehicle_bp_route2 = []
        self.vehicle_bp_route3 = []
        self.vehicle_bp_route4 = []

        self.vehicle_color_route1 = []
        self.vehicle_color_route2 = []
        self.vehicle_color_route3 = []
        self.vehicle_color_route4 = []

        self.driverID_route1 = []
        self.driverID_route2 = []
        self.driverID_route3 = []
        self.driverID_route4 = []

        self.vehicles_list = []
        self.walkers_list = []
        self.all_id = []

        self.vehicle_per_route = 0

    def get_actor_blueprints(self, filter, generation):
        bps = self.world.get_blueprint_library().filter(filter)

        if generation.lower() == "all":
            return bps

        # If the filter returns only one bp, we assume that this one needed
        # and therefore, we ignore the generation
        if len(bps) == 1:
            return bps

        try:
            int_generation = int(generation)
            # Check if generation is in available generations
            if int_generation in [1, 2, 3]:
                bps = [x for x in bps if int(x.get_attribute('generation')) == int_generation]
                return bps
            else:
                print("   Warning! Actor Generation is not valid. No actor will be spawned.")
                return []
        except:
            print("   Warning! Actor Generation is not valid. No actor will be spawned.")
            return []

    def create_routes(self, spawn_points):
        # route 1
        route_1_indices = [23, 63, 40, 119, 140, 138, 4, 7, 1, 18, 99, 95, 28, 9, 92, 5, 113, 111, 114]

        route_1 = []
        point_1 = []
        for ind in route_1_indices:
            route_1.append(spawn_points[ind].location)
            point_1.append(spawn_points[ind])

        # route 2
        # route_2_indices = [36, 20, 14, 118, 139, 137, 3, 67, 144, 145, 147, 66, 125, 123, 31]
        route_2_indices = [36, 118, 139, 3, 67, 147, 66, 123, 31, 29, 33, 100, 45, 150]

        # route_2_indices = [36, 118]

        route_2 = []
        point_2 = []
        for ind in route_2_indices:
            route_2.append(spawn_points[ind].location)
            point_2.append(spawn_points[ind])

        # route 3
        route_3_indices = [75, 86, 50, 39, 30, 102, 6, 104, 10, 25, 90, 96, 44, 11, 2, 62, 81, 120, 19, 117, 27, 31, 108,
                           106, 29, 51, 105]

        route_3 = []
        point_3 = []
        for ind in route_3_indices:
            route_3.append(spawn_points[ind].location)
            point_3.append(spawn_points[ind])

        # route 4
        route_4_indices = [78, 126, 73, 128, 130, 133, 134, 12, 121, 77, 116, 24, 57, 108, 106, 51, 105,
                           35, 72, 16, 91, 97, 8, 62, 81, 120, 19, 117, 27, 24]

        route_4 = []
        point_4 = []
        for ind in route_4_indices:
            route_4.append(spawn_points[ind].location)
            point_4.append(spawn_points[ind])

        self.routes = [route_1, route_2, route_3, route_4]
        self.points = [point_1, point_2, point_3, point_4]

    def get_distance(self, loc):
        final_loc = self.routes[1][len(self.routes[1]) - 1]

        dx = abs(final_loc.x - loc.x)
        dy = abs(final_loc.y - loc.y)

        if dx <= 5 and dy <= 5:
            route_complete = True
        else:
            route_complete = False

        return route_complete


    def create_vehicle_bp(self, args):
        blueprints = self.get_actor_blueprints(args.filterv, args.generationv)
        if not blueprints:
            raise ValueError("Couldn't find any vehicles with the specified filters")

        if args.safe:
            blueprints = [x for x in blueprints if x.get_attribute('base_type') == 'car']

        blueprints = sorted(blueprints, key=lambda bp: bp.id)

        spawn_points = self.world.get_map().get_spawn_points()
        number_of_spawn_points = len(spawn_points)

        if args.number_of_vehicles > number_of_spawn_points:
            msg = 'requested %d vehicles, but could only find %d spawn points'
            logging.warning(msg, args.number_of_vehicles, number_of_spawn_points)
            args.number_of_vehicles = number_of_spawn_points

        # create routes
        self.create_routes(spawn_points)

        self.vehicle_per_route = int(args.number_of_vehicles / 4)

        for i in range(self.vehicle_per_route):
            if i * 4 >= args.number_of_vehicles:
                break
            else:
                self.vehicle_bp_route1.append(random.choice(blueprints))
                self.vehicle_bp_route2.append(random.choice(blueprints))
                self.vehicle_bp_route3.append(random.choice(blueprints))
                self.vehicle_bp_route4.append(random.choice(blueprints))

                self.vehicle_color_route1.append(random.choice(self.vehicle_bp_route1[i].get_attribute('color').
                                                               recommended_values))
                self.vehicle_color_route2.append(random.choice(self.vehicle_bp_route2[i].get_attribute('color').
                                                               recommended_values))
                self.vehicle_color_route3.append(random.choice(self.vehicle_bp_route3[i].get_attribute('color').
                                                               recommended_values))
                self.vehicle_color_route4.append(random.choice(self.vehicle_bp_route4[i].get_attribute('color').
                                                               recommended_values))

                if self.vehicle_bp_route1[i].has_attribute('driver_id'):
                    self.driverID_route1.append(random.choice(self.vehicle_bp_route1[i].get_attribute('driver_id').
                                                              recommended_values))
                else:
                    self.driverID_route1.append(0)

                if self.vehicle_bp_route2[i].has_attribute('driver_id'):
                    self.driverID_route2.append(random.choice(self.vehicle_bp_route2[i].get_attribute('driver_id').
                                                              recommended_values))
                else:
                    self.driverID_route2.append(0)

                if self.vehicle_bp_route3[i].has_attribute('driver_id'):
                    self.driverID_route3.append(random.choice(self.vehicle_bp_route3[i].get_attribute('driver_id').
                                                              recommended_values))
                else:
                    self.driverID_route3.append(0)

                if self.vehicle_bp_route4[i].has_attribute('driver_id'):
                    self.driverID_route4.append(random.choice(self.vehicle_bp_route4[i].get_attribute('driver_id').
                                                              recommended_values))
                else:
                    self.driverID_route4.append(0)

    def generate_traffic(self, client, args, synchronous_master):
        SpawnActor = carla.command.SpawnActor
        SetAutopilot = carla.command.SetAutopilot
        FutureActor = carla.command.FutureActor

        # --------------
        # Spawn vehicles
        # --------------
        batch = []

        for i in range(self.vehicle_per_route):
            self.vehicle_bp_route1[i].set_attribute('color', self.vehicle_color_route1[i])
            self.vehicle_bp_route2[i].set_attribute('color', self.vehicle_color_route2[i])
            self.vehicle_bp_route3[i].set_attribute('color', self.vehicle_color_route3[i])
            self.vehicle_bp_route4[i].set_attribute('color', self.vehicle_color_route4[i])

            if self.vehicle_bp_route1[i].has_attribute('driver_id'):
                self.vehicle_bp_route1[i].set_attribute('driver_id', self.driverID_route1[i])

            if self.vehicle_bp_route2[i].has_attribute('driver_id'):
                self.vehicle_bp_route2[i].set_attribute('driver_id', self.driverID_route2[i])

            if self.vehicle_bp_route3[i].has_attribute('driver_id'):
                self.vehicle_bp_route3[i].set_attribute('driver_id', self.driverID_route3[i])

            if self.vehicle_bp_route4[i].has_attribute('driver_id'):
                self.vehicle_bp_route4[i].set_attribute('driver_id', self.driverID_route4[i])

            self.vehicle_bp_route1[i].set_attribute('role_name', 'autopilot')
            self.vehicle_bp_route2[i].set_attribute('role_name', 'autopilot')
            self.vehicle_bp_route3[i].set_attribute('role_name', 'autopilot')
            self.vehicle_bp_route4[i].set_attribute('role_name', 'autopilot')

            batch.append(SpawnActor(self.vehicle_bp_route1[i], self.points[0][i]).then(
                SetAutopilot(FutureActor, True, self.traffic_manager.get_port())))

            batch.append(SpawnActor(self.vehicle_bp_route2[i], self.points[1][i]).then(
                SetAutopilot(FutureActor, True, self.traffic_manager.get_port())))

            batch.append(SpawnActor(self.vehicle_bp_route3[i], self.points[2][i]).then(
                SetAutopilot(FutureActor, True, self.traffic_manager.get_port())))

            batch.append(SpawnActor(self.vehicle_bp_route4[i], self.points[3][i]).then(
                SetAutopilot(FutureActor, True, self.traffic_manager.get_port())))

        for response in client.apply_batch_sync(batch, synchronous_master):
            if response.error:
                logging.error(response.error)
            else:
                self.vehicles_list.append(response.actor_id)

        # -------------
        # Spawn Walkers
        # -------------
        blueprintsWalkers = self.get_actor_blueprints(args.filterw, args.generationw)
        if not blueprintsWalkers:
            raise ValueError("Couldn't find any walkers with the specified filters")

        # some settings
        percentagePedestriansRunning = 0.0  # how many pedestrians will run

        if args.seedw:
            self.world.set_pedestrians_seed(args.seedw)
            random.seed(args.seedw)
        # 1. take all the random locations to spawn
        spawn_points = []
        for i in range(args.number_of_walkers):
            spawn_point = carla.Transform()
            loc = self.world.get_random_location_from_navigation()
            if loc != None:
                spawn_point.location = loc
                spawn_points.append(spawn_point)
        # 2. we spawn the walker object
        batch = []
        walker_speed = []
        for spawn_point in spawn_points:
            walker_bp = random.choice(blueprintsWalkers)
            # set as not invincible
            if walker_bp.has_attribute('is_invincible'):
                walker_bp.set_attribute('is_invincible', 'false')
            # set the max speed
            if walker_bp.has_attribute('speed'):
                if (random.random() > percentagePedestriansRunning):
                    # walking
                    walker_speed.append(walker_bp.get_attribute('speed').recommended_values[1])
                else:
                    # running
                    walker_speed.append(walker_bp.get_attribute('speed').recommended_values[2])
            else:
                print("Walker has no speed")
                walker_speed.append(0.0)
            batch.append(SpawnActor(walker_bp, spawn_point))
        results = client.apply_batch_sync(batch, True)
        walker_speed2 = []
        for i in range(len(results)):
            if results[i].error:
                logging.error(results[i].error)
            else:
                self.walkers_list.append({"id": results[i].actor_id})
                walker_speed2.append(walker_speed[i])
        walker_speed = walker_speed2
        # 3. we spawn the walker controller
        batch = []
        walker_controller_bp = self.world.get_blueprint_library().find('controller.ai.walker')
        for i in range(len(self.walkers_list)):
            batch.append(SpawnActor(walker_controller_bp, carla.Transform(), self.walkers_list[i]["id"]))
        results = client.apply_batch_sync(batch, True)
        for i in range(len(results)):
            if results[i].error:
                logging.error(results[i].error)
            else:
                self.walkers_list[i]["con"] = results[i].actor_id
        # 4. we put together the walkers and controllers id to get the objects from their id
        for i in range(len(self.walkers_list)):
            self.all_id.append(self.walkers_list[i]["con"])
            self.all_id.append(self.walkers_list[i]["id"])
        all_actors = self.world.get_actors(self.all_id)

        self.set_walker_controller(all_actors, walker_speed)

        return self.vehicles_list, self.walkers_list, all_actors

    def set_routes(self, all_vehicle_actors):
        list_actor = self.world.get_actors()
        for actor_ in list_actor:
            if isinstance(actor_, carla.TrafficLight):
                actor_.set_state(carla.TrafficLightState.Green)
                actor_.set_green_time(1000000.0)

        vehicle_per_route = int(len(all_vehicle_actors) / 4)

        for i in range(vehicle_per_route):
            route = self.routes[0][i:len(self.routes[0])] + self.routes[0][0:i]
            self.traffic_manager.random_left_lanechange_percentage(all_vehicle_actors[i * 4], 0)
            self.traffic_manager.random_right_lanechange_percentage(all_vehicle_actors[i * 4], 0)
            self.traffic_manager.set_path(all_vehicle_actors[i * 4], route)
            all_vehicle_actors[i * 4].set_light_state(carla.VehicleLightState.NONE)

            route = self.routes[1][i:len(self.routes[1])] + self.routes[1][0:i]
            self.traffic_manager.random_left_lanechange_percentage(all_vehicle_actors[i * 4 + 1], 0)
            self.traffic_manager.random_right_lanechange_percentage(all_vehicle_actors[i * 4 + 1], 0)
            self.traffic_manager.auto_lane_change(all_vehicle_actors[i * 4 + 1], False)
            self.traffic_manager.set_path(all_vehicle_actors[i * 4 + 1], route)
            all_vehicle_actors[i * 4 + 1].set_light_state(carla.VehicleLightState.NONE)

            route = self.routes[2][i:len(self.routes[2])] + self.routes[2][0:i]
            self.traffic_manager.random_left_lanechange_percentage(all_vehicle_actors[i * 4 + 2], 0)
            self.traffic_manager.random_right_lanechange_percentage(all_vehicle_actors[i * 4 + 2], 0)
            self.traffic_manager.set_path(all_vehicle_actors[i * 4 + 2], route)
            all_vehicle_actors[i * 4 + 2].set_light_state(carla.VehicleLightState.NONE)

            route = self.routes[3][i:len(self.routes[3])] + self.routes[3][0:i]
            self.traffic_manager.random_left_lanechange_percentage(all_vehicle_actors[i * 4 + 3], 0)
            self.traffic_manager.random_right_lanechange_percentage(all_vehicle_actors[i * 4 + 3], 0)
            self.traffic_manager.set_path(all_vehicle_actors[i * 4 + 3], route)
            all_vehicle_actors[i * 4 + 3].set_light_state(carla.VehicleLightState.NONE)

    def set_walker_controller(self, all_actors, walker_speed):
        percentagePedestriansCrossing = 0.0  # how many pedestrians will walk through the road

        # 5. initialize each controller and set target to walk to (list is [controler, actor, controller, actor ...])
        # set how many pedestrians can cross the road
        self.world.set_pedestrians_cross_factor(percentagePedestriansCrossing)
        for i in range(0, len(self.all_id), 2):
            # start walker
            all_actors[i].start()
            # set walk to random point
            all_actors[i].go_to_location(self.world.get_random_location_from_navigation())
            # max speed
            all_actors[i].set_max_speed(float(walker_speed[int(i / 2)]))

    def destroy_traffic(self, client, all_actors):
        print('\ndestroying %d vehicles' % len(self.vehicles_list))
        client.apply_batch([carla.command.DestroyActor(x) for x in self.vehicles_list])

        # stop walker controllers (list is [controller, actor, controller, actor ...])
        for i in range(0, len(self.all_id), 2):
            all_actors[i].stop()

        print('\ndestroying %d walkers' % len(self.walkers_list))
        client.apply_batch([carla.command.DestroyActor(x) for x in self.all_id])

        self.vehicles_list = []
        self.walkers_list = []
        self.all_id = []
