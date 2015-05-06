#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <string>
#include "OpCode.h"

class Packet
{
public:
    Packet() = delete;
    Packet(OpCode op, unsigned int size);
    Packet(const unsigned char* buffer, unsigned int size);

    /**
     * Get buffer
     * @return pointer to buffer of packet
     */
    const unsigned char* getBuffer();

    /**
     * Get copy of buffer
     * @return New copy of buffer
     */
    std::vector<unsigned char> getBufferCopy();

    /**
     * @return OpCode of packet
     */
    OpCode getOpCode();

    // Writing data to Packet
    Packet& operator<<(const unsigned int data);
    Packet& operator<<(const std::string &data);
private:
    unsigned int _size,_writePos;
    std::vector<unsigned char> _buffer;
};

#endif
