# ENPM661 Project 4 - Pick and Place with MoveIt2

Pick and place simulation using the Franka Panda arm with MoveIt2 in ROS2 Humble.

**Yashal Ahmed | UID: 122288393**

## Overview

This project implements a pick and place motion using the Franka Panda robotic arm in RViz2 simulation. The robot starts at a home position, picks an object from the right side, places it on the left side, and returns home. MoveIt2 is used for motion planning with the OMPL planner.

## Dependencies

- Ubuntu 22.04
- ROS2 Humble
- MoveIt2 (ros-humble-moveit)
- ros2_control (ros-humble-ros2-control, ros-humble-ros2-controllers)
- rmw-cyclonedds (ros-humble-rmw-cyclonedds-cpp)
- ws_moveit2 built from source

## Run Instructions

1. Source the environment and build:

    source /opt/ros/humble/setup.bash
    source ~/ws_moveit2/install/setup.bash
    export RMW_IMPLEMENTATION=rmw_cyclonedds_cpp
    cd ~/project4_ws && colcon build
    export AMENT_PREFIX_PATH=~/project4_ws/install/package_122288393:~/project4_ws/install/panda_movit_config:~/project4_ws/install/moveit_resources_panda_description:$AMENT_PREFIX_PATH

2. Terminal 1 - Launch MoveIt demo:

    ros2 launch panda_movit_config demo.launch.py

3. Terminal 2 - Run pick and place:

    ros2 launch package_122288393 pick_and_place.launch.py

## Output

- RViz2 window showing the Panda robot executing the pick and place motion
- Terminal logs showing each step completing successfully

## File Structure

    package_122288393/
    CMakeLists.txt
    package.xml
    README.md
    src/pick_and_place.cpp
    launch/pick_and_place.launch.py
