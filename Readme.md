# 🐳 Flir-Multi-Camera-ROS

[![DOI](https://zenodo.org/badge/991268455.svg)](https://zenodo.org/badge/latestdoi/991268455)

Docker-Container enabling Real-Time 3D Tracking with ROS and ROS2.

Writing and designing this project's software was a main focus during my PhD.

## Main Features 
- Robot to vision system calibration
- Real time 3D object tracking for simple objects and and human body regions
- Real time 3D keypoint tracking for human pose estimation and post-processing

## 📑 Citation

If you use this software, please use the GitHub **“Cite this repository”** button at the top(-right) of this page.

TODO: 4 branches: 

- [] NOETIC: cam-only
- [] NOETIC: trt-online-tracking
- [] HUBLE: ros
- [] HUMBLE: trt-online-tracking

## Installation

```bash
git clone git@github.com:HenrikTrom/Docker-Flir-Multi-Camera.git
cd Docker-Flir-Multi-Camera
git submodule update --init --remote --recursive
```

### Requirements

If you want real-time 3D tracking you need:
* [Synchronizable Flir Cameras](https://flir.custhelp.com/app/answers/detail/a_id/3385/~/flir-cameras---trigger-vs.-sync-vs.-record-start), we are using 5x Flir Grasshopper GS3-U3-32S-4C
* USB Card to handle all the video input
* Synchronization cable that connects master cameras Trigger Pin to the Slave cameras input pins.
* TensorRT capable GPU (i.e. [Nvidia 3000 Series](https://www.nvidia.com/en-us/geforce/graphics-cards/30-series/), [Nvidia 4000 Series](https://www.nvidia.com/en-us/geforce/graphics-cards/40-series/), etc.)
* A powerfull processor (16+ cores)
* An Ubuntu installation (20.04, 22.04, etc.)
* Installed the [Docker engine](https://docs.docker.com/engine/install/ubuntu/), and follow the [post installation steps](https://docs.docker.com/engine/install/linux-postinstall/)

### Prerequisites

1. Download and install the [Spinnaker SDK](https://www.teledynevisionsolutions.com/products/spinnaker-sdk/?model=Spinnaker%20SDK&vertical=machine%20vision&segment=iisflir ). Make sure that you can run Spinview and that you can get a stable video of each camera.
2. Install the [Nvidia drivers](https://documentation.ubuntu.com/server/how-to/graphics/install-nvidia-drivers/index.html). Test with `nvidia-smi`. 
3. Test if the base container works: The project depends on [this submodule](https://github.com/HenrikTrom/Docker-OpenCV-TensorRT-Dev). Clone the repo and run the tests to make sure your TensorRT installation and models for detection pose inference work correctly.

**Note:** The cuda version in your container must be lower as the one on your machine. **Cuda 12.3 was the highest version I used in this container was as higher versions are currently incopatible with OpenCV.**

#### Before building the container:

1. Place the Spinnaker SDK archive (*.tar.gz) in ./build/spinnaker
2. Download the [TensorRT SDK](https://developer.nvidia.com/tensorrt) archive (*.tar.gz) and place it in `./build/dependencies/Docker-OpenCV-TensorRT-Dev/build/vision_dependencies/tensorrt`. Modify `./build/dependencies/Docker-OpenCV-TensorRT-Dev/build/vision_dependencies/tensorrt/install.sh` so that it matches the archive.
3. Adapt the `TENSORRT_VERSION` in `.env` and `./build/spinnaker/install_spinnaker.sh` to the correct version
4. Adapt other parameters in your .env file i.e. Serial numbers of your cameras, Master-Slave Trigger lines etc.
5. Add these environment variables to your system (**If you are using zsh, modify the code.**)
```bash
echo 'export ROS_MASTER_URI=<ip-of-ros-master>' >> ~/.bashrc 
echo 'export ROS_IP=<your-ip-(ifconfig)>' >> ~/.bashrc
source ~/.bashrc
```

### Build and run the container(s)
```bash
docker compose --profile build-only build # build the base container
docker compose build # builds the main container
docker compose up -d # launches the main container as background process
```

### Executables
```bash
# TODO
```

### 🧪 Tested with
* TensorRT-8.6.1.6.Linux.x86_64-gnu.cuda-11.8.tar.gz on NVIDIA GTX 2080 Super on Ubuntu 20.04, 22.04
* TensorRT-10.9.0.34.Linux.x86_64-gnu.cuda-12.8.tar.gz on NVIDIA RTX 4070 Super on Ubuntu 20.04,22.04
* OpenCV 10.0.0
