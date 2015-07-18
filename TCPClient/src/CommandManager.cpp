#include "CommandManager.h"

//Constructor
CommandManager::CommandManager()
:tcpClientMgr(new TCPClientManager("127.0.0.1", 1456)) {
    tcpClientMgr->start();
}

int CommandManager::submitPacket(MSGID id, unsigned char * buffer, int size) {
    unsigned int ID_SIZE = 1;
    if (id < MSGID::MSG_START || id >= MSGID::MSG_END) {
        return EXIT_FAILURE;
    }

//    uint64_t packetSize = size;

    uint32_t packetSize = size;
    uint32_t packetId = id;
    

    tcpClientMgr->sendData(reinterpret_cast<unsigned char *> (&packetSize), 4);
    tcpClientMgr->sendData(reinterpret_cast<unsigned char *> (&packetId), 4);
    tcpClientMgr->sendData(buffer, size);

    return EXIT_SUCCESS;
}
