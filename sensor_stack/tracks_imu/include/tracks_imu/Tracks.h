#ifndef TRACKS_H
#define TRACKS_H
#include <SerialPort/SerialPort.h>
#include <string>
#include <kraken_msgs/imuData.h>
#include <ros/ros.h>
#include <ros/timer.h>
#include <ros/publisher.h>
#include <bitset>
namespace kraken_sensors
{
  enum{
       roll=0, pitch, yaw, accelX, accelY, accelZ, magX, magY, magZ, gyroX, gyroY, gyroZ, temp
  };
  const char componentId[13] = {0x19, 0x18, 0x05, 0x15, 0x16, 0x17, 0x1B, 0x1C, 0x1D, 0x4A, 0x4B, 0x4C, 0x07};
  class Tracks:public kraken_hardware::SerialPort
  {
    private:
      kraken_msgs::imuData _data;
      void setMode(const int deviceNum);
      void setRPY();
      void setAccel();
      void setMag();
      void setGyro();
      void setTemp();
      bool getRPY();
      bool getAccel();
      bool getMag();
      bool getGyro();
      bool getTemp();
      unsigned short getCRC16(const std::string &data);
      void getData(const int dataLength);
      ///float dataToFloat32(const std::string &data);
      bool getValue(const std::string &data, const int length);
      void setAllMode();
      void setContinousMode();
      bool getAllData();
      ros::Publisher _publisher;
      ros::Timer _timer;
      int sensor_used;
      int _delay;
      int count;
      kraken_msgs::imuData _offset;
      static const double pi;
    protected:
    public:
      Tracks(const std::string &name,int freq=20, int rate=38400);
      virtual ~Tracks(){}
      std::string getInfo();
      bool start();
      void timerCallBack(const ros::TimerEvent &);
      float dataToFloat32(const std::string &data);
      
  };
  
}

#endif // TRACKS_H
