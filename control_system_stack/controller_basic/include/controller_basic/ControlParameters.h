#ifndef CONTROLPARAMETERS_H
#define CONTROLPARAMETERS_H
#include <kraken_msgs/krakenPose.h>
#include <fstream>

namespace kraken_controller
{
  class ControlParameters
  {
    public:
      ControlParameters(int row=6,int col=21);
      void load(const std::string &filename);
      inline float** getGain()
      {
        return _gain;
      }

      inline float* getOffset()
      {
        return _offset;
      }
      inline const std::string getName()
      {
        return _name;
      }
      virtual ~ControlParameters();
      void write(std::ostream &out);
    private:
       float** _gain;
       float* _offset;
       int _row;
       int _col;
       std::string _name;
    protected:
  };
}

#endif // CONTROLPARAMETERS_H
