#include "http_connection.hpp"
#include "http_server.hpp"


int main(int argc, char **argv) {
    insanetree::http_server server{4200, 80};
    return 0;
}
