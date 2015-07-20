#include <iostream>

#include "CommandManager.h"

//Constructor
CommandManager::CommandManager()
:tcpClientMgr(new TCPClientManager("127.0.0.1", 1456)) {
    isPacketLengthFound = false;
    tcpClientMgr->start();
    receiveThread();
}

CommandManager::~CommandManager() {
    delete(t3);
}


int CommandManager::extractMsgHeader(char *buf, int size) {
    int val = *reinterpret_cast<uint32_t *> (buf);
    return val;
}

void CommandManager::receiveThread()
{
    t3 = new std::thread(&CommandManager::parseReceivedPacket, this);
}
void CommandManager::parseReceivedPacket()
{

    while (1) {
        if (tcpClientMgr->checkDataReceivedFlag()) {
            if (isPacketLengthFound == false) {
                msgLength = 8;
                if (tcpClientMgr->getReceiveBufferSize() >= msgLength) {
                    char * buf = new char[msgLength];
                    tcpClientMgr->getReceivedData(buf, msgLength);
                    char tempBuf[4];
                    memcpy(tempBuf, buf, 4);
                    msgLength = extractMsgHeader(tempBuf, 4);
                    memcpy(tempBuf, buf + 4, 4);
                    msgId = extractMsgHeader(tempBuf, 4);

                    isPacketLengthFound = true;
                    delete(buf);
                }
            }
            else {
                if (tcpClientMgr->getReceiveBufferSize() >= msgLength){
                    char * buf = new char[msgLength];
                    tcpClientMgr->getReceivedData(buf, msgLength);
                    if (msgId == 2) {
                        for (int i = 0; i < msgLength; ++i) {
                            std::cout << buf[i];
                        }
                        std::cout << std::endl;

                    }
                    else if (msgId == 4) {

                    }
                    isPacketLengthFound = false;
                    delete(buf);
                }
            }

        }
    }

}

int CommandManager::submitPacket(MSGID id, unsigned char * buffer, int size) {
    unsigned int ID_SIZE = 1;
    if (id < MSGID::MSG_START || id >= MSGID::MSG_END) {
        return EXIT_FAILURE;
    }

    uint32_t packetSize = size;
    uint32_t packetId = id;

    tcpClientMgr->sendData(reinterpret_cast<unsigned char *> (&packetSize), 4);
    tcpClientMgr->sendData(reinterpret_cast<unsigned char *> (&packetId), 4);
    tcpClientMgr->sendData(buffer, size);

    return EXIT_SUCCESS;
}