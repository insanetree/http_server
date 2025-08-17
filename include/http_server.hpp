#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include <cstddef>
#include <cstdint>
#include <array>
#include <memory>
#include <map>

#include <sys/socket.h>
#include <netinet/in.h>

namespace insanetree
{

class http_connection;

class http_server
{
public:
    explicit http_server(in_port_t port, int backlog);
private:
    int m_fd{};
    sockaddr_in m_sock{};

    static constexpr size_t max_connections = 1024;
    size_t open_connections_count = 0;
    std::map<int, std::shared_ptr<http_connection>> open_connections;
};
}

#endif
