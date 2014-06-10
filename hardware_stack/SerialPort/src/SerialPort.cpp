#include <SerialPort/SerialPort.h>

namespace kraken_hardware
{
  SerialPort::SerialPort()
  {
      _port_is_open = false;
      setPortName("");
      setRate(0);
      setFlags(0);
      _file_descriptor = -1;
  }

  SerialPort::SerialPort(const std::string &name,int rate, long flags)
  {
      _port_is_open = false;
      setPortName(name);
      setRate(rate);
      setFlags(flags);
      _file_descriptor = -1;
  }

  SerialPort::SerialPort(const std::string &name, int rate)
  {	
      _port_is_open = false;
      setPortName(name);
      setRate(rate);
      setFlags(0);
      _file_descriptor = -1;
  }

  SerialPort::SerialPort(const std::string &name)
  {
      _port_is_open = false;
      setPortName(name);
      setRate(0);
      setFlags(0);
      _file_descriptor = -1;
  }

  SerialPort::~SerialPort()
  {
      if(_port_is_open)
      {
          closePort();
      }
  }

  bool SerialPort::isOpen()
  {
      return _port_is_open;
  }

  bool SerialPort::openPort()
  {
      if((!_port_is_open)&&(_port_name.c_str()!=NULL))
      {
          _file_descriptor = open(_port_name.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
      }
      else if(_port_is_open)
           {
             ROS_ERROR_STREAM("open port unable to open port : "<< _port_name << " port is already open");
             return false;
           }
           else
           {
             ROS_ERROR_STREAM("open port unable to open port : no port name is given");
             return false;
           }

      if(_file_descriptor==-1)
      {
          ROS_ERROR_STREAM("open port unable to open port : "<< _port_name);
          return false;
      }
      else
      {
          fcntl(_file_descriptor,F_SETFL,0);
          _port_is_open = true;
          if(configPort())
          {
            return true;
          }
          else
          {
              ROS_ERROR_STREAM("Can't configure the port named :" << _port_name);
              return false;
          }
      }
  }

  bool SerialPort::closePort()
  {
      if(_port_is_open)
      {
          close(_file_descriptor);
          _port_is_open = false;
      }
      else
      {
          ROS_ERROR_STREAM("Serial Port : "<<_port_name<<
                           " is already closed");
      }
      return _port_is_open;
  }

  bool SerialPort::setPortName(const std::string &name)
  {
      if(!_port_is_open)
      {
          _port_name = name;
      }
      else
      {
          ROS_ERROR_STREAM("Serial Port is already been used as name : "<<_port_name<<
                           ". Hence the name cannot change be changed");
      }
      return !_port_is_open;
  }

  bool SerialPort::setFlags(long flags)
  {
      if(!_port_is_open)
      {
          _port_flags = flags;
      }
      else
      {
          ROS_ERROR_STREAM("Serial Port is already been used as name : "<<_port_name<<
                           ". Hence the flags cannot be changed");
      }
      return !_port_is_open;
  }

  bool SerialPort::setRate(long rate)
  {
      if(!_port_is_open)
      {
          switch (rate)
                {
                   case 38400:
                   default:
                      _baudrate = B38400;
                      break;
                   case 19200:
                      _baudrate  = B19200;
                      break;
                   case 9600:
                      _baudrate  = B9600;
                      break;
                   case 4800:
                      _baudrate  = B4800;
                      break;
                   case 2400:
                      _baudrate  = B2400;
                      break;
                   case 1800:
                      _baudrate  = B1800;
                      break;
                   case 1200:
                      _baudrate  = B1200;
                      break;
                   case 600:
                      _baudrate  = B600;
                      break;
                   case 300:
                      _baudrate  = B300;
                      break;
                   case 200:
                      _baudrate  = B200;
                      break;
                   case 150:
                      _baudrate  = B150;
                      break;
                   case 134:
                      _baudrate  = B134;
                      break;
                   case 110:
                      _baudrate  = B110;
                      break;
                   case 75:
                      _baudrate  = B75;
                      break;
                   case 50:
                      _baudrate  = B50;
                      break;
                }
      }
      else
      {
          ROS_ERROR_STREAM("Serial Port is already been used as name : "<<_port_name<<
                           ". Hence the rate cannot be changed");
      }
      return !_port_is_open;
  }

  bool SerialPort::configPort()
  {
      struct termios port_setting;
      tcgetattr(_file_descriptor, &port_setting);
      cfsetispeed(&port_setting,_baudrate);
      cfsetospeed(&port_setting,_baudrate);
      port_setting.c_cflag |= (CLOCAL | CREAD);
      port_setting.c_cflag &= ~PARENB;
      port_setting.c_cflag &= ~CSTOPB;
      port_setting.c_cflag &= ~CSIZE;
      port_setting.c_cflag |= CS8;
      port_setting.c_cflag &= ~(CRTSCTS);
      port_setting.c_cflag |= _port_flags;
      port_setting.c_iflag |= (IXON | IXOFF | IXANY);
      port_setting.c_iflag &= ~(ICRNL);
      //port_setting.c_oflag &= ~(ONLCR | OCRNL | ONOCR  | ONLRET);
      port_setting.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
      port_setting.c_cc[VTIME]=10;
      tcsetattr(_file_descriptor,TCSANOW, &port_setting);
      return true;
  }

  int SerialPort::writeData(const std::string &bytes)
  {
      int byte_written = -1;
      int no_of_bytes = bytes.size();
      if(_port_is_open)
      {
          byte_written = write(_file_descriptor, bytes.c_str(),no_of_bytes);
      }
      return byte_written;
  }

  int SerialPort::readData(std::string &bytes, int no_of_bytes)
  {
      int byte_read = -1;

      fd_set read_fds,write_fds,except_fds;
      FD_ZERO(&read_fds);
      FD_ZERO(&write_fds);
      FD_ZERO(&except_fds);
      FD_SET(_file_descriptor,&read_fds);
      struct timeval time;
      time.tv_sec = 1;
      time.tv_usec = 0;
      if(_port_is_open)
      {
          bytes.clear();
          if(select(_file_descriptor+1,&read_fds,&write_fds,&except_fds,&time)==1)
          {
            bytes.resize(no_of_bytes);
            byte_read = read(_file_descriptor, (char *)bytes.c_str(),no_of_bytes);
          }
          else
          {

          }
      }
      return byte_read;
  }

  bool SerialPort::readChar(char &data)
  {
      int byte_read = -1;
      if(_port_is_open)
      {
          data = 0;
          byte_read = read(_file_descriptor,&data,1);
          return true;
      }
      return false;
  }

}	
