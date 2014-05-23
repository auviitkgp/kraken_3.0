#include <controller_basic/ControlParameters.h>

namespace kraken_controller
{
  ControlParameters::ControlParameters(int row, int col)
  {
    _offset = new float[row];
    _gain = new float*[row];
    for(int i=0;i<row;i++)
      {
        _gain[i] = new float[col];
      }
  }
  
  ControlParameters::~ControlParameters()
  {
    if(_offset==0)
    {
    }
  else
    {
      delete[] _offset;
    }
    int i=0;
    if(_gain!=0)
    {
      for(i=0;_gain[i]!=0;i++)
        {
          delete[] _gain[i];
        }
    }
  }
  
  void ControlParameters::load(const std::string &filename)
  {
    
  }
  
}
