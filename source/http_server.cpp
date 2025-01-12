#include "http_server.h"
#include <cerrno>
#include <system_error>

using namespace insanetree;

http_server::http_server(in_port_t port, int backlog)
{
    int ret;
    ret = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ret == -1) {
        throw std::system_error(std::make_error_code(std::errc(errno)));
    }
    m_fd = ret;
    m_sock.sin_family = AF_INET;
    m_sock.sin_addr.s_addr = INADDR_ANY;
    m_sock.sin_port = port;
    ret = ::bind(m_fd, (sockaddr*)&m_sock, sizeof(m_sock));
    if(ret == -1) {
        throw std::system_error(std::make_error_code(std::errc(errno)));
    }
    ret = ::listen(m_fd, backlog);
    if(ret == -1) {
        throw std::system_error(std::make_error_code(std::errc(errno)));
    }
}
