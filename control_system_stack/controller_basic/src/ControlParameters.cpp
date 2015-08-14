#include <controller_basic/ControlParameters.h>

namespace kraken_controller
{
ControlParameters::ControlParameters(int row, int col):_row(row),_col(col)
{
    _offset = new float[row];
    _gain = new float*[row];

    for(int i=0; i<row; i++)
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
        for(i=0; _gain[i]!=0; i++)
        {
            delete[] _gain[i];
        }
    }
}

void ControlParameters::load(const std::string &filename)
{
    std::ifstream file;
    file.open(filename.c_str());
    ROS_INFO("loading file with name %s outside if",filename.c_str());

    if(file.is_open())
    {
        const char *name=filename.c_str();
        ROS_INFO("loading file with name %s",name);
        file>>_name;

        for(int i=0; i<_row; i++)
        {
            file>>_offset[i];

            for(int j=0; j<_col; j++)
            {
                file>>_gain[i][j];
            }
        }
    }

    ROS_INFO("file not opened %s",filename.c_str());
}
void ControlParameters::load(int *arr[22])
{
    for(int i=0; i<_row; i++)
    {
        _gain[i][0]=arr[i][0];

        for(int j=0; j<_col; j++)
        {
            _gain[i][j]=arr[i][j];
        }
    }
}
void ControlParameters::write(std::ostream &out)
{
    //std::cerr<<"Writing"<<std::endl;
    out<<std::endl<<_name<<std::endl;

    for(int i=0; i<_row; i++)
    {
        out<<_offset[i]<<"\t";

        for(int j=0; j<_col; j++)
        {
            out<<_gain[i][j]<<"\t";
        }

        out<<std::endl;
    }

    out<<std::endl;
}
void ControlParameters::write(FILE* fp)
{
    //std::cerr<<"Writing"<<std::endl;
    fprintf(fp,"\n%s\n",_name.c_str());
    printf("\n%s\n",_name.c_str());

//    out<<std::endl<<_name<<std::endl;
    for(int i=0; i<_row; i++)
    {
        fprintf(fp,"%.2f\t",_offset[i]);

        for(int j=0; j<_col; j++)
        {
            fprintf(fp,"%.2f\t",_gain[i][j]);
        }

        fprintf(fp,"\n");
    }

    fprintf(fp,"\n");
}
}
