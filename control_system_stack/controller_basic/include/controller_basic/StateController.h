#ifndef STATECONTROLLER_H
#define STATECONTROLLER_H
#include <ros/ros.h>
#include <kraken_msgs/thrusterData4Thruster.h>
#include <kraken_msgs/thrusterData6Thruster.h>
#include <kraken_msgs/krakenPose.h>
#include <controller_basic/ControlParameters.h>

namespace kraken_controller
{
  class StateController
  {
    public:
      StateController();
      void resetController(float** gain,float* offset);
      virtual void updateState()=0;
      virtual void doControlIteration(const kraken_msgs::krakenPose feedback)=0;
      virtual void setSetPoint(const kraken_msgs::krakenPose &setpoint)=0;
      inline const kraken_msgs::thrusterData4Thruster& getThruster4Value()
      {
        return _thruster_data4;
      }

      inline const kraken_msgs::thrusterData6Thruster& getThruster6Value()
      {
        return _thruster_data6;
      }
      virtual void getState(kraken_msgs::krakenPose &state)=0;
      virtual ~ StateController();
    private:
    protected:
      float** _gain;
      float* _offset;
      float  _setPoint[21];
      float  _error[21];
      kraken_msgs::krakenPose _feedback;
      kraken_msgs::thrusterData4Thruster _thruster_data4;
      kraken_msgs::thrusterData6Thruster _thruster_data6;
      
  };
}

#endif // STATECONTROLLER_H
