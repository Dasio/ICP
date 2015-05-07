#ifndef PACKET_H
#define PACKET_H

#include <vector>
#include <string>
#include "OpCode.h"

#define OPCODE_LEN (sizeof(unsigned char))
#define PACKET_DATA_SIZE (sizeof(unsigned int))
#define PACKET_HEADER_SIZE (OPCODE_LEN + PACKET_DATA_SIZE)

class Packet
{
public:
    Packet() = delete;
    Packet(OpCode op, unsigned int size);
    Packet(const unsigned char* buffer, unsigned int size);

    /**
     * Append "size" bytes from buffer to packet buffer
     * @param buffer Buffer from where data will be read
     * @param size   How many bytes will be copied
     */
    void appendBuffer(unsigned char* buffer,int size);

    /**
     * @return Size of whole packet
     */
    unsigned int getSize();

    /**
     * @return Data length in bytes
     */
    unsigned int getDataSize();

    /**
     * Get buffer
     * @return pointer to buffer of packet
     */
    const unsigned char* getBuffer();

    /**
     * Shrink packet's buffer to fit dataSize
     */
    void shrink();

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
    // Reading data from Packet
    Packet& operator>>(unsigned int& data);
private:
    unsigned int _size,_writePos,_readPos;
    std::vector<unsigned char> _buffer;
};

#endif
