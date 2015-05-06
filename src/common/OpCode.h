#ifndef OPCODE_H
#define OPCODE_H

#include <vector>
#include <string>

enum OpCode : unsigned char
{
    CMSG_HANDSHAKE_RESPONSE = 0,
    SMSG_HANDSHAKE_REQUEST,
    MAX_OPCODE
};


#endif
