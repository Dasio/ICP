#ifndef CONNECTION_H
#define CONNECTION_H

#include <boost/asio.hpp>
#include "../common/Packet.h"
#include "../common/MyExc.h"

class ConnectionManager;

class Connection: public std::enable_shared_from_this<Connection>
{
public:

    Connection(boost::asio::ip::tcp::socket socket, ConnectionManager& manager);
    void start();
    void stop();
private:

    void send(Packet &packet);
    void handleSend(Packet &packet, size_t bytes, const boost::system::error_code& err);
    void receive();
    void handleReceive(size_t bytes, const boost::system::error_code &err);
    void disconnect();
    void processPacket();
    boost::asio::ip::tcp::socket _socket;
    ConnectionManager& _connectionManager;
    Packet* _packet;
    bool _connected;
    enum State
    {
        WAIT_HANDSHAKE,
        OK
    } _state;
    std::array<unsigned char, 8192> _buffer;
};
typedef std::shared_ptr<Connection> connectionPtr;
#endif
