#include <cerrno>
#include <cstring> // strerror
#include <stdexcept>
#include <iostream>

#include <fcntl.h> // open()
#include <mntent.h>

#include <mmolch/MountWatcher.hpp>


namespace mmolch
{

const int MountWatcher::m_pollingRequests_size = sizeof(m_pollingRequests) / sizeof(struct pollfd);

static const std::vector<std::string> GetMounts()
{
    std::vector<std::string> mountEntries;
    struct mntent *current;

    FILE *proc_mounts = setmntent("/proc/mounts", "r");

    while ((current = getmntent(proc_mounts)) != NULL)
    {
        mountEntries.push_back(current->mnt_dir);
    }
    fclose(proc_mounts);

    return mountEntries;
}

MountWatcher::MountWatcher()
    : m_pipe()
    , m_isEnabled(false)
    , m_watcherThread(nullptr)
{
    m_pollingRequests.mounts.fd = open("/proc/mounts", O_RDONLY, 0);
    if (m_pollingRequests.mounts.fd == -1)
    {
        int err = errno;
        throw std::runtime_error { std::string("Failed to create MountWatcher: ") + std::strerror(err) };
    }

    m_pollingRequests.mounts.events = 0&0;
    m_pollingRequests.mounts.revents = 0;

    m_pollingRequests.pipe.fd = m_pipe.GetReadFd();
    m_pollingRequests.pipe.events = POLLIN;
    m_pollingRequests.pipe.revents = 0;
}

MountWatcher::~MountWatcher()
{
    SetEnabled(false);
    close(m_pollingRequests.mounts.fd);
}

void MountWatcher::SetEnabled(bool enabled)
{
    if (m_isEnabled == enabled)
    {
        return;
    }

    m_isEnabled = enabled;

    if (enabled)
    {
        m_mounts = GetMounts();

        m_pollingRequests.mounts.revents = 0;
        m_pollingRequests.pipe.revents = 0;
        
        m_watcherThread = std::make_unique<std::thread>(&MountWatcher::Watch, this);
    }
    else
    {
        m_pipe.Write("x");
        m_watcherThread->join();
        m_watcherThread.reset(nullptr);
    }
}


bool MountWatcher::IsEnabled() const
{
    return m_isEnabled;
}

Signal<const std::string&>::Listener MountWatcher::OnMountAdded(std::function<void(const std::string&)> cb)
{
    return m_mountAddedSignal.connect(std::move(cb));
}

Signal<const std::string&>::Listener MountWatcher::OnMountRemoved(std::function<void(const std::string&)> cb)
{
    return m_mountRemovedSignal.connect(std::move(cb));
}

void MountWatcher::Update()
{
    const std::vector<std::string> currentMounts = GetMounts();
    std::vector<std::string> addedMounts;
    std::vector<std::string> removedMounts;

    std::set_difference(currentMounts.begin(), currentMounts.end(),
                        m_mounts.begin(), m_mounts.end(),
                        std::inserter(addedMounts, addedMounts.begin()));

    std::set_difference(m_mounts.begin(), m_mounts.end(),
                        currentMounts.begin(), currentMounts.end(),
                        std::inserter(removedMounts, removedMounts.begin()));


    std::for_each(removedMounts.begin(), removedMounts.end(), [this](const std::string& mountDir)
    {
        m_mountRemovedSignal.raise(mountDir);
    });

    std::for_each(addedMounts.begin(), addedMounts.end(), [this](const std::string& mountDir)
    {
        m_mountAddedSignal.raise(mountDir);
    });

    m_mounts = std::move(currentMounts);
}

void MountWatcher::Watch()
{
    constexpr const static int NO_TIMEOUT = -1;
    constexpr auto no_poll_error = [](int result) { return result != -1; };

    while( no_poll_error( poll( reinterpret_cast<struct pollfd*>(&m_pollingRequests),
                                m_pollingRequests_size,
                                NO_TIMEOUT ) ) )
    {
        if (m_pollingRequests.pipe.revents & POLLIN)
        {
            break;
        }

        if (m_pollingRequests.mounts.revents != 0)
        {
            Update();
        }

        m_pollingRequests.mounts.revents = 0;
    }
}

} // namespace mmolch
