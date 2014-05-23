#ifndef CONTROLPARAMETERS_H
#define CONTROLPARAMETERS_H
#include <kraken_msgs/krakenPose.h>

namespace kraken_controller
{
  class ControlParameters
  {
    public:
      ControlParameters(int row,int col);
      void load(const std::string &filename);
      inline float** getGain()
      {
        return _gain;
      }

      inline float* getOffset()
      {
        return _offset;
      }
      virtual ~ControlParameters();
    private:
       float** _gain;
       float* _offset;
    protected:
  };
}

#endif // CONTROLPARAMETERS_H
