#ifndef _HTTP_CONNECTION_H_
#define _HTTP_CONNECTION_H_

#include <netinet/in.h>
#include <netinet/ip.h>

namespace insanetree
{
class http_server;
class http_connection
{
public:
    http_connection(http_connection&&) = default;
private:
    friend class http_server;
    http_connection(int fd, sockaddr_in peer_addr);
    int m_fd;
    sockaddr_in m_peer_addr;
};
}

#endif
