#include "http_connection.hpp"
#include "http_parser.hpp"

#include <cassert>
#include <cerrno>
#include <sys/socket.h>
#include <sys/types.h>
#include <system_error>

namespace insanetree {

http_connection::http_connection(int fd, sockaddr_in peer_addr)
  : m_fd(fd)
  , m_peer_addr(peer_addr)
  , m_current_state(connection_state_e::UNINITIALIZED)
{
}

http_connection::~http_connection() {}

http_connection::connection_state_e
http_connection::get_state() const
{
    return m_current_state;
}

void
http_connection::initialize()
{
    assert(m_current_state == connection_state_e::UNINITIALIZED);
    int ret;
    ret = yylex_init(&m_scanner_state);
    if (ret) {
        throw std::system_error(std::make_error_code(std::errc(errno)));
    }
    m_parser_state = yypstate_new();
    if (!m_parser_state) {
        throw std::system_error(
          std::make_error_code((std::errc::not_enough_memory)));
    }
    m_current_state = connection_state_e::READY_TO_READ;
}

void
http_connection::read_socket()
{
    assert(m_current_state == connection_state_e::READY_TO_READ);
    ssize_t ret;
    ret = ::recv(
      m_fd, m_message_buffer.data(), message_buffer_capacity, MSG_DONTWAIT);
    if (ret == -1) {
        throw std::system_error(std::make_error_code(std::errc(errno)));
    }
    m_message_buffer_size = ret;
}

int
http_connection::parse_buffer()
{
    assert(m_current_state == connection_state_e::READY ||
           m_current_state == connection_state_e::RECEIVING_REQUEST);

    int ret;
    assert(m_message_buffer_size > 1);
    m_message_buffer[m_message_buffer_size++] = '\0';
    m_message_buffer[m_message_buffer_size++] = '\0';
    yy_scan_buffer(
      m_message_buffer.data(), m_message_buffer_size, m_scanner_state);

    YYSTYPE yylval;

    do {
        ret = yylex(&yylval, m_scanner_state);

    } while (true);

    return 0;
}

} // namespace insanetree