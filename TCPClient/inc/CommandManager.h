#ifndef __COMMANDMANAGER_H__
#define  __COMMANDMANAGER_H__

#include "message.h"
#include <string>
#include "TCPClientManager.h"

class CommandManager {
private:
    TCPClientManager * tcpClientMgr;

public:
    CommandManager();

    int submitPacket(MSGID id, unsigned char * buffer, int size);
    int submitPacket(MSGID id);
};
#endif  //__COMMANDMANAGER_H__