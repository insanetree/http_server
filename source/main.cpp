#include <iostream>
#include "http_server.h"

using namespace insanetree;

int main(int argc, char **argv) {
    std::cout << "Hello, world!" << std::endl;
    http_server server{4200, 80};
    return 0;
}
