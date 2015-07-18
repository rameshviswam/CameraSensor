#include <iostream>

#include "CommandManager.h"
#include "message.h"
#include <TCPClientManager.h>

int main() {
    CommandManager cmdMgr;
    unsigned char arr[] = { 'P', 'I', 'N', 'G' };


//    cmdMgr.start();

    cmdMgr.submitPacket(MSGID::HEARTBEAT_REQ, arr, 4);
    cmdMgr.submitPacket(MSGID::HEARTBEAT_REQ, arr, 4);
    //std::cout << "state 1" << std::endl;

    while (1) {
//       cmdMgr.submitPacket(MSGID::HEARTBEAT_REQ, arr, 4);
    }
    
    std::cout << "state 2" << std::endl;
    return 0;



    //TCPClientManager testClient("127.0.0.1", 1456);
    //unsigned char arr[] = { 'P', 'I', 'N', 'G' };

    //testClient.start();

    //std::cout << "state 1" << std::endl;

    //while (1) {
    //    testClient.sendData(arr, 4);
    //}

    //std::cout << "state 2" << std::endl;

}