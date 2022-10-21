#include <cerrno>
#include <cstring> // strerror
#include <stdexcept>
#include <iostream>

#include <fcntl.h> // open()

#include <mmolch/MountWatcher.hpp>


namespace mmolch
{

const int MountWatcher::m_pollingRequests_size = sizeof(m_pollingRequests) / sizeof(struct pollfd);

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

    m_pollingRequests.mounts.events = POLLERR | POLLPRI;
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

        if (m_pollingRequests.mounts.revents > 0)
        {
            //emit d_ptr->mountsChanged();
            std::cout << "Change" << std::endl;
        }

        m_pollingRequests.mounts.revents = 0;
    }
}

} // namespace mmolch
