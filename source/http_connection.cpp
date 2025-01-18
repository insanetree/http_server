#include "http_connection.h"

using namespace insanetree;

http_connection::http_connection(int fd, sockaddr_in peer_addr) : m_fd(fd), m_peer_addr(peer_addr)
{}
