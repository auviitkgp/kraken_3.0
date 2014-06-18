#include "iostream"
#include "eigen3/Eigen/Dense"
#include "eigen3/Eigen/Geometry"


using namespace Eigen;
int main()
{
    Vector2d U(2,3);
    RowVector2d V(4,5);
    V*U;
    return 0;
}
