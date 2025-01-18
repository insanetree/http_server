#include <iostream>
#include "http_connection.h"
#include "http_server.h"

using namespace insanetree;

int main(int argc, char **argv) {
    http_server server{4200, 80};
    http_connection con = server.accept();
    return 0;
}
