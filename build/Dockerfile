ARG BASE_IMAGE=my/opencv-tensorrt-dev

FROM ${BASE_IMAGE}

USER root

## Install: Spinnaker
################################################################

RUN mkdir /opt/spinnaker
COPY ./spinnaker/ /opt/spinnaker/
WORKDIR /opt/spinnaker
ENV DEBIAN_FRONTEND="noninteractive"
ENV DEBIAN_FRONTEND="teletype"
RUN chmod +x ./install_spinnaker_auto.sh
RUN chmod +x ./install_spinnaker.sh && ./install_spinnaker.sh

################################################################
# Set environment variables
################################################################
ARG FLIR_CAMERA_SERIAL_NUMBERS
ARG FLIR_CAMERA_COUNT
ARG FLIR_MASTER_CAM_SERIAL
ARG FLIR_TOP_CAM_SERIAL
ARG FLIR_MASTER_LINE
ARG FLIR_SLAVE_LINE
ARG FLIR_GID
ARG CONTAINER_NAME
ENV FLIR_GID=${FLIR_GID}
ENV FLIR_CAMERA_COUNT=${FLIR_CAMERA_COUNT}
ENV CONTAINER_NAME=${CONTAINER_NAME}
ENV BATCH_SIZE=${FLIR_CAMERA_COUNT}

RUN echo "FLIR_GID: ${FLIR_GID}"
RUN echo "FLIR_CAMERA_COUNT: ${FLIR_CAMERA_COUNT}"

################################################################
# Extra packages
################################################################
RUN apt-get update && apt-get install -y \
    pybind11-dev \
    python3-dev \
    python3-pip \
    python3-setuptools \
    python3-numpy \
    && apt-get clean && rm -rf /var/lib/apt/lists/*
################################################################
# Install Modules
################################################################

RUN mkdir /opt/modules
COPY ./dependencies/Docker-OpenCV-TensorRT-Dev/modules/ /opt/modules/
WORKDIR /opt/modules/cpp_utils
RUN rm -rf build; mkdir build && chmod +x ./build_install.sh && ./build_install.sh
WORKDIR /opt/modules/tensorrt-cpp-api
RUN rm -rf build; mkdir build && chmod +x ./build_install.sh && ./build_install.sh
WORKDIR /opt/modules/detection_inference
RUN rm -rf inputs/*.jpg && rm -rf models/*.onnx rm -rf models/*.engine.*
RUN rm -rf build; mkdir build && chmod +x ./build_install.sh && ./build_install.sh
WORKDIR /opt/modules/pose_inference
RUN rm -rf inputs/*.jpg && rm -rf models/*.onnx rm -rf models/*.engine.*
RUN rm -rf build; mkdir build && chmod +x ./build_install.sh && ./build_install.sh

################################################################
# Install libflirmulticamera
################################################################
COPY ./dependencies/flirmulticamera/ /opt/modules/flirmulticamera/
WORKDIR /opt/modules/flirmulticamera
RUN rm -rf build; mkdir build && chmod +x ./build_install.sh && ./build_install.sh
################################################################
# Install ROS Noetic
################################################################
RUN apt-get update && apt-get install -y --no-install-recommends \
    curl gnupg2 ca-certificates lsb-release && \
    rm -rf /var/lib/apt/lists/*


# Add ROS key to a dedicated keyring (no apt-key)
RUN curl -fsSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key \
    | gpg --dearmor -o /usr/share/keyrings/ros-archive-keyring.gpg


# Add ROS repository with signed-by pinning
RUN echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] \
http://packages.ros.org/ros/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" \
    > /etc/apt/sources.list.d/ros1-latest.list

# Environment
ENV LANG=C.UTF-8 LC_ALL=C.UTF-8 ROS_DISTRO=noetic

# Base build tools
RUN apt-get update && apt-get install -y --no-install-recommends \
    software-properties-common build-essential && \
    add-apt-repository universe && \
    rm -rf /var/lib/apt/lists/*

# Core ROS (pin exact versions if you need reproducibility)
RUN apt-get update && apt-get install -y --no-install-recommends \
    ros-noetic-ros-core=1.5.0-1* \
    ros-noetic-ros-base=1.5.0-1* && \
    rm -rf /var/lib/apt/lists/*

# Utilities
RUN apt-get update && apt-get install -y --no-install-recommends \
    python3-rosdep python3-rosinstall python3-vcstools python3-catkin-tools \
    ros-noetic-tf ros-noetic-image-exposure-msgs ros-noetic-camera-info-manager \
    ros-noetic-image-transport ros-noetic-roslint ros-noetic-image-proc && \
    rm -rf /var/lib/apt/lists/*

# rosdep
RUN rosdep init && rosdep update --rosdistro $ROS_DISTRO

RUN pip3 install --no-cache-dir rospkg
################################################################

# Remove potentially conflicting OpenCV libraries from system
RUN rm -rf /usr/lib/x86_64-linux-gnu/libopencv*
RUN rm -rf /usr/lib/x86_64-linux-gnu/cmake/opencv4/

## Final Setup: User Environment and Permissions
################################################################

# Create non-root user with sudo permissions
COPY ./setup/user.sh /tmp/user.sh
RUN chmod +x /tmp/user.sh && /tmp/user.sh

RUN groupadd -g $FLIR_GID flirimaging
# add users to the flirimaging group
RUN usermod -a -G flirimaging docker

# create catkin ws
# Create local catkin workspace
ENV ROS_WORKSPACE=/home/docker/workspace/workspace
ENV ROS_PYTHON_VERSION=3
WORKDIR $ROS_WORKSPACE

# Initialize local catkin workspace
RUN source /opt/ros/${ROS_DISTRO}/setup.bash \
    && cd $ROS_WORKSPACE \
    && rosdep install -y --from-paths . --ignore-src --rosdistro ${ROS_DISTRO}

# entrypoint stuff
COPY ./entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]

# Add custom shell configuration
COPY .bashrc /home/docker/.bashrc

USER docker


