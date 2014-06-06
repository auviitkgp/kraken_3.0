#include <control_server/AuvController.h>

namespace kraken_controller
{
  AuvController::AuvController():StateController()
  {
  }
  
  AuvController::~AuvController()
  {
    
  }
  
  void AuvController::pause()
  {
    
  }
  
  void AuvController::moveBack()
  {
    
  }
  
  void AuvController::moveForward()
  {
    
  }
  
  void AuvController::setSetPoint(const kraken_msgs::krakenPose &setpoint)
  {
    
  }
  
  void AuvController::doControlIteration(const kraken_msgs::krakenPose feedback)
  {
    
  }
  
  void AuvController::updateState()
  {
    
  }
  
  void AuvController::loadParams(const std::vector<std::string> &filenames)
  {
    for(int i=0;i<filenames.size();i++)
    {
        ControlParameters *param = new ControlParameters();
        param->load(filenames[i]);
      _control_parameters.push_back(param);
      _control_parameters_index[param->getName()] = i;
      param->write(std::cerr);
    }
  }
  
}
