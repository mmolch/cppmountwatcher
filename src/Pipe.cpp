#include <iostream>
#include <stdexcept>

#include <cerrno> // errno
#include <cstring> // strerror

#include <mmolch/Pipe.hpp>


namespace mmolch
{

Pipe::Pipe()
{
    // On success, zero is returned. On error, -1 is returned, errno is set appropriately, and pipefd is left unchanged.
    int result = pipe(reinterpret_cast<int*>(&m_pipe));
    if (result != 0)
    {
        int err = errno;
        throw std::runtime_error { std::string("Failed to create a pipe: ") + std::strerror(err) };
    }
}

Pipe::~Pipe()
{
    // Won't be called if constructor throws an exception
    close(m_pipe.readFd);
    close(m_pipe.writeFd);
}

ssize_t Pipe::Read(std::span<char> buffer) const
{
    return read(m_pipe.readFd, buffer.data(), buffer.size());
}

ssize_t Pipe::Write(std::span<const char> buffer) const
{
    return write(m_pipe.writeFd, buffer.data(), buffer.size());
}

int Pipe::GetReadFd() const
{
    return m_pipe.readFd;
}

int Pipe::GetWriteFd() const
{
    return m_pipe.writeFd;
}

} // namespace mmolch
