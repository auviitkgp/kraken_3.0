BUOY_DETECT
-----------
This repository contains the codes that uses ML (Machine Learning) techniques to detect the buoy. This is a combination of 3 parts - 
<br>
<br>(1) `Pixel-trainer` : A ML-trainer module.
<br>(2) `vw_detect` (`vw` = `vowpal wabbit`) : This module detects the buoy boundary with the help of ML, by taking the training data output of `Pixel-trainer`.
<br>(3) `Region-blower` : This module blows / colors the region within the buoy boundary detected by `vw_detect`.

RUNNING THE SCRIPTS
-------------------

// include the steps with further commits. 
// doubt : steps for working with ROS or without ROS?

CONTRIBUTION
------------
The work flow is the same as that of any other repository. 
<br> (1) Fork / clone the repository.
<br> (2) Create a new branch , say `my-changes` and make your changes in this branch.
<br> (3) Commit your changes and send a Pull request (PR) to this repository.

LICENSE
-------
[The MIT License](LICENSE.md) - [Team AUV , IIT Kharagpur](https://github.com/auviitkgp) - All Rights Reserved.
