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
RUN apt install -y pybind11-dev
################################################################
# Install Modules
################################################################

RUN mkdir /opt/modules
COPY ./dependencies/Docker-OpenCV-TensorRT-Dev/modules/ /opt/modules/
WORKDIR /opt/modules/cpp-utils
RUN rm -rf build; mkdir build && chmod +x ./build_install.sh && ./build_install.sh
WORKDIR /opt/modules/tensorrt-cpp-api
RUN rm -rf build; mkdir build && chmod +x ./build_install.sh && ./build_install.sh
WORKDIR /opt/modules/detection-inference
RUN rm -rf build; mkdir build && chmod +x ./build_install.sh && ./build_install.sh
WORKDIR /opt/modules/pose-inference
RUN rm -rf build; mkdir build && chmod +x ./build_install.sh && ./build_install.sh

################################################################
# Install libflirmulticamera
################################################################
COPY ./dependencies/FlirMultiCamera/ /opt/modules/FlirMultiCamera/
WORKDIR /opt/modules/FlirMultiCamera
RUN rm -rf build; mkdir build && chmod +x ./build_install.sh && ./build_install.sh
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

# Create local catkin workspace
ENV CATKIN_WS=/home/docker/workspace
ENV ROS_PYTHON_VERSION=3
WORKDIR $CATKIN_WS

# Initialize local catkin workspace
RUN source /opt/ros/${ROS_DISTRO}/setup.bash \
    && cd $CATKIN_WS \
    && rosdep install -y --from-paths . --ignore-src --rosdistro ${ROS_DISTRO}

RUN pip3 install rospkg

################################################################

# Remove potentially conflicting OpenCV libraries from system
RUN rm -rf /usr/lib/x86_64-linux-gnu/libopencv*
RUN rm -rf /usr/lib/x86_64-linux-gnu/cmake/opencv4/

# Update file search database
RUN updatedb

RUN groupadd -g $FLIR_GID flirimaging
# add users to the flirimaging group
RUN usermod -a -G flirimaging docker

# Add custom shell configuration
COPY .bashrc /home/docker/.bashrc

# # Switch to non-root user
USER docker


