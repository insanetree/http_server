#include "http_connection.hpp"

#include <cassert>
#include <cerrno>
#include <cstring>
#include <expected>
#include <memory>
#include <sys/socket.h>
#include <sys/types.h>
#include <system_error>

#include "http_lexer.hpp"
#include "http_parser.hpp"
#include "http_request.hpp"

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
    ret = yylex_init(&m_scanner_state);
    if (ret) {
        throw std::system_error(std::make_error_code(std::errc(errno)));
    }
    m_current_state = connection_state_e::READY_TO_READ;
}

std::expected<char, std::errc>
http_connection::read_byte()
{
    ssize_t ret;
    if (m_recv_idx == m_recv_size) {
        ret = ::recv(
          m_fd, m_recv_buffer.data(), m_recv_buffer.size(), MSG_DONTWAIT);
        if (ret == -1) {
            return std::unexpected<std::errc>{ std::errc{ errno } };
        }
        if (ret == 0) {
            return std::unexpected<std::errc>{ std::errc::connection_aborted };
        }
        assert(ret > 0);
        m_recv_idx = 0;
        m_recv_size = ret;
    }
    return { m_recv_buffer[m_recv_idx++] };
}

std::expected<void, std::errc>
http_connection::read_socket()
{
    assert(m_current_state == connection_state_e::READY_TO_READ);

    while (true) {
        auto r = read_byte();
        if (!r) {
            auto ec = r.error();

            // transient: no state change
            if (ec == std::errc::resource_unavailable_try_again
#if EWOULDBLOCK != EAGAIN
                || ec == std::errc::operation_would_block
#endif
            ) {
                return std::unexpected{ ec };
            }

            // permanent failure
            m_current_state = connection_state_e::ERROR;
            return std::unexpected{ ec };
        }

        // got a byte
        char c = *r;
        m_message_buffer[m_message_buffer_size++] = c;

        // check end-of-request (only valid if >= 4 bytes)
        if (m_message_buffer_size >= 4 &&
            memcmp(m_message_buffer.data() + m_message_buffer_size - 4,
                   "\r\n\r\n",
                   4) == 0) {
            m_current_state = connection_state_e::READY_TO_PARSE;
            return {};
        }

        // overflow
        if (m_message_buffer_size == message_buffer_capacity) {
            m_current_state = connection_state_e::ERROR;
            return std::unexpected{ std::errc::message_size };
        }
    }
}

std::expected<void, std::errc>
http_connection::parse_buffer()
{
    assert(m_current_state == connection_state_e::READY_TO_PARSE);
    int ret;

    if (!m_http_request) {
        m_http_request = std::make_unique<http_request>();
    }

    if (!m_buffer_state) {
        m_message_buffer[m_message_buffer_size] = '\0';
        m_message_buffer[m_message_buffer_size + 1] = '\0';
        m_buffer_state = yy_scan_buffer(
          m_message_buffer.data(), m_message_buffer_size + 2, m_scanner_state);
    }

    ret = yyparse(m_scanner_state, m_http_request.get());
    if (ret == 1) {
        m_current_state = connection_state_e::ERROR;
        return std::unexpected{ std::errc::invalid_argument };
    }
    if (ret == 2) {
        m_current_state = connection_state_e::ERROR;
        return std::unexpected{ std::errc::not_enough_memory };
    }
    m_current_state = connection_state_e::REQUEST_READY;
    return {};
}

std::unique_ptr<http_request>
http_connection::get_request()
{
    assert(m_current_state == connection_state_e::REQUEST_READY);
    m_current_state = connection_state_e::AWAITING_RESPONSE;
    return std::move(m_http_request);
}

} // namespace insanetree