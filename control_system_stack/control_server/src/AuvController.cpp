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
      std::cerr<<"Pause"<<std::endl;
      ControlParameters *param = _control_parameters[_control_parameters_index["pause"]];
      resetController(param->getGain(), param->getOffset());
  }
  
  void AuvController::moveBack()
  {
      std::cerr<<"Moving back"<<std::endl;
      ControlParameters *param = _control_parameters[_control_parameters_index["moveback"]];
      resetController(param->getGain(), param->getOffset());
  }
  
  void AuvController::moveForward()
  {
      std::cerr<<"Moving forward"<<std::endl;
      ControlParameters *param = _control_parameters[_control_parameters_index["moveforward"]];
      resetController(param->getGain(), param->getOffset());
  }
  
  void AuvController::setSetPoint(const kraken_msgs::krakenPose &setpoint)
  {
      _setPoint[0] = setpoint.data[kraken_core::_px];
      _setPoint[1] = setpoint.data[kraken_core::_py];
      _setPoint[2] = setpoint.data[kraken_core::_pz];

      _setPoint[kraken_core::_px+3] = setpoint.data[kraken_core::_px];
      _setPoint[kraken_core::_py+3] = setpoint.data[kraken_core::_py];
      _setPoint[kraken_core::_pz+3] = setpoint.data[kraken_core::_pz];

      _setPoint[kraken_core::_vx+3] = setpoint.data[kraken_core::_vx];
      _setPoint[kraken_core::_vy+3] = setpoint.data[kraken_core::_vy];
      _setPoint[kraken_core::_vz+3] = setpoint.data[kraken_core::_vz];

      _setPoint[kraken_core::_ax+3] = setpoint.data[kraken_core::_ax];
      _setPoint[kraken_core::_ay+3] = setpoint.data[kraken_core::_ay];
      _setPoint[kraken_core::_az+3] = setpoint.data[kraken_core::_az];


      _setPoint[kraken_core::_roll+3] = setpoint.data[kraken_core::_roll];
      _setPoint[kraken_core::_pitch+3] = setpoint.data[kraken_core::_pitch];
      _setPoint[kraken_core::_yaw+3] = setpoint.data[kraken_core::_yaw];

      _setPoint[kraken_core::_roll+6] = setpoint.data[kraken_core::_roll];
      _setPoint[kraken_core::_pitch+6] = setpoint.data[kraken_core::_pitch];
      _setPoint[kraken_core::_yaw+6] = setpoint.data[kraken_core::_yaw];

      _setPoint[kraken_core::_w_roll+6] = setpoint.data[kraken_core::_w_roll];
      _setPoint[kraken_core::_w_pitch+6] = setpoint.data[kraken_core::_w_pitch];
      _setPoint[kraken_core::_w_yaw+6] = setpoint.data[kraken_core::_w_yaw];
  }
  
  void AuvController::doControlIteration(const kraken_msgs::krakenPose feedback)
  {
    _feedback = feedback;
    for(int row = 0;row < 6; row++)
    {
        _thruster_data6.data[row] =  _offset[row];

        for(int col = 0; col<21; col++)
        {

            _thruster_data6.data[row] +=  _gain[row][col] * _error[col];
        }
    }

    for(int row = 0;row < 4; row++)
    {
        _thruster_data4.data[row] =  _thruster_data6.data[row];

    }
  }
  
  void AuvController::updateState()
  {
      _error[0] = _setPoint[0];// - _feedback.data[0];
      _error[1] = _setPoint[1];// - _feedback.data[1];
      _error[2] = _setPoint[2];// - _feedback.data[2];

      _error[kraken_core::_px+3] = _setPoint[kraken_core::_px+3] - _feedback.data[kraken_core::_px];
      _error[kraken_core::_py+3] = _setPoint[kraken_core::_py+3] - _feedback.data[kraken_core::_py];
      _error[kraken_core::_pz+3] = _setPoint[kraken_core::_pz+3] - _feedback.data[kraken_core::_pz];

      _error[kraken_core::_vx+3] = _setPoint[kraken_core::_vx+3] - _feedback.data[kraken_core::_vx];
      _error[kraken_core::_vy+3] = _setPoint[kraken_core::_vy+3] - _feedback.data[kraken_core::_vy];
      _error[kraken_core::_vz+3] = _setPoint[kraken_core::_vz+3] - _feedback.data[kraken_core::_vz];

      _error[kraken_core::_ax+3] = _setPoint[kraken_core::_ax+3] - _feedback.data[kraken_core::_ax];
      _error[kraken_core::_ay+3] = _setPoint[kraken_core::_ay+3] - _feedback.data[kraken_core::_ay];
      _error[kraken_core::_az+3] = _setPoint[kraken_core::_az+3] - _feedback.data[kraken_core::_az];


      _error[kraken_core::_roll+3] = _setPoint[kraken_core::_roll+3];// - _feedback.data[kraken_core::_roll];
      _error[kraken_core::_pitch+3] = _setPoint[kraken_core::_pitch+3];// - _feedback.data[kraken_core::_pitch];
      _error[kraken_core::_yaw+3] = _setPoint[kraken_core::_yaw+3];// - _feedback.data[kraken_core::_yaw];

      _error[kraken_core::_roll+6] = _setPoint[kraken_core::_roll+6] - _feedback.data[kraken_core::_roll];
      _error[kraken_core::_pitch+6] = _setPoint[kraken_core::_pitch+6] - _feedback.data[kraken_core::_pitch];
      _error[kraken_core::_yaw+6] = _setPoint[kraken_core::_yaw+6] - _feedback.data[kraken_core::_yaw];

      _error[kraken_core::_w_roll+6] = _setPoint[kraken_core::_w_roll+6] - _feedback.data[kraken_core::_w_roll];
      _error[kraken_core::_w_pitch+6] = _setPoint[kraken_core::_w_pitch+6] - _feedback.data[kraken_core::_w_pitch];
      _error[kraken_core::_w_yaw+6] = _setPoint[kraken_core::_w_yaw+6] - _feedback.data[kraken_core::_w_yaw];
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
