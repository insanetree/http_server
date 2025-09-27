#ifndef _HTTP_CONNECTION_H_
#define _HTTP_CONNECTION_H_

#include <cstddef>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <array>
#include <memory>

#include "http_request.hpp"
#include "http_parser.hpp"
#include "http_lexer.hpp"

namespace insanetree
{
class http_server;
class http_connection
{
public:
    static constexpr size_t message_buffer_capacity = YY_BUF_SIZE;

private:
    friend class http_server;

    http_connection(int fd, sockaddr_in peer_addr);
    ~http_connection();

    void read_socket();
    int parse_buffer();

    int m_fd;
    sockaddr_in m_peer_addr;
    std::array<char, message_buffer_capacity+2> m_message_buffer;
    size_t m_message_buffer_size = 0;

    yyscan_t m_scanner_state = nullptr;
    yypstate* m_parser_state = nullptr;

    std::unique_ptr<http_request> m_http_request = nullptr;
};
}

#endif
