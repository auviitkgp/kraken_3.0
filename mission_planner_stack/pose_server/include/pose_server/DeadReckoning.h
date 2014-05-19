#ifndef DEADRECKONING_H
#define DEADRECKONING_H
#include <pose_server/Estimator.h>
#include <imu/Tracks.h>

namespace kraken_core
{
  class DeadReckoning : public Estimator
  {
    public:
    DeadReckoning(int size=2,float time=0.05);
    virtual ~DeadReckoning(){}
    virtual void updatePose(kraken_msgs::imuData &);
    virtual void updatePose(kraken_msgs::imuData &,kraken_msgs::depthData &);
    virtual void updatePose(kraken_msgs::imuData &,kraken_msgs::depthData &, kraken_msgs::dvlData &);
    virtual void resetPose(KrakenPose &);
    private:
    void updateCurrentPosition(kraken_msgs::depthData &);
    void updateCurrentPosition();
    void updateCurrentVelocity();
    void updateCurrentAccelaration(kraken_msgs::imuData &);
    protected:
  };
}

#endif // DEADRECKONING_H
