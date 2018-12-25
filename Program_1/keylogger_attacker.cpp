#pragma comment(lib, "ws2_32")
#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define BUF_SIZE 30
#define DEF_PORT 8080
#define DEF_FILE_NAME "dat.txt"
void error_handling(const char* message);
int main() {
	WSADATA wsa;
	SOCKET hservSock;
	SOCKET hclntSock;
	SOCKADDR_IN servAddr;
	SOCKADDR_IN clntAddr;
	int clntAddr_len;
	int recv_len;
	char buf[BUF_SIZE];
	WSAStartup(MAKEWORD(2, 2), &wsa);
	hservSock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (hservSock == INVALID_SOCKET)
		error_handling("[-] Error: socket() fail");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htonl(DEF_PORT);

	if (bind(hservSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		error_handling("[-] Error: bind() fail");

	if (listen(hservSock, 1) == SOCKET_ERROR)
		error_handling("[-] Error: listen() fail");

	
	
	while (1) {
		
		clntAddr_len = sizeof(clntAddr);
		hclntSock = accept(hservSock, (SOCKADDR*)&clntAddr, &clntAddr_len);
		if (hclntSock == INVALID_SOCKET)
			error_handling("[-] Error : accept() fail");
		FILE *fp = NULL;
		fp = fopen(DEF_FILE_NAME, "wb");
		
		if (fp == NULL)
			error_handling("[-] Error : fopen() fail");
		
		while((recv_len = recv(hclntSock,buf,BUF_SIZE, 0)) != 0){
			fwrite(buf, BUF_SIZE, 1, fp);
		}
		fclose(fp);

	}
	closesocket(hservSock);
	WSACleanup();

	return 0;

}
void error_handling(const char* message) {
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}