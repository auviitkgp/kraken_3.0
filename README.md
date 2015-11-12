# Kraken 3.0 - IIT Kharagpur

This is the repository for the code on the vehicle Kraken 3.0.

Written on top of [ROS](http://ros.org).

The build system is [rosbuild](http://wiki.ros.org/rosbuild).

You would need a [ROSbuild workspace](http://wiki.ros.org/catkin/Tutorials/using_rosbuild_with_catkin#rosbuild_workspace_with_groovy_and_later) to work with this repository.

### System Requirements

- Tested on Ubuntu 14.04 LTS
- A full installation of ROS Indigo
- `rosbuild` workspace
- Packages as listed in the [packages](#packages) section below.

### Packages

- To run yaw control you need to run the following commands

  ```
  $ apt-get install python-numpy python-scipy python-matplotlib ipython 
  $ apt-get install python-notebook python-pandas python-sympy python-nose
  $ pip install scikit-fuzzy
  ```
  
  If the `setyaw` package does not run even after successfuly installation of the above packages,
  open an issue [here](https://github.com/auviitkgp/kraken_3.0/issues/new).
  
- The **Artistic Style** package for running the `formatAll.sh` hook.
  
  **TODO**


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
