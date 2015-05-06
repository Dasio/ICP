#include "ConnectionManager.h"

void ConnectionManager::start(connectionPtr c)
{
    _connections.insert(c);
    c->start();
}

void ConnectionManager::stop(connectionPtr c)
{
    _connections.erase(c);
    c->stop();
}

void ConnectionManager::stop_all()
{
    for (auto c: _connections)
        c->stop();
    _connections.clear();
}
