#include "http_server.h"
#include <cerrno>
#include <netinet/in.h>
#include <sys/socket.h>
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
    m_sock.sin_port = htons(port);
    ret = ::bind(m_fd, (sockaddr*)&m_sock, sizeof(m_sock));
    if(ret == -1) {
        throw std::system_error(std::make_error_code(std::errc(errno)));
    }
    ret = ::listen(m_fd, backlog);
    if(ret == -1) {
        throw std::system_error(std::make_error_code(std::errc(errno)));
    }
}

http_connection
http_server::accept()
{
    int ret;
    sockaddr_in peer_addr{};
    socklen_t addr_len = sizeof(peer_addr);
    ret = ::accept(m_fd, (sockaddr*)&peer_addr, &addr_len);
    if(ret == -1) {
        throw std::system_error(std::make_error_code(std::errc(errno)));
    }
    return http_connection(ret, peer_addr);
}
