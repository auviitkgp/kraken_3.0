#ifndef AUVCONTROLLER_H
#define AUVCONTROLLER_H
#include <controller_basic/StateController.h>

namespace kraken_controller
{
  
  class AuvController: public StateController
  {
    public:
      AuvController();
      virtual ~AuvController();
      void updateState();
      void doControlIteration(const kraken_msgs::krakenPose feedback);
      void setSetPoint(const kraken_msgs::krakenPose &setpoint);
      void moveForward();
      void moveBack();
      void pause();
    private:
      
    protected:
  };
}


#endif // AUVCONTROLLER_H
