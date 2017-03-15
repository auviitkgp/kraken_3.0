BUOY_DETECT
-----------
This repository contains the codes that uses ML (Machine Learning) techniques to detect the buoy. This is a combination of 3 parts - 


(1) `Pixel-trainer` : A ML-trainer module.


(2) `vw_detect` (`vw` = `vowpal wabbit`) : This module detects the buoy boundary with the help of ML, by taking the training data output of `Pixel-trainer`.


(3) `Region-blower` : This module blows / colors the region within the buoy boundary detected by `vw_detect`.


### System Requirements

- A full installation of ROS Indigo
- `rosbuild` workspace



RUNNING THE SCRIPTS
-------------------

Without ROS :-
------------------

### `Pixel-trainer` :

                    
- Clone the repository : [Pixel-trainer_vw](https://github.com/pranaypratyush/Pixel-Trainer/tree/vw) 
                    
- `cd` into the directory
                      
- Run `cmake .`
                      
- Now run `./main path/to/the/imagefile`
                      
- Labelling involves pressing a key and left clicking and dragging the mouse
                      
  - For labelling Yellow press Crtl key
                        
  - For labelling Green press Shift key
                          
  - For labelling Red press Alt key
                          
- After labelling,click on the broom icon to save the hsv values to a `.txt` file
                      
- Click the `save` icon to save the image.
  
  
### `vw_detect` :

- `cd` into `vw_detect`
  
- Run `cmake .`
  
- Run `./main path/to/the/imagefile`


### `region_growing`

- `cd` into `region_growing`

- Run `cmake .`

- Run `./main path/to/the/imagefile`

  
  

With ROS :-
------------

  - Clone the directory in your `ROS_WORKSPACE`
  - Do `roscd`
  - Then `cd buoy_detect`
  - `rosmake buoy_detect`
  - Open a new terminal window and execute `roscore`
  - In the original terminal window, run `rosrun buoy_detect [node_name]`, for example : 
    -   `rosrun buoy_detect vw_detect` 
    -   `rosrun buoy_detect region_growing`


CONTRIBUTION
------------
The work flow is the same as that of any other repository. 


(1) Fork / clone the repository.


(2) Create a new branch , say `my-changes` and make your changes in this branch.


(3) Commit your changes and send a Pull request (PR) to this repository.

LICENSE
-------
[The MIT License](LICENSE.md) - [Team AUV , IIT Kharagpur](https://github.com/auviitkgp) - All Rights Reserved.
