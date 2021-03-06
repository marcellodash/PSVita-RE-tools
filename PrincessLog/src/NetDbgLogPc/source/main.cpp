﻿
#include <errno.h>

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>

#include <winsock2.h>




char *name;
	int sockfd;

	int new_sockfd;


	struct sockaddr_in reader_addr; 

	struct sockaddr_in writer_addr;
	int writer_len = sizeof(writer_addr);

int main(int argc, char* argv[]){

	int number;

	char buf[0x400];

	WORD versionWanted = MAKEWORD(1, 1);
	WSADATA wsaData;
	WSAStartup(versionWanted, &wsaData);

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

	if(sockfd < 0){

		printf("socket error\n");
		printf("sockfd : 0x%X\n", sockfd);

		perror("socket");
		printf("0x%X\n", errno);

		while(1){}
		return 0;
	}

	memset(&reader_addr, 0, sizeof(reader_addr));

	reader_addr.sin_family = AF_INET;

	reader_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	//reader_addr.sin_addr.s_addr = inet_addr("255.255.255.255");

	reader_addr.sin_port = htons(8080);

	if(bind(sockfd, (struct sockaddr *)&reader_addr, sizeof(reader_addr)) < 0){

		printf("bind error\n");

		perror("bind");
		while(1){}
		return 0;
	}

	if(listen(sockfd, 128) < 0){

		close(sockfd);
		printf("listen error\n");

		perror("listen");

		while(1){}
		return 0;
	}

	while(1){

		memset(&buf, 0, sizeof(buf));

		new_sockfd = accept(sockfd,(struct sockaddr *)&writer_addr, &writer_len);

		if(new_sockfd < 0){
			break;
		}

		recv(new_sockfd, (char*)buf, sizeof(buf), 0);

		printf("%s", buf);

		closesocket(new_sockfd);
	}

	close(sockfd);

	WSACleanup();

	return 0;
}