#include "Connection.h"
#include "ConnectionManager.h"
#include <boost/bind.hpp>


Connection::Connection(boost::asio::ip::tcp::socket socket, ConnectionManager& manager) : _socket(std::move(socket)),_connectionManager(manager),_connected(false),_state(WAIT_HANDSHAKE)
{

}

void Connection::start()
{
    _connected = true;
    std::cout << "Client started: " << _socket.remote_endpoint().address().to_string() << std::endl;
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
    unsigned int readPos = 0;
    unsigned int size = 0;
    unsigned int remainingBytes = bytes;
    while(remainingBytes > 0)
    {
        if(_packet == nullptr)
        {
            _packet = new Packet(&_buffer[readPos], bytes - readPos);
        }
        else
        {
            _packet->appendBuffer(&_buffer[readPos], bytes - readPos);
        }
        size = _packet->getSize();
        remainingBytes -= size;
        readPos += size;
        // Packet cant be completed without header
        if(size < PACKET_HEADER_SIZE)
            continue;
        // Packet is complete
        if(size == _packet->getDataSize() + PACKET_HEADER_SIZE)
        {
             processPacket();
            _packet = nullptr;
        }
    }
    receive();

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
void Connection::processPacket()
{
    std::cout << "CMSG_HANDSHAKE_REQUEST " << std::endl;
    OpCode op = _packet->getOpCode();
    if(op == CMSG_HANDSHAKE_REQUEST)
    {
        unsigned int magic;
        *_packet >> magic;
        Packet response(SMSG_HANDSHAKE_RESPONSE, 4);
        if(magic == 42)
        {
            response << 1;
            _state = OK;
        }
        else
            response << 0;
        send(response);
        if(magic != 42)
            disconnect();
    }
    else
    {
        throw MyExc("Connection::processPacket: Unknown packet");
    }
}
void Connection::stop()
{
    _socket.close();
}
