#include <controller_basic/StateController.h>

namespace kraken_controller
{
  StateController::StateController()
  {
    
  }
  
  StateController::~StateController()
  {
    
  }
  
  void StateController::resetController(float **gain, float *offset)
  {
    _gain = gain;
    _offset = offset;    
  }
}
