ARG BASE_IMAGE=ubuntu:20.04

FROM ${BASE_IMAGE}

LABEL maintainer="Henrik Trommer <henrik.trommer@uni-goettingen.de>"

SHELL ["/bin/bash","-c"]

# Set system timezone to UTC
RUN echo 'Etc/UTC' > /etc/timezone && \
    ln -s /usr/share/zoneinfo/Etc/UTC /etc/localtime && \
    apt-get update && \
    apt-get install -q -y --no-install-recommends tzdata && \
    rm -rf /var/lib/apt/lists/*

# Install GPG and certificate tools for adding external repositories
RUN apt update && apt install -q -y --no-install-recommends \
    dirmngr \
    gnupg2 \
    && rm -rf /var/lib/apt/lists/*

################################################################
# Essential build tools and dependencies
################################################################

# Install build tools and software properties
RUN apt update && apt install --no-install-recommends -y \
    software-properties-common \
    build-essential

# Install common development libraries and utilities
RUN apt update --fix-missing && apt install --no-install-recommends -y \
    wget git bzip2 ca-certificates zlib1g-dev \
    libncurses5-dev libgdbm-dev libglib2.0-0 libxext6 libsm6 \
    libxrender1 libffi-dev libusb-1.0-0-dev \
    && rm -rf /var/lib/apt/lists/*

# Install additional helpful CLI tools and math libraries
RUN apt update && apt install --no-install-recommends -y \
    tree nano libeigen3-dev

################################################################
## Python Environment Setup
################################################################

# Install Python and pip
RUN apt update && apt install --no-install-recommends -y \
    python3 \
    python3-pip

# Install numerical and visualization packages
RUN pip3 install numpy==1.24.4 
RUN pip3 install opencv-contrib-python

################################################################
## X11 and debugging utilities
################################################################

# Enable X11 forwarding and archive tools
RUN apt-get update -y && apt-get install -y \
    x11-apps xauth zip

# Install multimedia, JSON, debugging, and system tools
RUN apt -y install ffmpeg libavdevice-dev rapidjson-dev gdb
RUN apt -y install zlib1g unzip mlocate
RUN apt -y install usbutils pciutils htop iputils-ping

# Install sudo for non-root user
RUN apt update && apt install --no-install-recommends -y sudo

################################################################
## CMake Installation (custom version)
################################################################

ARG CMAKE_VERSION
ENV CMAKE_VERSION=${CMAKE_VERSION}

# Install custom version of CMake
RUN apt-get update && apt-get install -y wget curl ca-certificates \
    && wget https://github.com/Kitware/CMake/releases/download/v${CMAKE_VERSION}/cmake-${CMAKE_VERSION}-linux-x86_64.sh \
    && mkdir /opt/cmake \
    && sh cmake-${CMAKE_VERSION}-linux-x86_64.sh --skip-license --prefix=/opt/cmake \
    && ln -s /opt/cmake/bin/cmake /usr/local/bin/cmake \
    && rm cmake-${CMAKE_VERSION}-linux-x86_64.sh

    
################################################################
## Additional variables
################################################################
RUN apt -y install libspdlog-dev libfmt-dev 
ARG OPENCV_VERSION
ARG UID
ARG GID
ARG CPP_OPTIMIZATIONS
# Export args as environment variables
ENV OPENCV_VERSION=${OPENCV_VERSION}
ENV CPP_OPTIMIZATIONS=${CPP_OPTIMIZATIONS}

################################################################
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
ENV FLIR_CAMERA_SERIAL_NUMBERS=${FLIR_CAMERA_SERIAL_NUMBERS}
ENV FLIR_CAMERA_COUNT=${FLIR_CAMERA_COUNT}
ENV FLIR_MASTER_CAM_SERIAL=${FLIR_MASTER_CAM_SERIAL}
ENV FLIR_TOP_CAM_SERIAL=${FLIR_TOP_CAM_SERIAL}
ENV FLIR_MASTER_LINE=${FLIR_MASTER_LINE}
ENV FLIR_SLAVE_LINE=${FLIR_SLAVE_LINE}
ENV CONTAINER_NAME=${CONTAINER_NAME}
ENV BATCH_SIZE=${FLIR_CAMERA_COUNT}

RUN echo "FLIR_GID: ${FLIR_GID}"
RUN echo "FLIR_CAMERA_SERIAL_NUMBERS: ${FLIR_CAMERA_SERIAL_NUMBERS}"
RUN echo "FLIR_CAMERA_COUNT: ${FLIR_CAMERA_COUNT}"
RUN echo "FLIR_MASTER_CAM_SERIAL: ${FLIR_MASTER_CAM_SERIAL}"
RUN echo "FLIR_TOP_CAM_SERIAL: ${FLIR_TOP_CAM_SERIAL}"
RUN echo "FLIR_MASTER_LINE: ${FLIR_MASTER_LINE}"
RUN echo "FLIR_SLAVE_LINE: ${FLIR_SLAVE_LINE}"

RUN actual_count=$(echo "$FLIR_CAMERA_SERIAL_NUMBERS" | tr ',' '\n' | wc -l) && \
    if [ "$actual_count" -ne "$FLIR_CAMERA_COUNT" ]; then \
        echo "ERROR: FLIR_CAMERA_COUNT ($FLIR_CAMERA_COUNT) does not match number of FLIR_CAMERA_SERIAL_NUMBERS ($actual_count)" >&2; \
        exit 1; \
    fi

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
# Install ROS Noetic
################################################################
# setup keys
RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys C1CF6E31E6BADE8868B172B4F42ED6FBAB17C654

# setup sources.list
RUN echo "deb http://packages.ros.org/ros/ubuntu focal main" > /etc/apt/sources.list.d/ros1-latest.list

# setup environment
ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8
ENV ROS_DISTRO noetic

RUN apt update && apt install --no-install-recommends -y \
    software-properties-common \
    build-essential
RUN add-apt-repository universe

# Install ros packages but build cv-bridge from source! to use correct opencv version
RUN apt update && apt install --no-install-recommends -y \
    ros-noetic-ros-core=1.5.0-1* \
    ros-noetic-ros-base=1.5.0-1*

RUN apt update && apt install --no-install-recommends -y \
    python3-rosdep \
    python3-rosinstall \
    python3-vcstools \
    python3-catkin-tools \
    ros-noetic-tf \
    ros-noetic-image-exposure-msgs \
    ros-noetic-camera-info-manager \
    ros-noetic-image-transport \
    ros-noetic-roslint \
    ros-noetic-image-proc

# bootstrap rosdep
RUN rosdep init && \
  rosdep update --rosdistro $ROS_DISTRO

RUN pip3 install rospkg

################################################################
# Install Modules
################################################################

RUN mkdir /opt/modules
COPY ./dependencies/cpp_utils/ /opt/modules/cpp_utils/
WORKDIR /opt/modules/cpp_utils
RUN rm -rf build; mkdir build && chmod +x ./build_install.sh && ./build_install.sh

################################################################
# Install libflirmulticamera
################################################################
COPY ./dependencies/flirmulticamera/ /opt/modules/flirmulticamera/
WORKDIR /opt/modules/flirmulticamera
RUN rm -rf build; mkdir build && chmod +x ./build_install.sh && ./build_install.sh

# Update file search 
RUN updatedb

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


