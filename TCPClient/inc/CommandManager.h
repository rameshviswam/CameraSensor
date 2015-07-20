#ifndef __COMMANDMANAGER_H__
#define  __COMMANDMANAGER_H__

#include <string>

#include "message.h"
#include "TCPClientManager.h"

class CommandManager {
private:
    TCPClientManager * tcpClientMgr;
    boolean isPacketLengthFound;
    int msgId;
    int msgLength;

    std::thread *t3;

private:
    void receiveThread();
    int extractMsgHeader(char *buf, int size);
public:
    CommandManager();
    ~CommandManager();

    int submitPacket(MSGID id, unsigned char * buffer, int size);
    int submitPacket(MSGID id);
    void parseReceivedPacket();
};
#endif  //__COMMANDMANAGER_H__