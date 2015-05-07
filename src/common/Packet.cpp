#include "Packet.h"
#include "../common/MyExc.h"
#include <cstring>
#include <iostream>


Packet::Packet(OpCode op, unsigned int size) : _size(size),_writePos(PACKET_HEADER_SIZE),_readPos(PACKET_HEADER_SIZE)
{
    _buffer.resize(PACKET_HEADER_SIZE + _size);
    // First byte is Opcode
    _buffer[0] = op;
    memset(&_buffer[1],_size,PACKET_DATA_SIZE);
}

Packet::Packet(const unsigned char* buffer, unsigned int size) : _size(size),_writePos(size),_readPos(PACKET_HEADER_SIZE)
{
    _buffer.resize(_size);
    memcpy(&_buffer[0], buffer, _size);
}
void Packet::appendBuffer(unsigned char* buffer,int size)
{
    if(_size < size + _writePos)
        throw MyExc("Packet::appendBuffer");
    memcpy(&_buffer[_writePos], buffer, size);
    _writePos += size;
}
unsigned int Packet::getSize()
{
    return _size;
}

unsigned int Packet::getDataSize()
{
    //CHECK! Propably wrong
    return static_cast<unsigned int>(_buffer[1]);
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
    memcpy(&_buffer[_writePos], &data, sizeof(data));
    _writePos += sizeof(data);
    return *this;
}

Packet& Packet::operator<<(const std::string &data)
{
    memcpy(&_buffer[_writePos], data.data(), data.length() + 1);
    _writePos += data.length() + 1;
    return *this;
}

Packet& Packet::operator>>(unsigned int& data)
{
    data = *((unsigned int*)&_buffer[_readPos]);
    _readPos += sizeof(data);
    return *this;
}
