#pragma once
#include <core/messageQueue/a3MessageQueue.h>

enum C2S_MESSAGES
{
    C2S_MSG_GRIDIMAGE = 0,
    // --!in the future
    C2S_MSG_LIGHTPATH
};

enum S2C_MESSAGES
{
    S2C_MSG_INIT = 0
};

#define MESSAGE_DEFINE(MessageClass, MessageType)      \
	MessageClass()  { type = MessageType; }            \

// Server to Client
struct s2cInitMessage : public a3MessageEntryHead
{
    int fuck;
    MESSAGE_DEFINE(s2cInitMessage, S2C_MSG_INIT)
};

// Client to Server
struct c2sGridImageMessage : public a3MessageEntryHead
{
    int shit;
    MESSAGE_DEFINE(c2sGridImageMessage, C2S_MSG_GRIDIMAGE)
};