Vision Stack
============
##Overall Architecture

With the use of actionlib, we hope to optimize the whole communication protocol between the main task planner and the vision module. When the task/mission planner, which is always running, throughout
the mission, invokes one of the tasks of the Vision stack, then, this is fixed up in such a way that, the IP code is present in an Action Server,
and the Task Planner acts as the Action Client. The client sends a goal to the server, and the server starts executing the goal. Throughout the
execution of the goal, the server sends feedback to the client. And at the successful completion of the goal or at the end of the timeout, set by
the client, the server sends a Final Result.

##Folders

###actionmsg/###

  TO DO

###blob/###

This folder contains code for the Blob detection, filtering and filling library.

###buoy/###

This will house code for the buoy detection task, of the RoboSub competition.

###camera/###

Due to a change in the overall architecture of procuring the images and then working on them, this folder has been added. Earlier, the code
for all the tasks individually opened the camera, whenever they required and procured the images directly from the camera, but this year,
the camera's will always be open. The only job of the cameras will be to take the images and then publish them on the topics `bottomcamimage`
and `frontcamimage`. All the tasks, which have been implemented as Action Servers, will subscribe to these two topics and work on the images
that they recieve from these two topics.

This module depends on `image_transport` and `cv_bridge`.

###task_marker/###

This folder houses code for the marker detection part. In this part of a task, we detect a marker and then send feedback to the task planner as to the status of completion and alignment changes that need to be made, in order to match the heading of the vehicle with the alignment of the marker, placed on the floor of the tank.

###validationgate/###

This module will house code for the validation gate task.

###vision_gui/###

Testing tool. This tool can accomplish thresholding, apply multiple filters such as Median, Gaussian and Simple Blur, Drawing contours using Hough Transformations and otherwise, blob detection and filling.

It then writes all the parameters currently being used to a file which may be parsed later to get the perfect parameters at runtime.

