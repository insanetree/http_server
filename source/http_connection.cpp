#include "http_connection.hpp"
#include "http_request.hpp"

#include <cassert>
#include <cerrno>
#include <memory>
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
    if (m_scanner_state) {
        yylex_destroy(m_scanner_state);
        m_scanner_state = nullptr;
        m_buffer_state = nullptr;
    }
    if (m_parser_state) {
        free(m_parser_state);
        m_parser_state = nullptr;
    }
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
    assert(ret > 1);
    m_message_buffer_size = ret;
    m_message_buffer[m_message_buffer_size] = '\0';
    m_message_buffer[m_message_buffer_size + 1] = '\0';
    m_buffer_state = yy_scan_buffer(
      m_message_buffer.data(), m_message_buffer_size + 2, m_scanner_state);
    assert(m_buffer_state);
    m_current_state = connection_state_e::READY_TO_PARSE;
}

void
http_connection::parse_buffer()
{
    assert(m_current_state == connection_state_e::READY_TO_PARSE);
    int ret;

    if(!m_http_request) {
        m_http_request = std::make_unique<http_request>();
    }

    YYSTYPE yylval;

    do {
        ret = yylex(&yylval, m_scanner_state);
        ret = yypush_parse(m_parser_state, ret, &yylval, m_http_request.get());
    } while (ret != 0);

    m_current_state = connection_state_e::REQUEST_READY;
}

std::unique_ptr<http_request>
http_connection::get_request() {
    assert(m_current_state == connection_state_e::REQUEST_READY);
    m_current_state = connection_state_e::AWAITING_RESPONSE;
    return std::move(m_http_request);
}

} // namespace insanetree