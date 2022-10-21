#ifndef MMOLCH_MOUNTWATCHER_HPP
#define MMOLCH_MOUNTWATCHER_HPP

#include <atomic>
#include <memory>
#include <thread>

#include <sys/poll.h>

#include <mmolch/Pipe.hpp>


namespace mmolch
{

class MountWatcher
{
public:
    MountWatcher();
    ~MountWatcher();

    void SetEnabled(bool enabled);
    bool IsEnabled() const;

    void SetCallback();    

private:
    void Update();
    void Watch();

    bool m_isEnabled;
    Pipe m_pipe; // Used to unblock when stopping to watch

    struct
    {
        struct pollfd mounts;
        struct pollfd pipe;

    } m_pollingRequests;
    static const int m_pollingRequests_size;

    std::unique_ptr<std::thread> m_watcherThread;
};

} // namespace mmolch

#endif // MMOLCH_MOUNTWATCHER_HPP
