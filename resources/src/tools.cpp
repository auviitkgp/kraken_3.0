#include "resources/tools.h"

namespace tools
{

bool getVerboseTag(int argc, char** argv)
{
    char tag1[] = "--verbose";
    char tag2[] = "--debug";

    if (argc >= 2)
    {
        if (strcmp(argv[1], tag1) == 0 || strcmp(argv[1], tag2) == 0)
        {
            return true;
        }
    }

    return false;
}
}
