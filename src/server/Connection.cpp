#include "Connection.h"
#include "ConnectionManager.h"
#include <boost/bind.hpp>


Connection::Connection(boost::asio::ip::tcp::socket socket, ConnectionManager& manager) : _socket(std::move(socket)),_connectionManager(manager),_connected(false),_state(WAIT_HANDSHAKE)
{

}

void Connection::start()
{
    _connected = true;
    Packet handshake(SMSG_HANDSHAKE_REQUEST,0);
    std::cout << "Client started: " << _socket.remote_endpoint().address().to_string() << std::endl;
    send(handshake);
    receive();
}

void Connection::send(Packet &packet)
{
    _socket.async_send(boost::asio::buffer(packet.getBufferCopy()), boost::bind(&Connection::handleSend, this, packet, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

void Connection::receive()
{
    _socket.async_receive(boost::asio::buffer(_buffer),boost::bind(&Connection::handleReceive, shared_from_this(),
                boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}
void Connection::handleReceive(size_t bytes, const boost::system::error_code& err)
{
    if(err)
        return;
    std::cout << "recv: " << bytes << std::endl;

}
void Connection::disconnect()
{
    std::cout << "Disconnect" << std::endl;
    _connected = false;
    stop();
}
void Connection::handleSend(Packet &packet, size_t bytes, const boost::system::error_code& err)
{
    if(err)
    {
        disconnect();
    }
}
void Connection::stop()
{
    _socket.close();
}
