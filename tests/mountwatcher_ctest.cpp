#include <mmolch/MountWatcher.hpp>

#include <iostream>
#include <exception>

int main(int argc, char const *argv[])
{
    /* code */

    mmolch::MountWatcher mw;

    try
    {
        mw.SetEnabled(true);
        //mw.SetEnabled(false);

    }
    catch (const std::exception& error)
    {
        std::cout << error.what() << std::endl;
    }

    return 0;
}
