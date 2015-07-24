#include <iostream>
#include <fstream>

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

void processHeartBeatResponse(char * buf, int msgLength) {
    for (int i = 0; i < msgLength; ++i) {
        std::cout << buf[i];
    }
    std::cout << std::endl;
}

void processSnapshotResponse(char * buf, int msgLength){
    std::cout << "RV..." << "SNAPSHOT" << std::endl;
    std::ofstream outFile;
    outFile.open("c:\\iWork\\test.jpg", std::ios::binary);
    outFile.write(buf, msgLength);
    outFile.close();
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
                    if (msgId == MSGID::HEARTBEAT_ACK) {
                        processHeartBeatResponse(buf, msgLength);
                    }
                    else if (msgId == MSGID::SNAPSHOT_RESPONSE) {
                        processSnapshotResponse(buf, msgLength);
                    }
                    isPacketLengthFound = false;
                    delete(buf);
                }
            }

        }
    }

}

int CommandManager::submitPacket(MSGID id) {
    unsigned int ID_SIZE = 1;
    if (id < MSGID::MSG_START || id >= MSGID::MSG_END) {
        return EXIT_FAILURE;
    }

    uint32_t packetSize = 0;
    uint32_t packetId = id;

    tcpClientMgr->sendData(reinterpret_cast<unsigned char *> (&packetSize), 4);
    tcpClientMgr->sendData(reinterpret_cast<unsigned char *> (&packetId), 4);
    
    return EXIT_SUCCESS;
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