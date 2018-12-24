#pragma comment(lib, "ws2_32")
#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "TCP_send.h"
#define BUF_SIZE 30
#define DEF_PORT 8080
#define DEF_IP "127.0.0.1"

int send_to_server(const char* filename) {
	int send_size;
	char buf[BUF_SIZE];
	WSADATA wsa;
	SOCKET send_socket;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	SOCKADDR_IN send_arr;
	send_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (send_socket == INVALID_SOCKET)
		return error_handling("[-] Error : socket() fail");
	memset(&send_arr, 0, sizeof(send_arr));
	send_arr.sin_family = AF_INET;
	send_arr.sin_addr.S_un.S_addr = inet_addr(DEF_IP);
	send_arr.sin_port = htonl(DEF_PORT);
	if (connect(send_socket, &send_arr, sizeof(send_arr)))
		return error_handling("[-] Error: connect() fail");

	FILE *fp;
	fp = fopen(filename, "rb");	
	if (!fp)
		return error_handling("[-] Error: fopen() fail");
	do{
		fread((void*)buf, BUF_SIZE, 1, fp);
	} while ((send_size = send(send_socket,buf,BUF_SIZE, 0 )!=0));
	fclose(fp);
	closesocket(send_socket);
	WSACleanup();
	return 0;

}

int error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	return 1;
}
