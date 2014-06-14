#include <iostream>
#include <vgateserver/vgateserver.h>

using namespace std;

int main(int argc, char ** argv)
{
    ros::init(argc, argv, "vgateserver");
    Vgate vgateserver("vgate");

    ros::spin();

	return 0;
}
