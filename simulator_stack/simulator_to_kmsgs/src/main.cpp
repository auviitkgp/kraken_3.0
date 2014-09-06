#include "kraken_msgs/dvlData.h"
#include "kraken_msgs/depthData.h"
#include "kraken_msgs/imuData.h"
#include "resources/topicHeader.h"
#include "ros/ros.h"
#include "sensor_msgs/Imu.h"
#include "underwater_sensor_msgs/DVL.h"
#include "underwater_sensor_msgs/Pressure.h"
#include "pose_server/KrakenPose.h"
#include "NqDVL/NqDVL.h"
#include "tracks_imu/Tracks.h"
const double PI=3.14;
class NavPub{
public:
    NavPub(){
        ros::NodeHandle n;
        _kraken_depth=n.advertise<underwater_sensor_msgs::Pressure>(topics::SENSOR_DEPTH,10);
        _kraken_imu=n.advertise<kraken_msgs::imuData>(topics::SENSOR_IMU,20);
        _kraken_dvl=n.advertise<kraken_msgs::dvlData>(topics::SENSOR_DVL,10);


        _sim_depth=n.subscribe(topics::SIMULATOR_DEPTH,10,&NavPub::simDepth2krakenDepth,this);
        _sim_dvl=n.subscribe(topics::SIMULATOR_DVL,10,&NavPub::simDvl2krakenDvl,this);
        _sim_imu=n.subscribe(topics::SIMULATOR_IMU,10,&NavPub::simImu2krakenImu,this);
    }

    void simDepth2krakenDepth(const underwater_sensor_msgs::PressureConstPtr &msg){
         _kraken_depth.publish(msg);
    }
    void simDvl2krakenDvl(const underwater_sensor_msgs::DVLConstPtr &msg){
        kraken_msgs::dvlData d;
        // https://github.com/uji-ros-pkg/underwater_simulation/blob/hydro-devel/underwater_sensor_msgs/msg/DVL.msg
        d.data[kraken_sensors::_dvl_vx]=msg->bs_longitudinal;
        d.data[kraken_sensors::_dvl_vy]=msg->bs_transverse;
        d.data[kraken_sensors::_dvl_vz]=msg->bs_normal;
        _kraken_dvl.publish(d);
    }
    void simImu2krakenImu(const sensor_msgs::ImuConstPtr &msg){
        kraken_msgs::imuData d;
        d.data[kraken_sensors::accelX]=msg->linear_acceleration.x;
        d.data[kraken_sensors::accelY]=msg->linear_acceleration.y;
        d.data[kraken_sensors::accelZ]=msg->linear_acceleration.z;

        geometry_msgs::Vector3 euler;
        QuaternionToEuler(msg->orientation,euler);
        d.data[kraken_sensors::roll]=euler.x;
        d.data[kraken_sensors::pitch]=euler.y;
        d.data[kraken_sensors::yaw]=euler.z;


        d.data[kraken_sensors::gyroX]=msg->angular_velocity.x;
        d.data[kraken_sensors::gyroY]=msg->angular_velocity.y;
        d.data[kraken_sensors::gyroZ]=msg->angular_velocity.z;


        _kraken_imu.publish(d);
    }

private:
    ros::Publisher _kraken_depth,_kraken_dvl,_kraken_imu;
    ros::Subscriber _sim_depth,_sim_dvl,_sim_imu;

    void QuaternionToEuler(const geometry_msgs::Quaternion &quaternion, geometry_msgs::Vector3 &euler)
    {
        double w,x,y,z;

        w = quaternion.w;
        x = quaternion.x;
        y = quaternion.y;
        z = quaternion.z;

        double sqw = w*w;
        double sqx = x*x;
        double sqy = y*y;
        double sqz = z*z;

        euler.z =  (atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw)) * (180.0f/PI));
        euler.x =  (atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw)) * (180.0f/PI));
        euler.y =  (asin(-2.0 * (x*z - y*w)) * (180.0f/PI));

    }
};







int main(int argc,char **argv)
{
    ros::init(argc,argv,"msg_converter");

    ros::spin();
}

