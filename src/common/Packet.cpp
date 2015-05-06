#include "Packet.h"
#include <cstring>
#include <iostream>

#define OPCODE_LEN (sizeof(unsigned char))
#define PACKET_DATA_SIZE (sizeof(unsigned int))
#define PACKET_HEADER_SIZE OPCODE_LEN + PACKET_DATA_SIZE

Packet::Packet(OpCode op, unsigned int size) : _size(size),_writePos(PACKET_HEADER_SIZE)
{
    _buffer.resize(PACKET_HEADER_SIZE + _size);
    // First byte is Opcode
    _buffer[0] = op;
    memset(&_buffer[1],_size,PACKET_DATA_SIZE);
}

Packet::Packet(const unsigned char* buffer, unsigned int size) : _size(size),_writePos(size)
{
    _buffer.resize(_size);
    memcpy(&_buffer[0], buffer, _size);
}

OpCode Packet::getOpCode()
{
    return static_cast<OpCode>(_buffer[0]);
}

const unsigned char* Packet::getBuffer()
{
    return &_buffer[0];
}

std::vector<unsigned char> Packet::getBufferCopy()
{
    return _buffer;
}

Packet& Packet::operator<<(const unsigned int data)
{
    memcpy(&_buffer[0] + _writePos, &data, sizeof(data));
    _writePos += sizeof(data);
    return *this;
}

Packet& Packet::operator<<(const std::string &data)
{
    memcpy(&_buffer[0] + _writePos, data.data(), data.length() + 1);
    _writePos += data.length() + 1;
    return *this;
}
