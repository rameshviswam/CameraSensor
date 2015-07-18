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
    std::queue<char> mSendQueue;
    std::queue<char> mReceiveQueue;

    // socket member variables
    std::string mServerAddr;
    int mServerPort;
    SOCKET mSocket;

    // Threads and mutex member variables
    std::thread *t1;
    std::thread *t2;
    std::mutex *m;

private:
    //receive queue getters and setters
    void pushDataIntoReceiveQueue(const char * buf, const int size);

    //send queue
    int getSendQueueSize();
    void getDataFromSendQueue(char * sendBuffer, int size);

    //socket methods
    int initializeSocket() const;
    int createSocket();
    int connectToServer();

    void sendToServer();

    bool isSocketReadReady();
    void receiveFromServer();

public:
    TCPClientManager(std::string addr, int port);
    //TCPClientManager(TCPClientManager&&);
    ~TCPClientManager();
    int start();
    void sendData(unsigned char data);
    void sendData(unsigned char *data, int size);
    std::thread transmitThread();
};

#endif  //__TCP_CLIENT_MANAGER_H__