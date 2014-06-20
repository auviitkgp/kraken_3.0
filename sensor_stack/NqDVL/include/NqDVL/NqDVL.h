#ifndef _SENSORS_NQDVL_H
#define _SENSORS_NQDVL_H

#include <SerialPort/SerialPort.h>
#include <kraken_msgs/dvlData.h>



namespace kraken_sensors
{
  enum
  {
    _dvl_vx,
    _dvl_vy,
    _dvl_vz,
    _dvl_depth,
    _dvl_roll,
    _dvl_pitch,
    _dvl_yaw,
    _dvl_salinity,
    _dvl_sound_speed,
    _dvl_temperature
  };
  
  class NqDVL: public kraken_hardware::SerialPort
  {
  private:
      kraken_msgs::dvlData _data;
      double _errorCode;
      double _goodOrBad[4];
      double _vAltitude[4];
      double _veloRad[4];
      double _wveloRad[4];
      double _wveloCredit[4];
      double _veloInstrument[4];
      double _veloEarth[4];
      double _waterVeloInstrument[4];
      double _waterVeloEarth[4];
      double _rph[3];
      double _altitudeEstimate;
      double _temperature;
      double _salinity;
      double _soundSpeed;
      double _checksum;
      bool _debug;
      int _freq;
      std::string _rawData;
      ros::Publisher _publisher;
      ros::Timer _timer;
  public:
      NqDVL(const std::string &name, int freq=2,int baudrate=115200);
      bool start(bool debug=false);
      void readDVLData();
      void getVal();
      void showVal();
      void timerCallBack(const ros::TimerEvent &);
};
}

#endif
