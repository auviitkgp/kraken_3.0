#ifndef SERIALPORT_H
#define SERIALPORT_H

#include <stdio.h>
#include <ros/ros.h>
#include <errno.h>
#include <string>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <cstdlib>

namespace kraken_hardware
{
  #define PERITYODD  PARENB|PARODD
  #define PARITYEN   PARENB
  #define STOPBIT2   CSTOPB
  class SerialPort
  {
    private:
      std::string _port_name;
      long _baudrate;
      bool _port_is_open;
      int _file_descriptor;
      long _port_flags;
      bool configPort();

    public:
      SerialPort();
      SerialPort(const std::string &name,int rate, long flags);
      SerialPort(const std::string &name, int rate);
      SerialPort(const std::string &name);
      bool openPort();
      bool closePort();
      bool isOpen();
      bool setFlags(long flags);
      bool setRate(long rate);
      bool setPortName(const std::string &name);
      int  readData(std::string &bytes, int no_of_bytes);
      bool readChar(char &data);
      int  writeData(const std::string &bytes);
      virtual ~SerialPort();
  };
}// end namespace kraken_hardware
#endif // IKATSERIALPORT_H
