#ifndef CONNECTION_MANAGER_H
#define CONNECTION_MANAGER_H

#include <set>
#include "Connection.h"

class ConnectionManager
{
public:
    ConnectionManager() { };
    void start(connectionPtr c);
    void stop(connectionPtr c);
    void stop_all();
private:
    std::set<connectionPtr> _connections;
};
#endif /* !CONNECTION_MANAGER_H */
