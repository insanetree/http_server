#include "http_server.h"
#include <cerrno>
#include <system_error>

using namespace insanetree;

http_server::http_server(uint16_t port, int32_t backlog)
{
    int ret;
    ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(ret == -1) {
        throw std::system_error(std::make_error_code(std::errc(errno)));
    }
}
