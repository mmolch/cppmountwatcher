# cppmountwatcher
A C++ class that uses the Linux kernel's events to watch for changes to /proc/mounts and signals changes

## Usage
    #include <mmolch/MountWatcher.hpp>
    #include <iostream>

    int main(int argc, char* argv[])
    {
        mmolch::MountWatcher mountwatcher;
        auto listener = mountwatcher.OnMountsChanged([]()
        {
            std::cout << "Mounts changed" << std::endl;
        });

        mountwatcher.SetEnabled(true);

        std::string xxx;
        std::cin >> xxx;
    }

## TODO
 * Maybe add some (optional) g_volume stuff to also catch gvfs mount changes

