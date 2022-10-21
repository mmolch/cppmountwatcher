#ifndef MMOLCH_MOUNTWATCHER_HPP
#define MMOLCH_MOUNTWATCHER_HPP

#include <atomic>
#include <memory>
#include <string>
#include <thread>
#include <vector>

#include <sys/poll.h>

#include <mmolch/Pipe.hpp>
#include <mmolch/Signal.hpp>


namespace mmolch
{

class MountWatcher
{
public:
    MountWatcher();
    ~MountWatcher();

    void SetEnabled(bool enabled);
    bool IsEnabled() const;

    [[nodiscard]]
    Signal<const std::string&>::Listener OnMountAdded(std::function<void(const std::string&)> cb);

    [[nodiscard]]
    Signal<const std::string&>::Listener OnMountRemoved(std::function<void(const std::string&)> cb);


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
    Signal<const std::string&> m_mountAddedSignal;
    Signal<const std::string&> m_mountRemovedSignal;
    std::vector<std::string> m_mounts;
};

} // namespace mmolch

#endif // MMOLCH_MOUNTWATCHER_HPP
