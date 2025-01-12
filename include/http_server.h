#ifndef _HTTP_SERVER_H_
#define _HTTP_SERVER_H_

#include <cstdint>

#include <sys/socket.h>
#include <netinet/in.h>

namespace insanetree
{
class http_server
{
public:
    explicit http_server(uint16_t port, int32_t backlog);
private:
    int m_fd;
    sockaddr_in m_sock;
};
}

#endif
