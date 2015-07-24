#include "TCPClientManager.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono;

// Link with ws2_32.lib
#pragma comment(lib, "Ws2_32.lib")

TCPClientManager::TCPClientManager(std::string addr = "127.0.0.1", int port = 1456)
{
    this->mServerAddr = addr;
    this->mServerPort = port;
    this->mSocket = INVALID_SOCKET;
    m = new std::mutex();
    isDataReceived = false;
}

TCPClientManager::~TCPClientManager(){
    int iResult = shutdown(mSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        std::cerr << "shutdown failed with error: " << WSAGetLastError() << std::endl;
        closesocket(mSocket);
        WSACleanup();
    }
    closesocket(mSocket);
    delete(m);
    delete(t1);
    delete(t2);
}

void TCPClientManager::sendData(unsigned char data) {
    m->lock();
    mSendQueue.push(data);
    m->unlock();
}

void TCPClientManager::sendData(unsigned char * data, int size) {
    m->lock();
    for (int i = 0; i < size; ++i) {
        mSendQueue.push(data[i]);
    }
    m->unlock();
}

//Queue private methods
void TCPClientManager::pushDataIntoReceiveQueue(const char * data, const int size) {
    m->lock();
    for (int i = 0; i < size; ++i) {
        mReceiveQueue.push(data[i]);
    }
    m->unlock();
}


int TCPClientManager::getReceiveBufferSize() {
    return mReceiveQueue.size();
}

void TCPClientManager::getReceivedData(char * buf, int size) {
    if (size <= mReceiveQueue.size()) {
        m->lock();
        for (int i = 0; i < size; ++i) {
            buf[i] = mReceiveQueue.front();
            mReceiveQueue.pop();
        }
        m->unlock();
    }
}


int TCPClientManager::getSendQueueSize()  {
    m->lock();
    int size = mSendQueue.size();
    m->unlock();
    return size;
}

void TCPClientManager::getDataFromSendQueue(char * sendBuffer, int size) {
    m->lock();
    int i = 0;

    while (size > 0){
        sendBuffer[i] = mSendQueue.front();
        mSendQueue.pop();
        i++;
        size--;
    }

    m->unlock();
}

// Socket private methods
int TCPClientManager::initializeSocket() const {
    WSADATA wsaData;

    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR) {
        printf("WSAStartup failed: %d\n", iResult);
        return -1;
    }

    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
        std::cout << "Could not find a usable version of Winsock.dll" << std::endl;
        WSACleanup();
        return -1;
    }
    else {
        std::cout << "Winsock 2.2 Version ..............." << std::endl;
    }

    return 0;
}

int TCPClientManager::createSocket() {
    if (mSocket != INVALID_SOCKET) {
        std::cerr << "Socket already exist" << std::endl;
        return EXIT_SUCCESS;
    }

    mSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (INVALID_SOCKET == mSocket) {
        std::cerr << "Create Socket failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }

    return EXIT_SUCCESS;
}

int TCPClientManager::connectToServer() {
    int iResult = SOCKET_ERROR;
    struct sockaddr_in serverAddr;

    serverAddr.sin_addr.s_addr = inet_addr(mServerAddr.c_str());
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(mServerPort);

    if (INVALID_SOCKET == mSocket) {
        std::cerr << "Invalid Socket" << std::endl;
        return -1;
    }

    iResult = connect(mSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (SOCKET_ERROR == iResult) {
        closesocket(mSocket);
        std::cerr << "conection to server failed with error: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return -1;
    }
    return EXIT_SUCCESS;
}

void TCPClientManager::sendToServer() {
    int sentSize = 0;
    int iResult = 0;

    std::cout << "send thread" << std::endl;

    while (1) {
        int queueSize = getSendQueueSize();
        if (queueSize) {
            char * sendBuffer = new char[queueSize];
            getDataFromSendQueue(sendBuffer, queueSize);

            std::cout << "sending to server with payload size: " << queueSize << std::endl;

            iResult = send(mSocket, sendBuffer, (int)queueSize, 0);
            if (SOCKET_ERROR == iResult) {
                closesocket(mSocket);
                std::cerr << "Data send failed with error: " << WSAGetLastError() << std::endl;
                WSACleanup();
            }
            std::cout << "sent data size: " << iResult << std::endl;

            free(sendBuffer);
        }
        //std::this_thread::sleep_until(system_clock::now() + seconds(1));
        std::this_thread::yield();
    }
}

bool TCPClientManager::isSocketReadReady(){
    fd_set readSockSet;
    struct timeval timeout;

    timeout.tv_sec = 0;
    timeout.tv_usec = 10000;

    FD_ZERO(&readSockSet);
    FD_SET(mSocket, &readSockSet);
    if (select(1, &readSockSet, NULL, NULL, &timeout) >> 0) {
        return TRUE;
    }
    return FALSE;
}

bool TCPClientManager::checkDataReceivedFlag() {
    return isDataReceived;
}


void TCPClientManager::resetDataReceivedFlag() {
    isDataReceived = false;
}


void TCPClientManager::receiveFromServer() {
    int recv_size = 0;

    while (1) {

        if (!mReceiveQueue.empty()) {
            m->lock();
            isDataReceived = true;
            m->unlock();
        }

        if (isSocketReadReady()) {
            const int receiveBufLength = 1024;
            char *receiveBuffer = new char[receiveBufLength];
            memset(receiveBuffer, 0, receiveBufLength);

            recv_size = recv(mSocket, receiveBuffer, receiveBufLength, 0);
            if (recv_size > 0)
                std::cout << "Bytes received: " << recv_size << std::endl;
            else if (recv_size == 0)
                std::cerr << "Connection closed\n" << std::endl;
            else
                std::cerr << "recv failed with error: " << WSAGetLastError() << std::endl;

            pushDataIntoReceiveQueue(receiveBuffer, recv_size);


            free(receiveBuffer);
        }
        std::this_thread::yield();
    }
}

int TCPClientManager::start() {
    int iResult = -1;

    iResult = initializeSocket();
    if (EXIT_SUCCESS != iResult) {
        return iResult;
    }

    iResult = createSocket();
    if (EXIT_SUCCESS != iResult) {
        return iResult;
    }

    iResult = connectToServer();
    if (EXIT_SUCCESS != iResult) {
        return iResult;
    }

    t1 = new std::thread(&TCPClientManager::receiveFromServer, this);
    t2 = new std::thread(&TCPClientManager::sendToServer, this);

    return EXIT_SUCCESS;
}