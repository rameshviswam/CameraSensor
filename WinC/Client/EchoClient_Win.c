/******************************************************************************
* Create a TCP socket
******************************************************************************/
#include<stdio.h>
#include<winsock2.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library

static int createSocket(SOCKET*s) {
	//Create a socket
	if ((*s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
		return -1;
	}

	printf("Socket created.\n");
	return 1;
}

static int connectToClient(const SOCKET s, const char * addr, const int port) {
	struct sockaddr_in server;
	server.sin_addr.s_addr = inet_addr(addr);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Connect to remote server
	if (connect(s, (struct sockaddr *)&server, sizeof(server)) < 0)
	{
		puts("connect error");
		return -11;
	}

	puts("Connected");
	return 0;
}

const int sendMsg(const SOCKET s, const char *data) {
	int size;
	int dataSize = strlen(data);

	if ((size = send(s, data, dataSize, 0)) < 0) {
		puts("Send failed");
		return 1;
	}

	if ((size = send(s, "\r\n", 2, 0)) < 0) {
		puts("Send failed");
		return 1;
	}

	printf("Send Data Successful.\n");
	printf("Sent Data:%s  Size: %d\n", data, dataSize);
	return 0;
}

const int receiveMsg(const SOCKET s, char *buffer, const unsigned int size) {
	int recv_size;
	memset(buffer, '\0', size);
	//Receive a reply from the server
	if ((recv_size = recv(s, buffer, size, 0)) == SOCKET_ERROR) {
		puts("recv failed");
		return -1;
	}

	printf("Receive Data Successful.\n");
	printf("Received Data:%s  Size: %d\n\n", buffer, recv_size);
	return 0;
}

int main(int argc, char *argv[])
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in server;
	char *message, server_reply[200];
	int recv_size;
	int size;
	char  input_str[2000];
	int i;

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialized.\n");

	// Create Socket
	createSocket(&s);

	//Establish connection
	connectToClient(s, "127.0.0.1", 1456);

	//Send some data
	message = "This is a test from windows C tcp";
	sendMsg(s, message);

	//Recieve response from client
	receiveMsg(s, server_reply, 200);

	while (scanf("%s", input_str)) {
		sendMsg(s, input_str);

		//Recieve response from client
		receiveMsg(s, server_reply, 200);
	}

	closesocket(s);
	WSACleanup();

	return 0;
}