#ifndef _HTTP_CONNECTION_H_
#define _HTTP_CONNECTION_H_

#include <array>
#include <cstddef>
#include <memory>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "http_parser.hpp"

#include "http_lexer.hpp"

#include "http_request.hpp"

namespace insanetree {
class http_server;
class http_connection
{
public:
    enum class connection_state_e
    {
        UNINITIALIZED,
        READY_TO_READ,
        READY_TO_PARSE,
        REQUEST_READY,
        AWAITING_RESPONSE
    };
    static constexpr size_t message_buffer_capacity = YY_BUF_SIZE;

    http_connection(int fd, sockaddr_in peer_addr);
    ~http_connection();

    connection_state_e get_state() const;

    void initialize();
    void read_socket();
    int parse_buffer();

private:
    friend class http_server;

    int m_fd;
    sockaddr_in m_peer_addr;
    std::array<char, message_buffer_capacity + 2> m_message_buffer;
    size_t m_message_buffer_size = 0;
    yyscan_t m_scanner_state = nullptr;
    YY_BUFFER_STATE m_buffer_state = nullptr;
    yypstate* m_parser_state = nullptr;
    std::unique_ptr<http_request> m_http_request = nullptr;
    connection_state_e m_current_state;
};
}

#endif
