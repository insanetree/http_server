#include "http_connection.hpp"

#include <cassert>
#include <cerrno>
#include <sys/socket.h>
#include <sys/types.h>
#include <system_error>

namespace insanetree {

http_connection::http_connection(int fd, sockaddr_in peer_addr) : m_fd(fd), m_peer_addr(peer_addr)
{}

void
http_connection::read_socket() {
    ssize_t ret;
    ret = ::recv(m_fd, m_message_buffer.data(), message_buffer_capacity, MSG_DONTWAIT);
    if(ret == -1) {
        throw std::system_error(std::make_error_code(std::errc(errno)));
    }
    m_message_buffer_size = ret;
}

int
http_connection::parse_buffer() {
    int ret;
    if(!m_scanner_state) {
        ret = yylex_init(&m_scanner_state);
        if(ret) {
            throw std::system_error(std::make_error_code(std::errc(errno)));
        }
    }
    if(!m_parser_state) {
        m_parser_state = yypstate_new();
        if(!m_parser_state) {
            throw std::system_error(std::make_error_code(std::errc::not_enough_memory));
        }
    }
    assert(m_message_buffer_size > 1);
    m_message_buffer[m_message_buffer_size++] = '\0';
    m_message_buffer[m_message_buffer_size++] = '\0';
    yy_scan_buffer(m_message_buffer.data(), m_message_buffer_size, m_scanner_state);

    YYSTYPE yylval;

    {
        ret = yylex(&yylval, m_scanner_state);
    }

    return 0;
}

} // namespace insanetree