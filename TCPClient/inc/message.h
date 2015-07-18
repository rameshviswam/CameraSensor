#ifndef __MESSAGE_H__
#define __MESSAGE_H__

enum MSGID{
    MSG_START = 0,
    HEARTBEAT_REQ,
    HEARTBEAT_ACK,
    SNAPSHOT_REQ,
    SNAPSHOT_RES,
    MSG_END
};



#endif  //__MESSAGE_H__