#ifndef OPCODE_H
#define OPCODE_H

#include <vector>
#include <string>

enum OpCode : unsigned char
{
    CMSG_HANDSHAKE_REQUEST = 0,
    SMSG_HANDSHAKE_RESPONSE,
    MAX_OPCODE
};


#endif
