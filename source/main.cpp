#include "http_connection.hpp"
#include "http_server.hpp"

using namespace insanetree;

int main(int argc, char **argv) {
    http_server server{4200, 80};
    return 0;
}
