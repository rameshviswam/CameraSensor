#include <iostream>

#include "CommandManager.h"
#include "message.h"
#include <TCPClientManager.h>

int main() {
    CommandManager cmdMgr;
    unsigned char arr[] = { 'P', 'I', 'N', 'G' };

    for (int i = 0; i < 20; ++i) {
        cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
        cmdMgr.submitPacket(MSGID::HEARTBEAT_REQ, arr, 4);
    }
 
    cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
 
    while (1);

    std::cout << "state 2" << std::endl;
    return 0;




}