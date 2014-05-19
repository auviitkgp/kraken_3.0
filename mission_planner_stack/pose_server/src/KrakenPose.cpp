#include <pose_server/KrakenPose.h>


namespace kraken_core
{
    
  void KrakenPose::write(std::ostream &out)
  {
    out<<"_px : " <<_pose[_px]<<std::endl;
    out<<"_py : " <<_pose[_py]<<std::endl;
    out<<"_pz : " <<_pose[_pz]<<std::endl;
    out<<"_vx : " <<_pose[_vx]<<std::endl;
    out<<"_vy : " <<_pose[_vy]<<std::endl;
    out<<"_vz : " <<_pose[_vz]<<std::endl;
    out<<"_ax : " <<_pose[_ax]<<std::endl;
    out<<"_ay : " <<_pose[_ay]<<std::endl;
    out<<"_az : " <<_pose[_az]<<std::endl;
  }
}
