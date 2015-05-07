#include "Client.h"
#include <boost/bind.hpp>

Client::Client(const std::string &hostname, int port) : _ioService(), _socket(_ioService),_packet(nullptr)
{
    boost::asio::ip::tcp::resolver::query query(hostname, std::to_string(port));
    boost::asio::ip::tcp::resolver resolver(_ioService);
    resolver.async_resolve(query,boost::bind(&Client::resolve, this, boost::asio::placeholders::error,boost::asio::placeholders::iterator));
    _ioService.run();
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
        throw MyExc("Client::resolve: Failed to resolve query");

}
void Client::receive()
{
    _socket.async_receive(boost::asio::buffer(_buffer),boost::bind(&Client::handleReceive, this, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}
void Client::handleReceive(size_t bytes, const boost::system::error_code &err)
{
    if(err)
        throw MyExc("Client::handleReceive");
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
        // Packet cant be completed without header
        size = _packet->getSize();
        if(size < PACKET_HEADER_SIZE)
        {
            remainingBytes -= size;
            readPos += size;
            continue;
        }
        // Need to shrink buffer, contains also data from next packet
        if(_packet->getSize() > _packet->getDataSize() + PACKET_HEADER_SIZE)
        {
            _packet->shrink();
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

void Client::handleConnect(const boost::system::error_code &err, boost::asio::ip::tcp::resolver::iterator ei)
{
    if(!err)
    {
        std::cout << "Connected" << std::endl;
        Packet packet(CMSG_HANDSHAKE_REQUEST, 4);
        packet << 42;
        send(packet);
        receive();
    }
    else if (ei != boost::asio::ip::tcp::resolver::iterator())
    {
        // Connection failed
        _socket.close();
        boost::asio::ip::tcp::endpoint ep = *ei;
        _socket.async_connect(ep,
            boost::bind(&Client::handleConnect, this, boost::asio::placeholders::error, ++ei));
    }
    else
        throw MyExc("Client::handleConnect - failed to connect");
}
void Client::handleSend(Packet /*&packet*/, size_t /*bytes*/, const boost::system::error_code& /*err*/)
{

}
void Client::send(Packet &packet)
{
    _socket.async_send(boost::asio::buffer(packet.getBufferCopy()), boost::bind(&Client::handleSend, this, packet, boost::asio::placeholders::bytes_transferred, boost::asio::placeholders::error));
}

void Client::processPacket()
{
    OpCode op = _packet->getOpCode();
    if (op == SMSG_HANDSHAKE_RESPONSE)
    {
        std::cout << "SMSG_HANDSHAKE_RESPONSE" << std::endl;
        unsigned int success;
        *_packet >> success;
        if(!success)
            throw MyExc("Client::processPacket: Handshake failed");

    }
    else
        throw MyExc("Connection::processPacket: Unknown packet");
}

