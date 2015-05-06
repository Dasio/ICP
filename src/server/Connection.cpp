#include "Connection.h"
#include "ConnectionManager.h"

Connection::Connection(boost::asio::ip::tcp::socket socket, ConnectionManager& manager) : _socket(std::move(socket)),_connectionManager(manager)
{

}

void Connection::start()
{
    std::cout << "Client started: " << _socket.remote_endpoint().address().to_string() << std::endl;
}

void Connection::stop()
{
    _socket.close();
}
