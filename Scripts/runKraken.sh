#!/bin/bash

cd ~/hydro_workspace/sandbox

#Launcing simulator GUI. Depth will not work without it
rosrun kraken_isualisationkraken_launcher &

#Starting Pose Server.  Returns estimated position of vehicle
rosrun pose_server server &

#Starting model. Arguments are forces in 6 direction
rosrun simulator_physics_model start_auv_model 0.0 0.0 0.0 0.0 0.0 0.0 &

#######
#Model should be run only after Pose Server to avoid initial position error
#######

#Starting Force Converter. Argument is force converter
rosrun thruster_force_converter converter 3 &

#Starting Ccontrol Server. Arguments are frequency and files from which
#parameters are read
rosrun control_server controlServer 10 parameters/MoveBack.cp parameters/MoveForward.cp parameters/Pause.cp &

#Starting control server. Argument is height
rosrun control_server client_action 3.0

