/*
	Bind socket to port 8888 on localhost
	*/
#include<io.h>
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

static int bindSocket(SOCKET s, int port) {
	struct sockaddr_in server, client;

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	//Bind
	if (bind(s, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Bind failed with error code : %d", WSAGetLastError());
		return -1;
	}

	puts("Bind done");
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


int acceptConnection(SOCKET s, SOCKET *new_socket) {
	struct sockaddr_in client;
	int c = sizeof(struct sockaddr_in);
	*new_socket = accept(s, (struct sockaddr *)&client, &c);
	if (*new_socket == INVALID_SOCKET)
	{
		printf("accept failed with error code : %d", WSAGetLastError());
		return -1;
	}

	puts("Connection accepted");
	return 0;
}

int main(int argc, char *argv[])
{
	WSADATA wsa;
	SOCKET s, new_socket;
	char *message;
	int done = 1;
	char server_reply[200];

	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}

	printf("Initialized.\n");

	// Create Socket
	createSocket(&s);

	// Bind Socket
	bindSocket(s, 1456);

	//Listen to incoming connections
	listen(s, 3);

	//Accept and incoming connection
	puts("Waiting for incoming connections...");

	//Accept connection
	acceptConnection(s, &new_socket);

	while (done) {
		receiveMsg(new_socket, server_reply, 200);
		printf("received from client: %s\n", server_reply);
		if (!strcmp(server_reply, "bye")){
			done = 0;
		}
		sendMsg(new_socket, server_reply);
	}
	closesocket(s);
	WSACleanup();

	return 0;
}