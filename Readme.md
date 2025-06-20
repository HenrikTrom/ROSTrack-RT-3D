# 🚀 ROSTrack-RT-3D

[![DOI](https://zenodo.org/badge/991268455.svg)](https://zenodo.org/badge/latestdoi/991268455)



Docker-Container enabling a simple image stream from a single or multiple flir cameras.

Writing and designing this project's software was a main focus during my PhD.

## 📑 Citation

If you use this software, please use the GitHub **“Cite this repository”** button at the top(-right) of this page.

TODO: 4 branches: 

- [x] NOETIC: cam-only
- [x] NOETIC: trt-online-tracking
- [] HUBLE: cam-only
- [] HUMBLE: trt-online-tracking

## Installation

```bash
git clone git@github.com:HenrikTrom/Docker-Flir-Multi-Camera.git
cd Docker-Flir-Multi-Camera
git submodule update --init --remote --recursive
```

### Requirements

You need:
* [Synchronizable Flir Cameras](https://flir.custhelp.com/app/answers/detail/a_id/3385/~/flir-cameras---trigger-vs.-sync-vs.-record-start), we are using 5x Flir Grasshopper GS3-U3-32S-4C
* USB Card to handle all the video input
* Synchronization cable that connects master cameras Trigger Pin to the Slave cameras input pins.
* An Ubuntu installation (20.04, 22.04, etc.)
* Installed the [Docker engine](https://docs.docker.com/engine/install/ubuntu/), and follow the [post installation steps](https://docs.docker.com/engine/install/linux-postinstall/)

### Prerequisites

1. Download and install the [Spinnaker SDK](https://www.teledynevisionsolutions.com/products/spinnaker-sdk/?model=Spinnaker%20SDK&vertical=machine%20vision&segment=iisflir ). Make sure that you can run Spinview and that you can get a stable video of each camera.

#### Before building the container:

1. Place the Spinnaker SDK archive (*.tar.gz) in ./build/spinnaker
2. Adapt the spinnaker file name in `./build/spinnaker/install_spinnaker.sh` to the correct version
4. Adapt other parameters in your .env file i.e. Serial numbers of your cameras, Master-Slave Trigger lines etc.
5. Add these environment variables to your system (**If you are using zsh, modify the code.**)
```bash
echo 'export ROS_MASTER_URI=<ip-of-ros-master>' >> ~/.bashrc 
echo 'export ROS_IP=<your-ip-(ifconfig)>' >> ~/.bashrc
source ~/.bashrc
```

### Build and run the container(s)
```bash
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
