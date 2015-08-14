## Contributing Guidelines

- Code must be formatted with the `hooks/formatAll.sh` script, before committing to the repository.
- We use [ArtisticStyle](http://astyle.sourceforge.net/) for code formatting.
	- Download the package from [SourceForge](http://sourceforge.net/projects/astyle)
	- Installation guideliens are provided [here](http://astyle.sourceforge.net/install.html)
- Code guide at a glance:

```cpp
#include <ros/ros.h>

float someFunction (int firstArgument, float secondArgument)
{
    return firstArgument + secondArgument;
}

int main ()
{
    int  i;
    int  count = 10;
    bool condition;

    if (condition)
    {
        std::cout << "Print a statement." << std::endl;
    }
    else
    {
        std::cout << "Print other statement." << std::endl;
    }

    for (i = 0; i < count; i++)
    {
        std::cout << "Index: " << i << endl;
    }

    switch (i)
    {
        case 1:
        {
            std::cout << "This was the first case." << std::endl;
            break;
        }

        case 2:
        {
            std::cout << "This was the second case." << std::endl;
            break;
        }

        default:
        {
            std::cout << "This was the default case." << std::endl;
            break;
        }
    }

    return 0;
}
```

- Check [this file](./vision_stack/task_buoy/src/buoy_server.cpp) to see more example code.
