#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include "http_connection.h"
#include <cstdint>

#include <sys/socket.h>
#include <netinet/in.h>

namespace insanetree
{
class http_server
{
public:
    explicit http_server(in_port_t port, int backlog);
    http_connection accept();
private:
    int m_fd{};
    sockaddr_in m_sock{};
};
}

#endif
