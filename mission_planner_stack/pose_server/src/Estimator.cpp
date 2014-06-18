#include <pose_server/Estimator.h>

namespace kraken_core
{
  //const int Estimator::_time = 0.05;
  
  Estimator::Estimator(int size,float time):_prev_states_body(size),_prev_states_world(size),_time(time)
  {
    for(int i=0;i<size;i++)
    {
      _prev_states_body.push_back(_next_pose_body);

      _prev_states_world.push_back(_next_pose_world);

    }

  }
  
  void  Estimator::updateTransformMatrix()
  {
    float _cos_rpy[3];
    float _sin_rpy[3];
    boost::circular_buffer<KrakenPose>::iterator _start = _prev_states_body.begin();
    float *_data = (*_start).getData();
    
    _cos_rpy[0] = cos(_data[_roll]);
    _sin_rpy[0] = sin(_data[_roll]);
    _cos_rpy[1] = cos(_data[_pitch]);
    _sin_rpy[1] = sin(_data[_pitch]);
    _cos_rpy[2] = cos(_data[_yaw]);
    _sin_rpy[2] = sin(_data[_yaw]);
    
    _body_to_world_matrix[0][0]   = _cos_rpy[2]*_cos_rpy[1];
    _body_to_world_matrix[0][1]   = _cos_rpy[2]*_sin_rpy[1]*_sin_rpy[1]-_sin_rpy[2]*_cos_rpy[0];
    _body_to_world_matrix[0][2]   = _cos_rpy[2]*_sin_rpy[1]*_cos_rpy[0]+_sin_rpy[2]*_sin_rpy[0];
    _body_to_world_matrix[1][0]   = _sin_rpy[2]*_cos_rpy[1];
    _body_to_world_matrix[1][1]   = _sin_rpy[0]*_sin_rpy[1]*_sin_rpy[2]+_cos_rpy[2]*_cos_rpy[0];
    _body_to_world_matrix[1][2]   = _cos_rpy[0]*_sin_rpy[1]*_sin_rpy[2]-_cos_rpy[2]*_sin_rpy[0];
    _body_to_world_matrix[2][0]   = -_sin_rpy[1];
    _body_to_world_matrix[2][1]   = _sin_rpy[0]*_cos_rpy[1];
    _body_to_world_matrix[2][2]   = _cos_rpy[0]*_cos_rpy[1];
    
  }
  
void Estimator::transformToWorld()
  {
    float *_data_new = _next_pose_body.getData();
    float *_data_next = _next_pose_world.getData();
    updateTransformMatrix();
    _data_next[_vx] = _body_to_world_matrix[0][0]*_data_new[_vx]+
                      _body_to_world_matrix[0][1]*_data_new[_vy]+
                      _body_to_world_matrix[0][2]*_data_new[_vz];
    _data_next[_vy] = _body_to_world_matrix[1][0]*_data_new[_vx]+
                      _body_to_world_matrix[1][1]*_data_new[_vy]+
                      _body_to_world_matrix[1][2]*_data_new[_vz];
    _data_next[_vz] = _body_to_world_matrix[1][0]*_data_new[_vx]+
                      _body_to_world_matrix[2][1]*_data_new[_vy]+
                      _body_to_world_matrix[2][2]*_data_new[_vz];
  }
  
}
