#ifndef CLIENT_H
#define CLIENT_H

#include <boost/asio.hpp>
#include <queue>
#include "../common/Packet.h"
#include "../common/MyExc.h"

class Client
{
public:
    Client(const std::string &hostname, int port);
private:
    void processPacket();
    void resolve(const boost::system::error_code &err, boost::asio::ip::tcp::resolver::iterator ei);
    void handleConnect(const boost::system::error_code &err, boost::asio::ip::tcp::resolver::iterator ei);
    void receive();
    void handleReceive(size_t bytes, const boost::system::error_code &err);
    void send(Packet &packet);
    void handleSend(Packet /*&packet*/, size_t /*bytes*/, const boost::system::error_code& /*err*/);
    boost::asio::io_service _ioService;
    boost::asio::ip::tcp::socket _socket;
    Packet* _packet;
    std::array<unsigned char, 8192> _buffer;
};

#endif /* !CLIENT_H */
