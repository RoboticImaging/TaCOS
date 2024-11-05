TaCOS: Task-Specific Camera Optimization with Simulation
===============
Please refer to the Stereo Camera Design folder for the implementation of the stereo camera design experiment, and the Monocular Camera Design folder for the implementation of the monocular camera design experiment. 

## Stereo Camera Design
### Simulator Setup
We use the [**CARLA Simulator**](https://carla.org//) version 0.9.15 as the virtual environment, please follow the official installation instructions to install the simulator.

### Camera Optimization
To start the optimization, first start to run the CARLA simulator then run main.py. The pretrained PSMNet can be downloaded from their [**GitHub repository**](https://github.com/JiaRenChang/PSMNet).

## Monocular Camera Design
### Simulator Setup
The simulator that procedurally generates virtual indoor environments is placed in the ProceduralGeneration folder. To build this project, the following steps need to be performed:
1. Download [**Unreal Engine 5.3**](https://www.unrealengine.com/en-US/download) from the official website
2. Download the ProceduralGeneration folder of this repository to a preferred disk location, make sure the location has a disk space of a minimum of 30GB.
3. Download the assets used for this project from Unreal Engine Marketplace and add them to the project. However, other preferred application-specific assets can be added to this project if required. The assets we used include:
   * [**4K Materials: Wood Flooring Vol.01**](https://www.unrealengine.com/marketplace/en-US/product/4k-materials-wood-flooring)
   * [**Advanced Glass Material Pack**](https://www.unrealengine.com/marketplace/en-US/product/advanced-glass-material-pack)
   * Assets from project [**Triplex House Villa**](https://www.unrealengine.com/marketplace/en-US/product/big-triplex-house-villa)
   * [**Free Furniture Pack**](https://www.unrealengine.com/marketplace/en-US/product/a4907129f69c44a892f76782489736ab)
   * [**Loft Office**](https://www.unrealengine.com/marketplace/en-US/product/loft-office-modular)
   * [**Chairs & Tables Pack 1**](https://www.unrealengine.com/marketplace/en-US/product/twinmotion-chairs-tables-pack-1)
   * [**High Tech Pack 1**](https://www.unrealengine.com/marketplace/en-US/product/twinmotion-high-tech-pack-1)
   * [**Storages Pack 1**](https://www.unrealengine.com/marketplace/en-US/product/twinmotion-storages-pack-1)
4. Download the [**TCP Socket Plugin**](https://github.com/CodeSpartan/UE4TcpSocketPlugin) to communicate with Python programs
5. Run the .unproject file or build from source code by generating project files with this file.

### Camera Optimization
The codes that optimize the camera are placed in the CameraOptimization folder. To start the camera optimization, first click the Run button in the Unreal Engine Editor and then run GeneticAlgorithmMain.py.

## Citation
If you find our work useful, please cite the below paper:
```
@inproceedings{yan2025tacos,
  title={{TaCOS}: Task-Specific Camera Optimization with Simulation},
  author={Yan, Chengyang and Dansereau, Donald G},
  booktitle={Proceedings of the IEEE/CVF Winter Conference on Applications of Computer Vision (WACV)},
  year={2025}
}
```
