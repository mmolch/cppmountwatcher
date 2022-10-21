# cppmountwatcher
A C++ class that uses the Linux kernel's events to watch for changes to /proc/mounts and signals changes


## Usage
    #include <mmolch/MountWatcher.hpp>
    #include <iostream>

    int main(int argc, char* argv[])
    {
        mmolch::MountWatcher mountwatcher;
        mountwatcher.SetEnabled(true);

        auto addedListener = mountwatcher.OnMountAdded([](const std::string& mountDir)
        {
            std::cout << "Mount added: " << mountDir << std::endl;
        });

        auto removedListener = mountwatcher.OnMountRemoved([](const std::string& mountDir)
        {
            std::cout << "Mount removed: " << mountDir << std::endl;
        });


        std::string xxx;
        std::cin >> xxx;
    }


## TODO
 * Maybe add some (optional) g_volume stuff to also catch gvfs mount changes

