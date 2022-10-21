// Taken from https://codereview.stackexchange.com/questions/230577/light-weight-c-signals-slots-or-event-system

#ifndef MMOLCH_SIGNAL_HPP
#define MMOLCH_SIGNAL_HPP

#include <forward_list>
#include <functional>
#include <memory>


namespace mmolch
{

template <class... FuncArgs>
class Signal
{   
public:
    using Function = std::function<void(FuncArgs...)>;
    using Listener = std::shared_ptr<Function>;

    [[nodiscard]]
    Listener add(Function cb)
    {
        auto const result = std::make_shared<Function>(std::move(cb));
        listeners.push_front(result);
        return result;
    }

    template<class... Args>
    void raise(Args&&... args)
    {
        listeners.remove_if([&args...](std::weak_ptr<Function> const& e)
        {
            if (auto f = e.lock())
            {
                (*f)(std::forward<Args>(args)...);
                return false;
            }
            return true;
        });
    }


private:

    std::forward_list<std::weak_ptr<Function>> listeners;
};

} // namespace mmolch

#endif // MMOLCH_SIGNAL_HPP
