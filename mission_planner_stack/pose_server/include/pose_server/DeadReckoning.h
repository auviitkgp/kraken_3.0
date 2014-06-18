#ifndef DEADRECKONING_H
#define DEADRECKONING_H
#include <pose_server/Estimator.h>
#include <tracks_imu/Tracks.h>
#include <NqDVL/NqDVL.h>

namespace kraken_core
{
  class DeadReckoning : public Estimator
  {
    public:
    DeadReckoning(int size=2,float time=0.05);
    virtual ~DeadReckoning(){}
    /*
     * Function to update pose using only IMU data
     */
    virtual void updatePose(kraken_msgs::imuData &);
    /*
     * Function to update pose using only IMU and depth data
     */
    virtual void updatePose(kraken_msgs::imuData &,kraken_msgs::depthData &);
    /*
     * Function to update pose using DVL, depth and IMU data
     */
    virtual void updatePose(kraken_msgs::imuData &,kraken_msgs::depthData &, kraken_msgs::dvlData &);
    /*
     * Function to reset position
     */
    virtual void resetPose(KrakenPose &);
    private:
    /*
     * Function to update current position with depth data
     */
    void updateCurrentPosition(kraken_msgs::depthData &);
    /*
     * Function to update current position without depth data
     */
    void updateCurrentPosition();
    /*
     * Function to update current velocity 
     */
    void updateCurrentVelocity();
    void updateCurrentVelocity(kraken_msgs::dvlData & dvl_data);

    /*
     * Function to update current accelaration
     */

    void updateCurrentAccelaration(kraken_msgs::imuData &);
    protected:
  };
}

#endif // DEADRECKONING_H
