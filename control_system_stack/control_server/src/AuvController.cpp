#include <control_server/AuvController.h>

namespace kraken_controller
{
  AuvController::AuvController():StateController()
  {
      for(int i=0;i<21;i++)
      {
          _setPoint[i]=0;
          _error[i]=0;
      }
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
  
  void AuvController::moveTest()
  {
      std::cerr<<"Test Movement"<<std::endl;
      ControlParameters *param = _control_parameters[_control_parameters_index["test"]];
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

    for(int i=0;i<kraken_core::countState;i++)
    {
      _feedback.data[i] = feedback.data[i];
    }

    /*float error[kraken_core::_yaw +1 ];
    int multiplier[6] = {1,1,1,1,1,1};

    //new part
    error[kraken_core::_px] = _setPoint[kraken_core::_px] - feedback.data[kraken_core::_px];
    if(fabs(error[kraken_core::_px]) > 0.05) {multiplier[0] = multiplier[1] = (error[kraken_core::_px])/fabs(error[kraken_core::_px]);}
    else { multiplier[0] = multiplier[1] = 0;}

    error[kraken_core::_py] = _setPoint[kraken_core::_py] - feedback.data[kraken_core::_py];
    if(fabs(error[kraken_core::_py]) > 0.05) {multiplier[4] = multiplier[5] = (error[kraken_core::_py])/fabs(error[kraken_core::_py]);}
    else { multiplier[4] = multiplier[5] = 0;}

    error[kraken_core::_pz] = _setPoint[kraken_core::_pz] - feedback.data[kraken_core::_pz];
    if(fabs(error[kraken_core::_pz]) > 0.05) {multiplier[2] = multiplier[3] = (error[kraken_core::_pz])/fabs(error[kraken_core::_pz]);}
    else { multiplier[2] = multiplier[3] = 0;}

    error[kraken_core::_roll] = _setPoint[kraken_core::_roll] - feedback.data[kraken_core::_roll];
    error[kraken_core::_pitch] = _setPoint[kraken_core::_pitch] - feedback.data[kraken_core::_pitch];
    error[kraken_core::_yaw] = _setPoint[kraken_core::_yaw] - feedback.data[kraken_core::_yaw];*/
    //end of new part

    for(int row = 0;row < 6; row++)
    {
        _thruster_data6.data[row] =  _offset[row];// * multiplier[row];

        for(int col = 0; col<21; col++)
        {

            _thruster_data6.data[row] +=  _gain[row][col] * _error[col];

        }
    }
    _thruster_data4.data[0] =  _thruster_data6.data[0];
    _thruster_data4.data[1] =  _thruster_data6.data[1];
    _thruster_data4.data[2] =  _thruster_data6.data[4];
    _thruster_data4.data[3] =  _thruster_data6.data[5];
  }
  

  void AuvController::updateIPState()
  {

      _error[0] = _setPoint[0];// - _feedback.data[0];
      _error[1] = _setPoint[1];// - _feedback.data[1];
      _error[2] = _setPoint[2];// - _feedback.data[2];

      _error[kraken_core::_px+3] = _feedback.data[kraken_core::_px];
      _error[kraken_core::_py+3] = _feedback.data[kraken_core::_py];
      _error[kraken_core::_pz+3] = _feedback.data[kraken_core::_pz];

      _error[kraken_core::_vx+3] = _feedback.data[kraken_core::_vx];
      _error[kraken_core::_vy+3] = _feedback.data[kraken_core::_vy];
      _error[kraken_core::_vz+3] = _feedback.data[kraken_core::_vz];

      _error[kraken_core::_ax+3] = _feedback.data[kraken_core::_ax];
      _error[kraken_core::_ay+3] = _feedback.data[kraken_core::_ay];
      _error[kraken_core::_az+3] = _feedback.data[kraken_core::_az];

      _error[kraken_core::_roll+3] = _setPoint[kraken_core::_roll+3];// - _feedback.data[kraken_core::_roll];
      _error[kraken_core::_pitch+3] = _setPoint[kraken_core::_pitch+3];// - _feedback.data[kraken_core::_pitch];
      _error[kraken_core::_yaw+3] = _setPoint[kraken_core::_yaw+3];// - _feedback.data[kraken_core::_yaw];

      _error[kraken_core::_roll+6] =  _feedback.data[kraken_core::_roll];
      _error[kraken_core::_pitch+6] = _feedback.data[kraken_core::_pitch];
      _error[kraken_core::_yaw+6] = _feedback.data[kraken_core::_yaw];

      _error[kraken_core::_w_roll+6] =  _feedback.data[kraken_core::_w_roll];
      _error[kraken_core::_w_pitch+6] = _feedback.data[kraken_core::_w_pitch];
      _error[kraken_core::_w_yaw+6] =   _feedback.data[kraken_core::_w_yaw];
  }

  void AuvController::updateState()
  {
      float _cos_rpy[3];
      float _sin_rpy[3];


      _cos_rpy[0]   = cos(_feedback.data[kraken_core::_roll]);
      _sin_rpy[0] = sin(_feedback.data[kraken_core::_roll]);

      _cos_rpy[1]   = cos(_feedback.data[kraken_core::_pitch]);
      _sin_rpy[1] = sin(_feedback.data[kraken_core::_pitch]);

      _cos_rpy[2]   = cos(_feedback.data[kraken_core::_yaw]);
      _sin_rpy[2] = sin(_feedback.data[kraken_core::_yaw]);


      //////////////////////////////////////////////////////////////
      float _body_to_world_matrix[3][3];
      _body_to_world_matrix[0][0]   = _cos_rpy[2]*_cos_rpy[1];
      _body_to_world_matrix[1][0]   = _cos_rpy[2]*_sin_rpy[1]*_sin_rpy[0]-_sin_rpy[2]*_cos_rpy[0];
      _body_to_world_matrix[2][0]   = _cos_rpy[2]*_sin_rpy[1]*_cos_rpy[0]+_sin_rpy[2]*_sin_rpy[0];
      _body_to_world_matrix[0][1]   = _sin_rpy[2]*_cos_rpy[1];
      _body_to_world_matrix[1][1]   = _sin_rpy[0]*_sin_rpy[1]*_sin_rpy[2]+_cos_rpy[2]*_cos_rpy[0];
      _body_to_world_matrix[2][1]   = _cos_rpy[0]*_sin_rpy[1]*_sin_rpy[2]-_cos_rpy[2]*_sin_rpy[0];
      _body_to_world_matrix[0][2]   = -_sin_rpy[1];
      _body_to_world_matrix[1][2]   = _sin_rpy[0]*_cos_rpy[1];
      _body_to_world_matrix[2][2]   = _cos_rpy[0]*_cos_rpy[1];


      float _error1[21];
      _error1[0] = _setPoint[0];// - _feedback.data[0];
      _error1[1] = _setPoint[1];// - _feedback.data[1];
      _error1[2] = _setPoint[2];// - _feedback.data[2];

      _error1[kraken_core::_px+3] = _setPoint[kraken_core::_px+3] - _feedback.data[kraken_core::_px];
      _error1[kraken_core::_py+3] = _setPoint[kraken_core::_py+3] - _feedback.data[kraken_core::_py];
      _error1[kraken_core::_pz+3] = _setPoint[kraken_core::_pz+3] - _feedback.data[kraken_core::_pz];

      _error1[kraken_core::_vx+3] = _setPoint[kraken_core::_vx+3] - _feedback.data[kraken_core::_vx];
      _error1[kraken_core::_vy+3] = _setPoint[kraken_core::_vy+3] - _feedback.data[kraken_core::_vy];
      _error1[kraken_core::_vz+3] = _setPoint[kraken_core::_vz+3] - _feedback.data[kraken_core::_vz];

      _error1[kraken_core::_ax+3] = _setPoint[kraken_core::_ax+3] - _feedback.data[kraken_core::_ax];
      _error1[kraken_core::_ay+3] = _setPoint[kraken_core::_ay+3] - _feedback.data[kraken_core::_ay];
      _error1[kraken_core::_az+3] = _setPoint[kraken_core::_az+3] - _feedback.data[kraken_core::_az];

      multiply(_body_to_world_matrix,&_error1[kraken_core::_px+3],&_error[kraken_core::_px+3]);
      multiply(_body_to_world_matrix,&_error1[kraken_core::_vx+3],&_error[kraken_core::_vx+3]);
      multiply(_body_to_world_matrix,&_error1[kraken_core::_ax+3],&_error[kraken_core::_ax+3]);


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
    for(unsigned int i=0;i<filenames.size();i++)
    {
        ControlParameters *param = new ControlParameters();
        param->load(filenames[i]);
      _control_parameters.push_back(param);
      _control_parameters_index[param->getName()] = i;
      param->write(std::cerr);
    }
  }

  bool AuvController::checkError(const kraken_msgs::krakenPose &msg)
  {
      float curState[kraken_core::countState + 1];
      float error[kraken_core::countState + 1];
      float thresh[kraken_core::countState + 1];

      bool result;

      thresh[kraken_core::_px] = 0.05;
      thresh[kraken_core::_py] = 0.05;
      thresh[kraken_core::_pz] = 0.05;
      thresh[kraken_core::_roll] = 0.05;
      thresh[kraken_core::_pitch] = 0.05;
      thresh[kraken_core::_yaw] = 0.05;

      curState[kraken_core::_px] = msg.data[kraken_core::_px];
      curState[kraken_core::_py] = msg.data[kraken_core::_py];
      curState[kraken_core::_pz] = msg.data[kraken_core::_pz];
      curState[kraken_core::_roll] = msg.data[kraken_core::_roll];
      curState[kraken_core::_pitch] = msg.data[kraken_core::_pitch];
      curState[kraken_core::_yaw] = msg.data[kraken_core::_yaw];


      error[kraken_core::_px] = fabs(curState[kraken_core::_px] - _setPoint[kraken_core::_px+3]);
      error[kraken_core::_py] = fabs(curState[kraken_core::_py] - _setPoint[kraken_core::_py+3]);
      error[kraken_core::_pz] = fabs(curState[kraken_core::_pz] - _setPoint[kraken_core::_pz+3]);
      error[kraken_core::_roll] = fabs(curState[kraken_core::_roll] - _setPoint[kraken_core::_roll+6]);
      error[kraken_core::_pitch] = fabs(curState[kraken_core::_pitch] - _setPoint[kraken_core::_pitch+6]);
      error[kraken_core::_yaw] = fabs(curState[kraken_core::_yaw] - _setPoint[kraken_core::_yaw+6]);

      result =((error[kraken_core::_px] < thresh[kraken_core::_px ]) && (error[kraken_core::_py] < thresh[kraken_core::_py]) && (error[kraken_core::_pz] < thresh[kraken_core::_pz]) && (error[kraken_core::_roll] < thresh[kraken_core::_roll ]) && (error[kraken_core::_pitch] < thresh[kraken_core::_pitch]) && (error[kraken_core::_yaw] < thresh[kraken_core::_yaw ]));

      /*std::cout<<"error 1 : "<< (error[kraken_core::_px])/fabs(error[kraken_core::_px]) <<std::endl;
      std::cout<<"error 2 : "<< (error[kraken_core::_py])/fabs(error[kraken_core::_py]) <<std::endl;
      std::cout<<"error 3 : "<< (error[kraken_core::_pz])/fabs(error[kraken_core::_pz]) <<std::endl;
      std::cout<<"error 4 : "<< (error[kraken_core::_roll])/fabs(error[kraken_core::_yaw]) <<std::endl;
      std::cout<<"error 5 : "<< (error[kraken_core::_pitch])/fabs(error[kraken_core::_pitch]) <<std::endl;
      std::cout<<"error 6 : "<< (error[kraken_core::_yaw])/fabs(error[kraken_core::_yaw]) <<std::endl;*/

      //std::cout<<" "<<curState[kraken_core::_yaw]<<" : "<<_setPoint[kraken_core::_yaw+6]<<" : "<<error[kraken_core::_yaw] <<std::endl;
      //std::cout<<"result : "<<result<<std::endl;
      //std::cout<<"kd : "<<_gain[0][5]<<std::endl;
      return result;
  }

  void AuvController::getState(kraken_msgs::krakenPose &state)
  {
      state.data[kraken_core::_px] = _setPoint[kraken_core::_px+3];
      state.data[kraken_core::_py] = _setPoint[kraken_core::_py+3];
      state.data[kraken_core::_pz] = _setPoint[kraken_core::_pz+3];

      state.data[kraken_core::_vx] = _setPoint[kraken_core::_vx+3];
      state.data[kraken_core::_vy] = _setPoint[kraken_core::_vy+3];
      state.data[kraken_core::_vz] = _setPoint[kraken_core::_vz+3];

      state.data[kraken_core::_ax] = _setPoint[kraken_core::_ax+3];
      state.data[kraken_core::_ay] =  _setPoint[kraken_core::_ay+3];
      state.data[kraken_core::_az] = _setPoint[kraken_core::_az+3];

      state.data[kraken_core::_roll] = _setPoint[kraken_core::_roll+6];
      state.data[kraken_core::_pitch] = _setPoint[kraken_core::_pitch+6];
      state.data[kraken_core::_yaw] = _setPoint[kraken_core::_yaw+6];

      state.data[kraken_core::_w_roll] = _setPoint[kraken_core::_w_roll+6];
      state.data[kraken_core::_w_pitch] = _setPoint[kraken_core::_w_pitch+6];
      state.data[kraken_core::_w_yaw] = _setPoint[kraken_core::_w_yaw+6];
  }


  void AuvController::local2globalAll(kraken_msgs::krakenPose &local, kraken_msgs::krakenPose &global)
  {
      float _cos_rpy[3];
      float _sin_rpy[3];


      _cos_rpy[0]   = cos(local.data[kraken_core::_roll]);
      _sin_rpy[0]   = sin(local.data[kraken_core::_roll]);

      _cos_rpy[1]   = cos(local.data[kraken_core::_pitch]);
      _sin_rpy[1]   = sin(local.data[kraken_core::_pitch]);

      _cos_rpy[2]   = cos(local.data[kraken_core::_yaw]);
      _sin_rpy[2]   = sin(local.data[kraken_core::_yaw]);


      //////////////////////////////////////////////////////////////
      float _body_to_world_matrix[3][3];
      _body_to_world_matrix[0][0]   = _cos_rpy[2]*_cos_rpy[1];
      _body_to_world_matrix[0][1]   = _cos_rpy[2]*_sin_rpy[1]*_sin_rpy[0]-_sin_rpy[2]*_cos_rpy[0];
      _body_to_world_matrix[0][2]   = _cos_rpy[2]*_sin_rpy[1]*_cos_rpy[0]+_sin_rpy[2]*_sin_rpy[0];
      _body_to_world_matrix[1][0]   = _sin_rpy[2]*_cos_rpy[1];
      _body_to_world_matrix[1][1]   = _sin_rpy[0]*_sin_rpy[1]*_sin_rpy[2]+_cos_rpy[2]*_cos_rpy[0];
      _body_to_world_matrix[1][2]   = _cos_rpy[0]*_sin_rpy[1]*_sin_rpy[2]-_cos_rpy[2]*_sin_rpy[0];
      _body_to_world_matrix[2][0]   = -_sin_rpy[1];
      _body_to_world_matrix[2][1]   = _sin_rpy[0]*_cos_rpy[1];
      _body_to_world_matrix[2][2]   = _cos_rpy[0]*_cos_rpy[1];


      float _local[kraken_core::countState];
      std::cout<<"global x "<<global.data[kraken_core::_px]<<" : "<<global.data[kraken_core::_px]<<std::endl;
      std::cout<<"global y "<<global.data[kraken_core::_py]<<" : "<<global.data[kraken_core::_py]<<std::endl;
      std::cout<<"global z "<<global.data[kraken_core::_pz]<<" : "<<global.data[kraken_core::_pz]<<std::endl;
      _local[kraken_core::_px] = local.data[kraken_core::_px];// - _feedback.data[0];
      _local[kraken_core::_py] = local.data[kraken_core::_py];// - _feedback.data[1];
      _local[kraken_core::_pz] = local.data[kraken_core::_pz];// - _feedback.data[2];

      float _global[kraken_core::countState];
      multiply(_body_to_world_matrix,&_local[kraken_core::_px],&_global[kraken_core::_px]);
      multiply(_body_to_world_matrix,&_local[kraken_core::_vx+3],&_error[kraken_core::_vx+3]);
      multiply(_body_to_world_matrix,&_local[kraken_core::_ax+3],&_error[kraken_core::_ax+3]);

      global.data[kraken_core::_px] += _global[kraken_core::_px];
      global.data[kraken_core::_py] += _global[kraken_core::_py];
      global.data[kraken_core::_pz] += _global[kraken_core::_pz];

      global.data[kraken_core::_vx] += _global[kraken_core::_vx];
      global.data[kraken_core::_vy] += _global[kraken_core::_vy];
      global.data[kraken_core::_vz] += _global[kraken_core::_vz];

      global.data[kraken_core::_ax] += _global[kraken_core::_ax];
      global.data[kraken_core::_ay] += _global[kraken_core::_ay];
      global.data[kraken_core::_az] += _global[kraken_core::_az];


      std::cout<<"global x "<<_global[kraken_core::_px]<<" : "<<global.data[kraken_core::_px]<<std::endl;
      std::cout<<"global y "<<_global[kraken_core::_py]<<" : "<<global.data[kraken_core::_py]<<std::endl;
      std::cout<<"global z "<<_global[kraken_core::_pz]<<" : "<<global.data[kraken_core::_pz]<<std::endl;

      std::cout<<"global vx "<<_global[kraken_core::_vx]<<" : "<<global.data[kraken_core::_vx]<<std::endl;
      std::cout<<"global vy "<<_global[kraken_core::_vy]<<" : "<<global.data[kraken_core::_vy]<<std::endl;
      std::cout<<"global vz "<<_global[kraken_core::_vz]<<" : "<<global.data[kraken_core::_vz]<<std::endl;

      std::cout<<"global ax "<<_global[kraken_core::_ax]<<" : "<<global.data[kraken_core::_ax]<<std::endl;
      std::cout<<"global ay "<<_global[kraken_core::_ay]<<" : "<<global.data[kraken_core::_ay]<<std::endl;
      std::cout<<"global az "<<_global[kraken_core::_az]<<" : "<<global.data[kraken_core::_az]<<std::endl;


      global.data[kraken_core::_roll] = local.data[kraken_core::_roll];
      global.data[kraken_core::_pitch] = local.data[kraken_core::_pitch];
      global.data[kraken_core::_yaw] = local.data[kraken_core::_yaw];

  }

  void AuvController::local2global(kraken_msgs::krakenPose &local, kraken_msgs::krakenPose &global)
  {
      float _cos_rpy[3];
      float _sin_rpy[3];


      _cos_rpy[0]   = cos(local.data[kraken_core::_roll]);
      _sin_rpy[0]   = sin(local.data[kraken_core::_roll]);

      _cos_rpy[1]   = cos(local.data[kraken_core::_pitch]);
      _sin_rpy[1]   = sin(local.data[kraken_core::_pitch]);

      _cos_rpy[2]   = cos(local.data[kraken_core::_yaw]);
      _sin_rpy[2]   = sin(local.data[kraken_core::_yaw]);


      //////////////////////////////////////////////////////////////
      float _body_to_world_matrix[3][3];
      _body_to_world_matrix[0][0]   = _cos_rpy[2]*_cos_rpy[1];
      _body_to_world_matrix[0][1]   = _cos_rpy[2]*_sin_rpy[1]*_sin_rpy[0]-_sin_rpy[2]*_cos_rpy[0];
      _body_to_world_matrix[0][2]   = _cos_rpy[2]*_sin_rpy[1]*_cos_rpy[0]+_sin_rpy[2]*_sin_rpy[0];
      _body_to_world_matrix[1][0]   = _sin_rpy[2]*_cos_rpy[1];
      _body_to_world_matrix[1][1]   = _sin_rpy[0]*_sin_rpy[1]*_sin_rpy[2]+_cos_rpy[2]*_cos_rpy[0];
      _body_to_world_matrix[1][2]   = _cos_rpy[0]*_sin_rpy[1]*_sin_rpy[2]-_cos_rpy[2]*_sin_rpy[0];
      _body_to_world_matrix[2][0]   = -_sin_rpy[1];
      _body_to_world_matrix[2][1]   = _sin_rpy[0]*_cos_rpy[1];
      _body_to_world_matrix[2][2]   = _cos_rpy[0]*_cos_rpy[1];


      float _local[kraken_core::countState];
      std::cout<<"global x "<<global.data[kraken_core::_px]<<" : "<<global.data[kraken_core::_px]<<std::endl;
      std::cout<<"global y "<<global.data[kraken_core::_py]<<" : "<<global.data[kraken_core::_py]<<std::endl;
      std::cout<<"global z "<<global.data[kraken_core::_pz]<<" : "<<global.data[kraken_core::_pz]<<std::endl;
      _local[kraken_core::_px] = local.data[kraken_core::_px];// - _feedback.data[0];
      _local[kraken_core::_py] = local.data[kraken_core::_py];// - _feedback.data[1];
      _local[kraken_core::_pz] = local.data[kraken_core::_pz];// - _feedback.data[2];

      float _global[kraken_core::countState];
      multiply(_body_to_world_matrix,&_local[kraken_core::_px],&_global[kraken_core::_px]);
      //multiply(_body_to_world_matrix,&_local[kraken_core::_vx+3],&_error[kraken_core::_vx+3]);
      //multiply(_body_to_world_matrix,&_local[kraken_core::_ax+3],&_error[kraken_core::_ax+3]);

      global.data[kraken_core::_px] += _global[kraken_core::_px];
      global.data[kraken_core::_py] += _global[kraken_core::_py];
      global.data[kraken_core::_pz] += _global[kraken_core::_pz];
     /*
      global.data[kraken_core::_vx] += _global[kraken_core::_vx];
      global.data[kraken_core::_vy] += _global[kraken_core::_vy];
      global.data[kraken_core::_vz] += _global[kraken_core::_vz];
      global.data[kraken_core::_ax] += _global[kraken_core::_ax];
      global.data[kraken_core::_ay] += _global[kraken_core::_ay];
      global.data[kraken_core::_az] += _global[kraken_core::_az];
     */

      std::cout<<"global x "<<_global[kraken_core::_px]<<" : "<<global.data[kraken_core::_px]<<std::endl;
      std::cout<<"global y "<<_global[kraken_core::_py]<<" : "<<global.data[kraken_core::_py]<<std::endl;
      std::cout<<"global z "<<_global[kraken_core::_pz]<<" : "<<global.data[kraken_core::_pz]<<std::endl;
     /*
      std::cout<<"global x "<<_global[kraken_core::_vx]<<" : "<<global.data[kraken_core::_vx]<<std::endl;
      std::cout<<"global y "<<_global[kraken_core::_vy]<<" : "<<global.data[kraken_core::_vy]<<std::endl;
      std::cout<<"global z "<<_global[kraken_core::_vz]<<" : "<<global.data[kraken_core::_vz]<<std::endl;
      std::cout<<"global x "<<_global[kraken_core::_ax]<<" : "<<global.data[kraken_core::_ax]<<std::endl;
      std::cout<<"global y "<<_global[kraken_core::_ay]<<" : "<<global.data[kraken_core::_ay]<<std::endl;
      std::cout<<"global z "<<_global[kraken_core::_az]<<" : "<<global.data[kraken_core::_az]<<std::endl;
     */
      /*
      global.data[kraken_core::_roll] = local.data[kraken_core::_roll];
      global.data[kraken_core::_pitch] = local.data[kraken_core::_pitch];
      global.data[kraken_core::_yaw] = local.data[kraken_core::_yaw];
      */

  }

  void AuvController::multiply(float matrix[][3], float* src_vec, float* dst_vec)
  {
      for(int i=0; i<3; i++)
      {
          float val=0;
          for(int j =0; j<3; j++)
          {
              val+=matrix[i][j]*src_vec[j];
          }
          dst_vec[i] = val;
      }
  }

  
}

