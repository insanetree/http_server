#include "http_connection.hpp"

#include <cerrno>
#include <sys/socket.h>
#include <sys/types.h>
#include <system_error>

#include "http_request.hpp"

using namespace insanetree;

http_connection::http_connection(int fd, sockaddr_in peer_addr) : m_fd(fd), m_peer_addr(peer_addr)
{}

http_request
http_connection::receive()
{

    return http_request{};
}

char
http_connection::read_byte()
{
    ssize_t ret;
    if (m_message_buffer_size == 0) {
        ret = ::recv(m_fd, m_message_buffer.data(), message_buffer_capacity, MSG_DONTWAIT);
        if(ret == -1) {
            throw std::system_error(std::make_error_code(std::errc(errno)));
        }
        m_message_buffer_size = static_cast<size_t>(ret);
        m_message_buffer_index = 0;
    }
    m_message_buffer_size--;
    return m_message_buffer[m_message_buffer_index++];
}
