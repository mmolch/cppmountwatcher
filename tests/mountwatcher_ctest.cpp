#include <mmolch/MountWatcher.hpp>

int main(int argc, char const *argv[])
{
    mmolch::MountWatcher mw;

    mw.SetEnabled(true);

    return 0;
}
