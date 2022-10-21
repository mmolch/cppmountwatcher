#include <algorithm>
#include <cassert>
#include <iostream>

#include <mmolch/Signal.hpp>

static int counter = 0;

int main(int argc, char const *argv[])
{
    auto s = mmolch::Signal<int>();
    auto listener1 = s.add([] (int i) { counter += i; });

    {
        s.raise(5);
        assert(counter == 5);

        s.raise(3);
        assert(counter == 8);

        auto listener2 = s.add([] (int i) { counter -= i*2; });

        s.raise(3);
        assert(counter == 5);
    }

    s.raise(4);
    assert(counter == 9);

    listener1.reset();

    s.raise(4);
    assert(counter == 9);

    return 0;
}
