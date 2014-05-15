#ifndef KRAKENPOSE_H
#define KRAKENPOSE_H

namespace kraken_core
{
  static const int countState = 15;
  enum _state
  {
    _px=0,
    _vx,
    _ax,
    
    _py,
    _vy,
    _ay,
    
    _pz,
    _vz,
    _az,
    
    _roll,
    _pitch,
    _yaw
  };
  class KrakenPose
  {
    private:
      float _pose[countState];
    public:
      KrakenPose()
      {
        for(int i=0;i<countState;i++)
        {
            _pose[i]=0; 
        }
      }
      inline float* getData()
      {
        return &_pose[0];
      }
  };
  
} // end kraken_core

#endif // KRAKENPOSE_H
