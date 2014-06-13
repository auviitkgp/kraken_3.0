Telemetry
==========

This package is used to visualize full vehicle state like battery status, thruster status and all.

Some workaround
===============

1. To use ros msg in Q_SLOT function you have to first declare this outside your class defination
    Q_DECLARE_METATYPE(sensor_msgs::ImageConstPtr);
2. Now inside constructor before connecting the slots you have to call this functions
    qRegisterMetaType<sensor_msgs::ImageConstPtr>("ImageConstPtr");

Description
===========
