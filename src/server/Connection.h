#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio.hpp>

class ConnectionManager;

class Connection: public std::enable_shared_from_this<Connection>
{
public:
    Connection(boost::asio::ip::tcp::socket socket, ConnectionManager& manager);
    void start();
    void stop();
private:
    boost::asio::ip::tcp::socket _socket;
    ConnectionManager& _connectionManager;
};
typedef std::shared_ptr<Connection> connectionPtr;
#endif
