#include <iostream>

#include "CommandManager.h"
#include "message.h"
#include <TCPClientManager.h>


void little_sleep(std::chrono::microseconds us)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + us;
    do {
        std::this_thread::yield();
    } while (std::chrono::high_resolution_clock::now() < end);
}




int main() {
    CommandManager cmdMgr;
    unsigned char arr[] = { 'P', 'I', 'N', 'G' };

    int i = 0;

    while (1) {
        std::cout << "enter 1 or 2: " << std::endl;
        std::cin >> i;

        if (i == 1) {
            cmdMgr.submitPacket(MSGID::HEARTBEAT_REQ, arr, 4);

        }
        else if (i == 2) {
            cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);

        }
    }

    //for (int i = 0; i < 20; ++i) {
    //   // cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    //    cmdMgr.submitPacket(MSGID::HEARTBEAT_REQ, arr, 4);
    //}
 
   // cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
  /*  cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
  */ // cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    /*cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    *///cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
 

    //cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    //for (int i = 0; i < 200; ++i) {
    //    cmdMgr.submitPacket(MSGID::HEARTBEAT_REQ, arr, 4);
    //}

    //for (int i = 0; i < 2000; ++i) {
    //    cmdMgr.submitPacket(MSGID::HEARTBEAT_REQ, arr, 4);
    //}
    cmdMgr.submitPacket(MSGID::HEARTBEAT_REQ, arr, 4);
   cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
   cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
   cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
   cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    cmdMgr.submitPacket(MSGID::HEARTBEAT_REQ, arr, 4);
    cmdMgr.submitPacket(MSGID::HEARTBEAT_REQ, arr, 4);
    cmdMgr.submitPacket(MSGID::HEARTBEAT_REQ, arr, 4);

    
    //    std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    //little_sleep(std::chrono::milliseconds(100));
//    cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    //cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    //cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);

    //    cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
   // cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    //cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    //cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    //cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);



    while (1) {
      //  cmdMgr.submitPacket(MSGID::HEARTBEAT_REQ, arr, 4);
        //cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST);
    };

    std::cout << "state 2" << std::endl;
    return 0;




}