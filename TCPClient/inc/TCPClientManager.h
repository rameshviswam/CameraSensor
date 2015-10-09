#ifndef __TCP_CLIENT_MANAGER_H__
#define __TCP_CLIENT_MANAGER_H__

#include <queue>        //std::queue
#include <string>       //std::string
#include <WinSock2.h>   // windows socket
#include <thread>
#include <mutex>

class TCPClientManager {
private:
    // packet queue data structures
   std::vector<char> mReceivedBuffer;

    // socket member variables
    std::string mServerAddr;
    int mServerPort;
    SOCKET mSocket;

    // Threads and mutex member variables
    std::thread *t1;
    std::thread *t2;
    std::mutex *m;

    //signals
    bool isDataReceived;

private:
    //receive queue getters and setters
    void pushDataIntoReceiveQueue(const char * buf, const int size);

    //socket methods
    int initializeSocket() const;
    int createSocket();
    int connectToServer();

    void sendToServer();

    bool isSocketReadReady();
    void receiveFromServer();

public:
    TCPClientManager(std::string addr, int port);
    ~TCPClientManager();

    int start();

    void sendData(const char *data, int size);
    int getReceiveBufferSize();
    void getReceivedData(char * buf, int size);
    bool checkDataReceivedFlag();
    void resetDataReceivedFlag();
};

#endif  //__TCP_CLIENT_MANAGER_H__