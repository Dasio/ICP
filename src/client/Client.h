#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>

class Client
{
public:
    Client(const std::string &hostname, int port);
private:
    void resolve(const boost::system::error_code &err, boost::asio::ip::tcp::resolver::iterator ei);
    void handleConnect(const boost::system::error_code &err, boost::asio::ip::tcp::resolver::iterator ei);
    boost::asio::io_service _io_service;
    boost::asio::ip::tcp::socket _socket;
};

#endif /* !CLIENT_H */
