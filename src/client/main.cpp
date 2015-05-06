#include <iostream>
#include <cstdlib>

#include "Client.h"
int main(int argc, char **argv)
{
    if(argc < 3)
        return 1;
    int port;
    char *end;
    port = strtol(argv[2],&end,10);
    if(*end != '\0')
        return 1;
    std::cout << "Client: " << argv[1] << " port: " << port << std::endl;
    Client client(argv[1],port);
    return 0;
}
