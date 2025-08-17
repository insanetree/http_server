#ifndef _HTTP_CONNECTION_H_
#define _HTTP_CONNECTION_H_

#include <cstddef>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <array>

#include "http_request.hpp"

namespace insanetree
{
class http_server;
class http_connection
{
public:
    static constexpr size_t message_buffer_capacity = 1024;
    http_connection(http_connection&&) = default;

    http_request receive();

private:
    friend class http_server;

    http_connection(int fd, sockaddr_in peer_addr);
    char read_byte();

    int m_fd;
    sockaddr_in m_peer_addr;
    std::array<char, message_buffer_capacity> m_message_buffer;
    size_t m_message_buffer_index = 0;
    size_t m_message_buffer_size = 0;
};
}

#endif
