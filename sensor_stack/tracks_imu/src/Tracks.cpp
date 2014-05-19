#include <tracks_imu/Tracks.h>

namespace kraken_sensors
{
  Tracks::Tracks(const std::string &name,int freq, int rate):SerialPort(name,rate)
  {
    ros::NodeHandle n;
    _publisher = n.advertise<kraken_msgs::imuData>("/kraken/imu_data",2);
    this->openPort();
    _timer = n.createTimer(ros::Duration(1.0/freq),&Tracks::timerCallBack,this);
    _delay = 500;
  }
  
  bool Tracks::start()
  {
    if(this->isOpen())
    {
        setAllMode();
        //setMode(12);
        return true;
    }
    else
    {
      return false;
    }
  }
  
  std::string Tracks::getInfo()
  {
    std::string modfInfo;
    modfInfo+=(char)0x00;
    modfInfo+=(char)0x05;
    modfInfo+=(char)0x01;
    modfInfo+=(char)0xEF;
    modfInfo+=(char)0xD4;
    std::string response;

    this->writeData(modfInfo);
    usleep(_delay);
    this->readData(response,13);
    return response;
  }
  
  unsigned short Tracks::getCRC16(const std::string &data)
  {
    std::bitset<16> CRC(0);
    for(int i=0;i<data.length();i++)
    {
        std::bitset<8> num(data[i]);
        for(int j=7;j>=0;j--)
        {
            bool DoInvert = num[j]^CRC[15];
            CRC[15] = CRC[14];
            CRC[14] = CRC[13];
            CRC[13] = CRC[12];
            CRC[12] = CRC[11] ^ DoInvert;
            CRC[11] = CRC[10];
            CRC[10] = CRC[9];
            CRC[9] = CRC[8];
            CRC[8] = CRC[7];
            CRC[7] = CRC[6];
            CRC[6] = CRC[5];
            CRC[5] = CRC[4] ^ DoInvert;
            CRC[4] = CRC[3];
            CRC[3] = CRC[2];
            CRC[2] = CRC[1];
            CRC[1] = CRC[0];
            CRC[0] = DoInvert;
        }
    }
    return CRC.to_ulong();
  }
  
  void Tracks::timerCallBack(const ros::TimerEvent &)
  {
    getAllData();
    _publisher.publish(_data);
  }
  
  void Tracks::setMode(int deviceNum)
  {
    std::string payload;
    payload+=(char)0x00;
    payload+=(char)0x07;
    payload+=(char)0x03 ;
    payload+=(char)0x01 ;
    payload+=(char)componentId[deviceNum];
    unsigned short crc= getCRC16(payload);
    payload+=(char)(crc>>8);
    payload+=(char)(crc);
    this->writeData(payload);
    usleep(_delay);
    
  }
  
  void Tracks::setAllMode()
  {
      //char payload[16] = {0x00, 0x12, 0x0D,0x19, 0x18, 0x05, 0x15, 0x16, 0x17, 0x1B, 0x1C, 0x1D, 0x4A, 0x4B, 0x4C, 0x07};
      //{0x19, 0x18, 0x05, 0x15, 0x16, 0x17, 0x1B, 0x1C, 0x1D, 0x4A, 0x4B, 0x4C, 0x07};
      sensor_used = 0x0c;
      std::string payload;
      payload+=(char)0x00;
      payload+=(char)(sensor_used+6);
      payload+=(char)0x03;
      payload+=(char)sensor_used;
      payload+=(char)0x19;//4
      payload+=(char)0x18;//4
      payload+=(char)0x05;//4
      payload+=(char)0x15;//4
      payload+=(char)0x16;
      payload+=(char)0x17;
      payload+=(char)0x1B;
      payload+=(char)0x1C;
      payload+=(char)0x1D;
      payload+=(char)0x4A;
      payload+=(char)0x4B;
      payload+=(char)0x4C;
      //payload+=(char)0x07;//4*/
      unsigned short crc= getCRC16(payload);
      payload+=(char)(crc>>8);
      payload+=(char)(crc);

      this->writeData(payload);
      usleep(_delay);
  }
  
  void Tracks::getAllData()
  {
    /*std::string response;
    getData(4);
    response.clear();
    usleep(_delay);
    this->readData(response, 6 + 5*sensor_used);
    //std::cout<<response<<std::endl;
    getValue(response,sensor_used);*/
      getRPY();
      getAccel();
      getMag();
      getGyro();
      //getTemp();
  }
  
  void Tracks::getValue(const std::string &output, const int length)
  {
    //int len = output.length();
    int start;
    //int dataLen = stoi(output.substr(0,2));
    int deviceId;
    std::string buff;
    float value;
    buff = output.substr(0,4);
    value = dataToFloat32(buff);
    std::string payload = output.substr(0,length*5+4);
    //payload[3] = (unsigned char)0x0d;
    unsigned short crc = (unsigned short)output[length*5+4]<<8;
    crc +=output[length*5+5];
    unsigned short crc_cal = getCRC16(payload);
    if(crc!=crc_cal)
        return;
    printf("crc: %x %x",crc,crc_cal);
    //int cast = reinterpret_cast<int>(value);
    printf("Head : %x %x %x %x \n",(unsigned char)buff[0],(unsigned char)buff[1],(unsigned char)buff[2],(unsigned char)buff[3]);
    int j=0;
    for(start = 4;start<(length*5+1);start+=5)
      {
        buff = output.substr(start,5);
        value = dataToFloat32(buff);
        printf("%x %x %x %x %x\t\t",(unsigned char)buff[0],(unsigned char)buff[1],(unsigned char)buff[2],(unsigned char)buff[3],(unsigned char)buff[4]);
        std::cout<<value<<"\n";
        _data.data[j++]=value;
      }
    std::cout<<std::endl;

  }
  
  float Tracks::dataToFloat32(const std::string &buffer)
  {
      unsigned int x=0;
      float data;
      x|=(unsigned char)buffer[1];
      for(int i=2;i<buffer.length();i++)
      {
        x=x<<8;
        x|=(unsigned char)buffer[i];
      }
/*
      printf("binary : %x\n",x);

      int sign = 1;
      if(x&0x80000000)
        {
          sign = -1;
        }
      int exponent = (x&0x7fffffff)>>23;
      int mantissa = (x&0x007fffff);
      data = sign*pow(2,exponent-127)*(1+mantissa/10000000.0);
      std::cout<<"Sign : "<<sign<<std::endl;
      std::cout<<"Mantissa : "<<mantissa<<std::endl;
      std::cout<<"Exponent : "<<exponent<<std::endl;*/
      memcpy(&data, &x, sizeof(int));
      return data;
  }
  
  void  Tracks::getData(int dataLength)
  {
      //char getData[8] = {0x00,0x05,0x04,0xbf,0x71};
      std::string payload;
      payload+=(char)0x00;
      payload+=(char)0x05;
      payload+=(char)0x04;
      unsigned short crc= getCRC16(payload);
      payload+=(char)(crc>>8);
      payload+=(char)(crc);
      this->writeData(payload);
      usleep(_delay);
  }
  void Tracks::setRPY()
  {
      sensor_used = 0x03;
      std::string payload;
      payload+=(char)0x00;
      payload+=(char)(sensor_used+6);
      payload+=(char)0x03;
      payload+=(char)sensor_used;
      payload+=(char)0x19;//4
      payload+=(char)0x18;//4
      payload+=(char)0x05;//4
      unsigned short crc= getCRC16(payload);
      payload+=(char)(crc>>8);
      payload+=(char)(crc);
      this->writeData(payload);
      usleep(_delay);
  }

  void Tracks::setAccel()
  {
      sensor_used = 0x03;
      std::string payload;
      payload+=(char)0x00;
      payload+=(char)(sensor_used+6);
      payload+=(char)0x03;
      payload+=(char)sensor_used;
      payload+=(char)0x15;//4
      payload+=(char)0x16;//4
      payload+=(char)0x17;//4
      unsigned short crc= getCRC16(payload);
      payload+=(char)(crc>>8);
      payload+=(char)(crc);
      this->writeData(payload);
      usleep(_delay);
  }

  void Tracks::setGyro()
  {
      sensor_used = 0x03;
      std::string payload;
      payload+=(char)0x00;
      payload+=(char)(sensor_used+6);
      payload+=(char)0x03;
      payload+=(char)sensor_used;
      payload+=(char)0x1b;//4
      payload+=(char)0x1c;//4
      payload+=(char)0x1d;//4
      unsigned short crc= getCRC16(payload);
      payload+=(char)(crc>>8);
      payload+=(char)(crc);
      this->writeData(payload);
      usleep(_delay);
  }

  void Tracks::setMag()
  {
      sensor_used = 0x03;
      std::string payload;
      payload+=(char)0x00;
      payload+=(char)(sensor_used+6);
      payload+=(char)0x03;
      payload+=(char)sensor_used;
      payload+=(char)0x4a;//4
      payload+=(char)0x4b;//4
      payload+=(char)0x4c;//4
      unsigned short crc= getCRC16(payload);
      payload+=(char)(crc>>8);
      payload+=(char)(crc);
      this->writeData(payload);
      usleep(_delay);
  }

  void Tracks::setTemp()
  {
      sensor_used = 0x01;
      std::string payload;
      payload+=(char)0x00;
      payload+=(char)(sensor_used+6);
      payload+=(char)0x03;
      payload+=(char)sensor_used;
      payload+=(char)0x07;//4
      unsigned short crc= getCRC16(payload);
      payload+=(char)(crc>>8);
      payload+=(char)(crc);
      this->writeData(payload);
      usleep(_delay);
  }

  void Tracks::getRPY()
  {
    setRPY();
    int start;
    std::string output;
    getData(4);
    usleep(_delay);
    this->readData(output, 6 + 5*sensor_used);
    std::string buff;
    float value;
    buff = output.substr(0,4);
    value = dataToFloat32(buff);
    std::string payload = output.substr(0,sensor_used*5+4);
    unsigned short crc = (unsigned short)output[sensor_used*5+4]<<8;
    crc +=output[sensor_used*5+5];
    unsigned short crc_cal = getCRC16(payload);
    if(crc!=crc_cal)
        return;
    printf("crc: %x %x",crc,crc_cal);
    //int cast = reinterpret_cast<int>(value);
    printf("Head : %x %x %x %x \n",(unsigned char)buff[0],(unsigned char)buff[1],(unsigned char)buff[2],(unsigned char)buff[3]);
    int j=roll;
    for(start = 4;start<(sensor_used*5+1);start+=5)
      {
        buff = output.substr(start,5);
        value = dataToFloat32(buff);
        printf("%x %x %x %x %x\t\t",(unsigned char)buff[0],(unsigned char)buff[1],(unsigned char)buff[2],(unsigned char)buff[3],(unsigned char)buff[4]);
        std::cout<<value<<"\n";
        _data.data[j++]=value;
      }
    std::cout<<std::endl;
  }
  void Tracks::getAccel()
  {
    setAccel();
    int start;
    std::string output;
    getData(4);
    usleep(_delay);
    this->readData(output, 6 + 5*sensor_used);
    std::string buff;
    float value;
    buff = output.substr(0,4);
    value = dataToFloat32(buff);
    std::string payload = output.substr(0,sensor_used*5+4);
    unsigned short crc = (unsigned short)output[sensor_used*5+4]<<8;
    crc +=output[sensor_used*5+5];
    unsigned short crc_cal = getCRC16(payload);
    if(crc!=crc_cal)
        return;
    printf("crc: %x %x",crc,crc_cal);
    //int cast = reinterpret_cast<int>(value);
    printf("Head : %x %x %x %x \n",(unsigned char)buff[0],(unsigned char)buff[1],(unsigned char)buff[2],(unsigned char)buff[3]);
    int j=accelX;
    for(start = 4;start<(sensor_used*5+1);start+=5)
      {
        buff = output.substr(start,5);
        value = dataToFloat32(buff);
        printf("%x %x %x %x %x\t\t",(unsigned char)buff[0],(unsigned char)buff[1],(unsigned char)buff[2],(unsigned char)buff[3],(unsigned char)buff[4]);
        std::cout<<value<<"\n";
        _data.data[j++]=value;
      }
    std::cout<<std::endl;
  }
  void Tracks::getGyro()
  {
    setGyro();
    int start;
    std::string output;
    getData(4);
    usleep(_delay);
    this->readData(output, 6 + 5*sensor_used);
    std::string buff;
    float value;
    buff = output.substr(0,4);
    value = dataToFloat32(buff);
    std::string payload = output.substr(0,sensor_used*5+4);
    unsigned short crc = (unsigned short)output[sensor_used*5+4]<<8;
    crc +=output[sensor_used*5+5];
    unsigned short crc_cal = getCRC16(payload);
    if(crc!=crc_cal)
        return;
    printf("crc: %x %x",crc,crc_cal);
    //int cast = reinterpret_cast<int>(value);
    printf("Head : %x %x %x %x \n",(unsigned char)buff[0],(unsigned char)buff[1],(unsigned char)buff[2],(unsigned char)buff[3]);
    int j=gyroX;
    for(start = 4;start<(sensor_used*5+1);start+=5)
      {
        buff = output.substr(start,5);
        value = dataToFloat32(buff);
        printf("%x %x %x %x %x\t\t",(unsigned char)buff[0],(unsigned char)buff[1],(unsigned char)buff[2],(unsigned char)buff[3],(unsigned char)buff[4]);
        std::cout<<value<<"\n";
        _data.data[j++]=value;
      }
    std::cout<<std::endl;
  }
  void Tracks::getMag()
  {
    setMag();
    int start;
    std::string output;
    getData(4);
    usleep(_delay);
    this->readData(output, 6 + 5*sensor_used);
    std::string buff;
    float value;
    buff = output.substr(0,4);
    value = dataToFloat32(buff);
    std::string payload = output.substr(0,sensor_used*5+4);
    unsigned short crc = (unsigned short)output[sensor_used*5+4]<<8;
    crc +=output[sensor_used*5+5];
    unsigned short crc_cal = getCRC16(payload);
    if(crc!=crc_cal)
        return;
    printf("crc: %x %x",crc,crc_cal);
    //int cast = reinterpret_cast<int>(value);
    printf("Head : %x %x %x %x \n",(unsigned char)buff[0],(unsigned char)buff[1],(unsigned char)buff[2],(unsigned char)buff[3]);
    int j=magX;
    for(start = 4;start<(sensor_used*5+1);start+=5)
      {
        buff = output.substr(start,5);
        value = dataToFloat32(buff);
        printf("%x %x %x %x %x\t\t",(unsigned char)buff[0],(unsigned char)buff[1],(unsigned char)buff[2],(unsigned char)buff[3],(unsigned char)buff[4]);
        std::cout<<value<<"\n";
        _data.data[j++]=value;
      }
    std::cout<<std::endl;
  }

  void Tracks::getTemp()
  {
    setTemp();
    int start;
    std::string output;
    getData(4);
    usleep(_delay);
    this->readData(output, 6 + 5*sensor_used);
    std::string buff;
    float value;
    buff = output.substr(0,4);
    value = dataToFloat32(buff);
    std::string payload = output.substr(0,sensor_used*5+4);
    unsigned short crc = (unsigned short)output[sensor_used*5+4]<<8;
    crc +=output[sensor_used*5+5];
    unsigned short crc_cal = getCRC16(payload);
    if(crc!=crc_cal)
        return;
    printf("crc: %x %x",crc,crc_cal);
    //int cast = reinterpret_cast<int>(value);
    printf("Head : %x %x %x %x \n",(unsigned char)buff[0],(unsigned char)buff[1],(unsigned char)buff[2],(unsigned char)buff[3]);
    int j=temp;
    for(start = 4;start<(sensor_used*5+1);start+=5)
      {
        buff = output.substr(start,5);
        value = dataToFloat32(buff);
        printf("%x %x %x %x %x\t\t",(unsigned char)buff[0],(unsigned char)buff[1],(unsigned char)buff[2],(unsigned char)buff[3],(unsigned char)buff[4]);
        std::cout<<value<<"\n";
        _data.data[j++]=value;
      }
    std::cout<<std::endl;
  }
}
