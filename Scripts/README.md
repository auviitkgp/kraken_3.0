# Scripts

### Bash Scripts (TODO: Arrange this properly)

- `kraken_visualisation_launcher` - Script to run uwsim visualisation module
- `kraken_visualisation_joystick_launcher` - Launch file to run kraken simulator with joystick control
- `runKraken.launch`
- `runKraken.sh` - 
- `runKraken_joystick.sh` - Script to run kraken simulator with joystick control
- `stopKraken.sh` - Script to stop the `runKraken.sh` script
- `makeKraken.sh` - Script to make all the project packages at once

### ROS Launch Files (inside launch/)

#### Folder `simulator`

These files can be run on any machine that has all the dependencies 
installed. We are working to list all the dependencies here.

File name | Short description
:---: | :---
`imuDvl.launch` | Launches the Simulator with the visualisation.
`yawControl.launch` | Launches the simulator and the `yawControl` server. Doing `roscd setyaw` and then running the Action Client will let you control the heading of the vehicle inside the simulator.
`poseEstimator.launch` | Launches the simulator and the Position Estimating Server. Doing `rostopic list` and then `rostopic echo`-ing the topic that publishes the position will let you see the position of the vehicle in the simulator.

