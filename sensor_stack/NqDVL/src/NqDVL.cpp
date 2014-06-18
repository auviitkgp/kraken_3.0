#include <NqDVL/NqDVL.h>
#include <ros/ros.h>

using namespace std;
using namespace kraken_hardware;

char * pEnd;
char *b;

std::string bytes;


namespace kraken_sensors
{

  NqDVL::NqDVL(const string &name, int freq, int baudrate):SerialPort(name,baudrate),_freq(freq)
  {
    ros::NodeHandle n;
    _publisher = n.advertise<kraken_msgs::dvlData>("/kraken/dvl_data",2);
  }
  
  bool NqDVL::start(bool debug)
  {
    _debug = debug;
    openPort();
    if(isOpen())
      {
        ros::NodeHandle n;
        _timer = n.createTimer(ros::Duration(1.0/_freq),&NqDVL::timerCallBack,this);
      }
    else
      {
        return false;
      }
  }
  
  void NqDVL::timerCallBack(const ros::TimerEvent &)
  {
    readDVLData();
    getVal();
    _publisher.publish(_data);
  }
  
  void NqDVL::readDVLData()
  {
    _rawData.clear();
    std::string _garbage;
    while(ros::ok())
    {
        if(readData(_garbage, 1) != -1)
        {
            if(_garbage[0] == '$')
            {
                if(readData(_garbage, 1))
                {
                    if(_garbage[0] == '#')
                        break;
                }
            }
        }
        else
        {
            cout << "Unable to read" << endl;
        }
    }

    do{
        if(readData(_garbage, 1) != -1)
        {
            _rawData.append((_garbage));
        }
        else
        {
            break;
        }
    }while(_garbage[0] != '\r' || ros::ok());
  }

  void NqDVL::getVal()
  {
       _errorCode = strtod(b, &pEnd);
       _goodOrBad[0] = strtod(pEnd, &b);
       _goodOrBad[1] = strtod(b, &pEnd);
       _goodOrBad[2] = strtod(pEnd, &b);
       _goodOrBad[3] = strtod(b, &pEnd);
       _vAltitude[0] = strtod(pEnd, &b);
       _vAltitude[1] = strtod(b, &pEnd);
       _vAltitude[2] = strtod(pEnd, &b);
       _vAltitude[3] = strtod(b, &pEnd);
       _veloRad[0] = strtod(pEnd, &b);
       _veloRad[1] = strtod(b, &pEnd);
       _veloRad[2] = strtod(pEnd, &b);
       _veloRad[3] = strtod(b, &pEnd);
       _wveloCredit[0] = strtod(pEnd, &b);
       _wveloCredit[1] = strtod(b, &pEnd);
       _wveloCredit[2] = strtod(pEnd, &b);
       _wveloCredit[3] = strtod(b, &pEnd);
       _veloInstrument[0] = strtod(pEnd, &b);
       _veloInstrument[1] = strtod(b, &pEnd);
       _veloInstrument[2] = strtod(pEnd, &b);
       _veloInstrument[3] = strtod(b, &pEnd);
       _veloEarth[0] = strtod(pEnd, &b);
       _veloEarth[1] = strtod(b, &pEnd);
       _veloEarth[2] = strtod(pEnd, &b);
       _veloEarth[3] = strtod(b, &pEnd);
       _waterVeloInstrument[0] = strtod(pEnd, &b);
       _waterVeloInstrument[1] = strtod(b, &pEnd);
       _waterVeloInstrument[2] = strtod(pEnd, &b);
       _waterVeloInstrument[3] = strtod(b, &pEnd);
       _waterVeloEarth[0] = strtod(pEnd, &b);
       _waterVeloEarth[1] = strtod(b, &pEnd);
       _waterVeloEarth[2] = strtod(pEnd, &b);
       _waterVeloEarth[3] = strtod(b, &pEnd);
       _data.data[_dvl_roll] = _rph[0] = strtod(pEnd, &b);
       _data.data[_dvl_pitch] = _rph[1] = strtod(b, &pEnd);
       _data.data[_dvl_yaw] = _rph[2] = strtod(pEnd, &b);
       _data.data[_dvl_depth] = _altitudeEstimate = strtod(b, &pEnd);
       _data.data[_dvl_temperature] = _temperature = strtod(pEnd, &b);
       _data.data[_dvl_salinity] = _salinity = strtod(b, &pEnd);
       _data.data[_dvl_sound_speed] = _soundSpeed = strtod(pEnd, &b);
       _checksum = strtod(b, &pEnd);
  }
   
  void NqDVL::showVal()
  {  
      cout<<"starting to print the values from Errorcode.\n"<<_errorCode;
      for(int i=0; i<4; i++)
          cout<<"\n"<<"GoodOrBad["<< i<<"] : "<<_goodOrBad[i];
      for(int i=0; i<4; i++)
        cout<<"\n"<<"Valtitude["<< i<<"] : "<<_vAltitude[i];
      for(int i=0; i<4; i++)
          cout<<"\n"<<"VeloRad["<< i<<"] : "<<_veloRad[i];
      for(int i=0; i<4; i++)
          cout<<"\n"<<"WveloCredit["<< i<<"] : "<<_wveloCredit[i];
      for(int i=0; i<4; i++)
          cout<<"\n"<<"VeloInstrument["<< i<<"] : "<<_veloInstrument[i];
      for(int i=0; i<4; i++)
          cout<<"\n"<<"VeloEarth["<< i<<"] : "<<_veloEarth[i];
      for(int i=0; i<4; i++)
          cout<<"\n"<<"WaterVeloInstrument["<< i<<"] : "<<_waterVeloInstrument[i];
      for(int i=0; i<4; i++)
          cout<<"\n"<<"WaterVeloEarth["<< i<<"] : "<<_waterVeloEarth[i];
      for(int i=0; i<3; i++)
          cout<<"\n"<<"RPY["<< i<<"] : "<<_rph[i];
      cout<<"\n"<<"AltitudeEstimate : "<<_altitudeEstimate;
      cout<<"\n"<<"Temperature : "<<_temperature;
      cout<<"\n"<<"Salinity : "<<_salinity;
      cout<<"\n"<<"SoundSpeed : "<<_soundSpeed;
      cout<<"Ending at the Value of _checksum"<<_checksum;
  }

}


/*
  NqDVL::NqDVL(const std::string &name, int rate, long flag) : _dvlIO(name, rate, flag)
  {
      _dvlIO.openPort();
      if(_dvlIO.isOpen()==true)
      {
          cout<<"Port has been opened" << endl;
      }
      else
          cout<<"Port opening failed" << endl;
      cout << "BBYe" << endl;
  }
 * 
  void NqDVL::readDVLData()
  {
      _rawData.clear();
      std::string _garbage;
      while(ros::ok())
      {
          if(_dvlIO.readData(_garbage, 1) != -1)
          {
              if(_garbage[0] == '$')
              {
                  cout << "Inside"<<_garbage[0] << endl;
  
                  cout << "Inside dollar" << endl;
                  if(_dvlIO.readData(_garbage, 1))
                  {
                      if(_garbage[0] == '#')
                          break;
                  }
              }
          }
          else
          {
              cout << "Unable to read" << endl;
          }
          cout << "Inside"<<_garbage[0] << endl;
      }
  
      do{
          if(_dvlIO.readData(_garbage, 1) != -1)
          {
              _rawData.append((_garbage));
          }
          else
          {
              break;
          }
          cout << "Inside buf"<<_garbage[0] << endl;
      }while(_garbage[0] != '\r' || ros::ok());
  
      cout << _rawData.c_str() << endl;
  }
*  
  
  void NqDVL::getVal()
  {
       _errorCode = strtod(b, &pEnd);
       _goodOrBad[0] = strtod(pEnd, &b);
       _goodOrBad[1] = strtod(b, &pEnd);
       _goodOrBad[2] = strtod(pEnd, &b);
       _goodOrBad[3] = strtod(b, &pEnd);
       _vAltitude[0] = strtod(pEnd, &b);
       _vAltitude[1] = strtod(b, &pEnd);
       _vAltitude[2] = strtod(pEnd, &b);
       _vAltitude[3] = strtod(b, &pEnd);
       _veloRad[0] = strtod(pEnd, &b);
       _veloRad[1] = strtod(b, &pEnd);
       _veloRad[2] = strtod(pEnd, &b);
       _veloRad[3] = strtod(b, &pEnd);
       _wveloCredit[0] = strtod(pEnd, &b);
       _wveloCredit[1] = strtod(b, &pEnd);
       _wveloCredit[2] = strtod(pEnd, &b);
       _wveloCredit[3] = strtod(b, &pEnd);
       _veloInstrument[0] = strtod(pEnd, &b);
       _veloInstrument[1] = strtod(b, &pEnd);
       _veloInstrument[2] = strtod(pEnd, &b);
       _veloInstrument[3] = strtod(b, &pEnd);
       _veloEarth[0] = strtod(pEnd, &b);
       _veloEarth[1] = strtod(b, &pEnd);
       _veloEarth[2] = strtod(pEnd, &b);
       _veloEarth[3] = strtod(b, &pEnd);
       _waterVeloInstrument[0] = strtod(pEnd, &b);
       _waterVeloInstrument[1] = strtod(b, &pEnd);
       _waterVeloInstrument[2] = strtod(pEnd, &b);
       _waterVeloInstrument[3] = strtod(b, &pEnd);
       _waterVeloEarth[0] = strtod(pEnd, &b);
       _waterVeloEarth[1] = strtod(b, &pEnd);
       _waterVeloEarth[2] = strtod(pEnd, &b);
       _waterVeloEarth[3] = strtod(b, &pEnd);
       _rph[0] = strtod(pEnd, &b);
       _rph[1] = strtod(b, &pEnd);
       _rph[2] = strtod(pEnd, &b);
       _altitudeEstimate = strtod(b, &pEnd);
       _temperature = strtod(pEnd, &b);
       _salinity = strtod(b, &pEnd);
       _soundSpeed = strtod(pEnd, &b);
       _checksum = strtod(b, &pEnd);
  }
*/ 
