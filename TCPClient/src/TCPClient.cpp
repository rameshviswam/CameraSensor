#include <iostream>

#include "CommandManager.h"
#include "message.h"
#include <TCPClientManager.h>


#define CONTROL_MODE_AUTO 1


//std::this_thread::sleep_for(std::chrono::milliseconds(3000));
//little_sleep(std::chrono::milliseconds(100));

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
    char arr[] = { 'P', 'I', 'N', 'G' };
    int userChoice = 0;
    int sensitivity = 0;
    float focusDistance = 0.0f;
    long exposureTime = 0;
    long frameDuration;
    while (1) {
        std::cout << std::fflush;
        std::cout << "-----------------------------" << std::endl;
        std::cout << "enter 1 for PING msg" << std::endl;
        std::cout << "Enter 2 for Snapshot Request" << std::endl;
        std::cout << "-----------------------------" << std::endl;
        std::cin >> userChoice;

        if (userChoice == 1) {
            cmdMgr.submitPacket(MSGID::HEARTBEAT_REQ, arr, (size_t)4);

        }
        else if (userChoice == 2) {
            char buf[150];

            std::cout << "Enter exposure gain: ";
            std::cin >> sensitivity;

            std::cout << "Enter exposure time: ";
            std::cin >> exposureTime;

            std::cout << "Enter focus distance: ";
            std::cin >> focusDistance;

            std::cout << "Enter frame Duration: ";
            std::cin >> frameDuration;


            sprintf(buf, "EXPOSURE_GAIN:%d;EXPOSURE_TIME:%ld;FOCUS_DISTANCE:%f;FRAME_DURATION:%ld", sensitivity, exposureTime, focusDistance, frameDuration);
            std::cout << "input string: " << buf << std::endl;
            cmdMgr.submitPacket(MSGID::SNAPSHOT_REQUEST, buf, strlen(buf));
        }
    }

    return 0;
}