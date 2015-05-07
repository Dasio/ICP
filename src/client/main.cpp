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
    try
    {
       Client client(argv[1],port);
    }
    catch (MyExc& exc)
    {
        std::cerr << exc.what() << std::endl;
        return 1;
    }
    return 0;
}
