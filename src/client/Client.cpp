#include "Client.h"
#include "../common/Packet.h"
#include <boost/bind.hpp>

Client::Client(const std::string &hostname, int port) : _io_service(), _socket(_io_service)
{
    boost::asio::ip::tcp::resolver::query query(hostname, std::to_string(port));
    boost::asio::ip::tcp::resolver resolver(_io_service);
    resolver.async_resolve(query,boost::bind(&Client::resolve, this, boost::asio::placeholders::error,boost::asio::placeholders::iterator));
    _io_service.run();
}

void Client::resolve(const boost::system::error_code &err, boost::asio::ip::tcp::resolver::iterator ei)
{
    if(!err)
    {
        boost::asio::ip::tcp::endpoint endpoint = *ei;
        _socket.async_connect(endpoint,
            boost::bind(&Client::handleConnect, this, boost::asio::placeholders::error, ++ei));
    }
    else
        std::cerr << "Failed to resolve query" << std::endl;

}

void Client::handleConnect(const boost::system::error_code &err, boost::asio::ip::tcp::resolver::iterator ei)
{
    if(!err)
    {
        std::cout << "Connected" << std::endl;
        // Send handshake
        //boost::asio::async_send(_socket,)
        //boost::asio::async_write(socket,request,
          //  boost::bind(&Client::read_response, this, boost::asio::placeholders::error, msg));
    }
    else if (ei != boost::asio::ip::tcp::resolver::iterator())
    {
        // Connection failed
        _socket.close();
        boost::asio::ip::tcp::endpoint ep = *ei;
        _socket.async_connect(ep,
            boost::bind(&Client::handleConnect, this, boost::asio::placeholders::error, ++ei));
    }
}
