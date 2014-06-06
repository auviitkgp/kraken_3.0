#include <controller_basic/ControlParameters.h>

namespace kraken_controller
{
  ControlParameters::ControlParameters(int row, int col):_row(row),_col(col)
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
    std::ifstream file;
    file.open(filename.c_str());
    if(file.is_open())
    {
        file>>_name;
        for(int i=0;i<_row;i++)
        {
            file>>_offset[i];
            for(int j=0;j<_col;j++)
            {
                file>>_gain[i][j];
            }
        }
    }
  }
  
  void ControlParameters::write(std::ostream &out)
  {
    //std::cerr<<"Writing"<<std::endl;
    out<<std::endl<<_name<<std::endl;
    for(int i=0;i<_row;i++)
    {
        out<<_offset[i]<<"\t";
        for(int j=0;j<_col;j++) 
        {
          out<<_gain[i][j]<<"\t";
        }
        out<<std::endl;
    }
    out<<std::endl;
  }
  
}
