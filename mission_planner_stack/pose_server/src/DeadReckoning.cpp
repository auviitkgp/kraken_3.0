#include <pose_server/DeadReckoning.h>


namespace kraken_core
{
  DeadReckoning::DeadReckoning(int size, float time):Estimator(size,time)
  {
    
  }
  
  void DeadReckoning::updatePose(kraken_msgs::imuData &imu)
  {
    updateCurrentAccelaration(imu);
    updateCurrentVelocity();
    updateCurrentPosition();
  }
  
  void DeadReckoning::updatePose(kraken_msgs::imuData &imu, kraken_msgs::depthData &depth)
  {
    updateCurrentAccelaration(imu);
    updateCurrentVelocity();
    updateCurrentPosition(depth);
  }
  
  void DeadReckoning::updatePose(kraken_msgs::imuData &imu, kraken_msgs::depthData &depth, kraken_msgs::dvlData &dvl)
  {
    updateCurrentAccelaration(imu);
    updateCurrentVelocity(dvl);
    updateCurrentPosition(depth);
  }
  
  void DeadReckoning::resetPose(KrakenPose & pose)
  {
    // Resets only 6 state variable of auv
    boost::circular_buffer<KrakenPose>::iterator start = _prev_states_world.end()-1;
    float* _data = (*start).getData();
    float* _pos = pose.getData();
    _data[_px] = _pos[_px];
    _data[_py] = _pos[_py];
    _data[_pz] = _pos[_pz];
    _data[_roll] = _pos[_roll];
    _data[_pitch] = _pos[_pitch];
    _data[_yaw] = _pos[_yaw];
  }
  
  void DeadReckoning::updateCurrentAccelaration(kraken_msgs::imuData & imu)
  {
    float* _data_body_next  = _next_pose_body.getData();
    float* _data_world_next = _next_pose_world.getData();
    // Update body accelaration buffer
    _data_body_next[_ax] = imu.data[kraken_sensors::accelX];
    _data_body_next[_ay] = imu.data[kraken_sensors::accelY];
    _data_body_next[_az] = imu.data[kraken_sensors::accelZ];
    // Update body angular velocity buffer
    _data_world_next[_w_roll]   = _data_body_next[_w_roll]  = imu.data[kraken_sensors::gyroX];
    _data_world_next[_w_pitch]  = _data_body_next[_w_pitch] = imu.data[kraken_sensors::gyroY];
    _data_world_next[_w_yaw]    = _data_body_next[_w_yaw]   = imu.data[kraken_sensors::gyroZ];
    // Update body angular position buffer
    _data_world_next[_roll]   = _data_body_next[_roll]  = imu.data[kraken_sensors::roll];
    _data_world_next[_pitch]  = _data_body_next[_pitch] = imu.data[kraken_sensors::pitch];
    _data_world_next[_yaw]    = _data_body_next[_yaw]   = imu.data[kraken_sensors::yaw];
  }
  
  void DeadReckoning::updateCurrentVelocity()
  {
    boost::circular_buffer<KrakenPose>::iterator start = _prev_states_body.end()-1;
    float* _data_body_next  = _next_pose_body.getData();
    float* _data = (*start).getData();
    // Calculate body velocities
    _data_body_next[_vx] = _data[_vx]+_data[_ax]*_time;
    _data_body_next[_vy] = _data[_vy]+_data[_ay]*_time;
    _data_body_next[_vz] = _data[_vz]+_data[_az]*_time;
    // Transform to world velocities
    transformToWorld();
    //
    float* _data_world_next  = _next_pose_world.getData();
    start = _prev_states_world.end()-1;
    _data = (*start).getData();
    // Calculate world accelaration
    _data_world_next[_ax] = (_data_world_next[_vx] - _data[_vx])/_time;
    _data_world_next[_ay] = (_data_world_next[_vy] - _data[_vy])/_time;
    _data_world_next[_az] = (_data_world_next[_vz] - _data[_vz])/_time;
  }

  void DeadReckoning::updateCurrentVelocity(kraken_msgs::dvlData &dvl_data)
  {
      boost::circular_buffer<KrakenPose>::iterator start = _prev_states_body.end()-1;
      float* _data_body_next  = _next_pose_body.getData();
      float* _data = (*start).getData();
      // Calculate body velocities
      _data_body_next[_vx] = dvl_data.data[kraken_sensors::_dvl_vx];
      _data_body_next[_vy] = dvl_data.data[kraken_sensors::_dvl_vy];
      _data_body_next[_vz] = dvl_data.data[kraken_sensors::_dvl_vy];
      // Transform to world velocities
      transformToWorld();
      //
      float* _data_world_next  = _next_pose_world.getData();
      start = _prev_states_world.end()-1;
      _data = (*start).getData();
      // Calculate world accelaration
      _data_world_next[_ax] = (_data_world_next[_vx] - _data[_vx])/_time;
      _data_world_next[_ay] = (_data_world_next[_vy] - _data[_vy])/_time;
      _data_world_next[_az] = (_data_world_next[_vz] - _data[_vz])/_time;
  }
  
  void DeadReckoning::updateCurrentPosition(kraken_msgs::depthData & depth)
  {
    float* _data_world_next  = _next_pose_world.getData();
    boost::circular_buffer<KrakenPose>::iterator start = _prev_states_world.end()-1;
    float* _data = (*start).getData();
    // Calculate world positions
    _data_world_next[_px] = _data[_px]+(_data[_vx]+_data[_ax]*_time/2.0)*_time;
    _data_world_next[_py] = _data[_py]+(_data[_vy]+_data[_ay]*_time/2.0)*_time;
    _data_world_next[_pz] = depth.depth;
    _prev_states_world.push_back(_next_pose_world);
    //
    float* _data_body_next  = _next_pose_body.getData();
    start = _prev_states_body.end()-1;
    _data = (*start).getData();
    // Calculate body positions
    _data_body_next[_px] = _data[_px]+(_data[_vx]+_data[_ax]*_time/2.0)*_time;
    _data_body_next[_py] = _data[_py]+(_data[_vy]+_data[_ay]*_time/2.0)*_time;
    _data_body_next[_pz] = depth.depth;
    _prev_states_body.push_back(_next_pose_body);
  }
  
  void DeadReckoning::updateCurrentPosition()
  {
    float* _data_world_next  = _next_pose_world.getData();
    boost::circular_buffer<KrakenPose>::iterator start = _prev_states_world.end()-1;
    float* _data = (*start).getData();
    // Calculate world positions
    _data_world_next[_px] = _data[_px]+(_data[_vx]+_data[_ax]*_time/2.0)*_time;
    _data_world_next[_py] = _data[_py]+(_data[_vy]+_data[_ay]*_time/2.0)*_time;
    _data_world_next[_pz] = _data[_pz]+(_data[_vz]+_data[_az]*_time/2.0)*_time;
    _prev_states_world.push_back(_next_pose_world);
    //
    float* _data_body_next  = _next_pose_body.getData();
    start = _prev_states_body.end()-1;
     _data = (*start).getData();
     // Calculate body positions
    _data_body_next[_px] = _data[_px]+(_data[_vx]+_data[_ax]*_time/2.0)*_time;
    _data_body_next[_py] = _data[_py]+(_data[_vy]+_data[_ay]*_time/2.0)*_time;
    _data_body_next[_pz] = _data[_pz]+(_data[_vz]+_data[_az]*_time/2.0)*_time;
    _prev_states_body.push_back(_next_pose_body);
  }
  
}
