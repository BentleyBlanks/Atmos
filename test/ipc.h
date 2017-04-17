#pragma once
#include <core/messageQueue/a3Message.h>
#include <core/messageQueue/a3MessageQueueIPC.h>

void main()
{
    a3MessageQueueIPC ipc;

    ipc.init(L"Who's Your Daddy", false, 100, 1024);

    for(int i = 0; i < 100; i++)
    {
        c2sGridImageMessage msg;
        msg.shit = i;
        ipc.enqueue(msg);
    }

    getchar();
}