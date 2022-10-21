#include <mmolch/Pipe.hpp>

#include <array>
#include <cassert>
#include <cstring>
#include <iostream>


std::string_view testdata = "xxx Hello Pipes xxx";

int main(int argc, char const *argv[])
{
    // Create a pipe
    mmolch::Pipe p;

    // Write testdata into pipe
    p.Write(testdata);

    // Read from pipe
    std::array<char, 100> str {0};
    p.Read(str);

    // Check whether the read data matches the testdata
    assert(std::strlen(str.data()) == testdata.size());
    assert(std::strncmp(str.data(), testdata.data(), testdata.size()) == 0);
   
    return 0;
}
