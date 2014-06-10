#include <tracks_imu/Tracks.h>


int main(int argc,char** argv)
{
  ros::init(argc,argv,"Tracks_imu_node");
  kraken_sensors::Tracks _imu("/dev/ttyUSB0",50);
  std::string info = _imu.getInfo();
  for(int j = 0; j< info.length(); j++)
      printf(" %x",(unsigned char)info[j]);
  printf("\n");

  //std::cout<<_imu.getInfo()<<std::endl;
  _imu.start();
  std::string buff;
  buff+=(char)0x41;
  buff+=(char)0xe6;
  buff+=(char)0x80;
  buff+=(char)0x00;
//  buff+=(char)0x42;
//  buff+=(char)0x05;
//  buff+=(char)0x60;
//  buff+=(char)0x00;
  //std::cout<<_imu.dataToFloat32(buff)<<std::endl;
  //std::cout<< sizeof(float)<<std::endl;
  ros::spin();
  return 0;  
}
