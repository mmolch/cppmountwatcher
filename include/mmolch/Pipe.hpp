#ifndef MMOLCH_PIPE_HPP
#define MMOLCH_PIPE_HPP

#include <unistd.h> // ssize_t
#include <span>

namespace mmolch
{

class Pipe
{
public:
    Pipe();
    ~Pipe();

    ssize_t Read(std::span<char> buffer) const;
    ssize_t Write(std::span<const char> buffer) const;

    int GetReadFd() const;
    int GetWriteFd() const;


private:
    struct
    {
        int readFd;
        int writeFd;

    } m_pipe;
};

} // namespace mmolch

#endif // MMOLCH_PIPE_HPP
