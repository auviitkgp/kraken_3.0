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
    void load(int *[21]);


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
    inline int getRows()
    {
        return _row;
    }
    inline int getColumns()
    {
        return _col;
    }

    virtual ~ControlParameters();
    void write(FILE *fp);
    void write(std::ostream &out);

private:





protected:
    float** _gain;
    float* _offset;
    int _row;
    int _col;
    std::string _name;
};
}

#endif // CONTROLPARAMETERS_H
