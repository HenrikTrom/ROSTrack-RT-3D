#!/bin/bash

cd $ROS_WORKSPACE

# rosdep update 
# rosdep install --from-paths src --ignore-src --rosdistro noetic -y --skip-keys libfranka
# catkin_make -DCMAKE_BUILD_TYPE=Release -DFranka_DIR:PATH=/libfranka/build 

source /opt/ros/noetic/setup.bash
if [ -f $ROS_WORKSPACE/devel/setup.bash ]; then
    source /home/docker/workspace/workspace/devel/setup.bash
fi
if [ -f $ROS_WORKSPACE/devel_cb/setup.bash ]; then
    source $ROS_WORKSPACE/devel_cb/setup.bash
fi


export PATH=/opt/cmake-3.26.3-linux-x86_64/bin:$PATH
export LC_NUMERIC="en_US.UTF-8"



# Run the main container command
exec "$@"
