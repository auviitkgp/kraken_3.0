# Kraken 3.0 - IIT Kharagpur

This is the repository for the code on the vehicle Kraken 3.0.

Written on top of [ROS](http://ros.org).

The build system is [rosbuild](http://wiki.ros.org/rosbuild).

You would need a [ROSbuild workspace](http://wiki.ros.org/catkin/Tutorials/using_rosbuild_with_catkin#rosbuild_workspace_with_groovy_and_later) to work with this repository.

## Launching the simulator : Start visualisation

```
roscd
cd kraken_3.0
roslaunch Scripts/launch/simulator/imuDvl.launch
```

## Launching the simulator : Start Yaw control

```
roscd
cd kraken_3.0
roslaunch Scripts/launch/simulator/yawControl.launch
```
