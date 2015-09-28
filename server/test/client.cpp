/*
 * =====================================================================================
 *
 *       Filename:  client.cpp
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  2013年09月22日 20时44分39秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  imane (), imanecr@gmail.com
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>  
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>

struct TestPkg
{
	uint16_t len; 
	uint16_t cmd;
	char str[200];
}__attribute__ ((packed));

int main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in servaddr;
	if (argc != 3)
		printf("usage client <ip address> <port>\n");
	int port = atol(argv[2]);
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("socket fail\n");
		return -1;
	}

	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);

	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0)
	{
		printf("ip switch fail\n");
		return -1;
	}
	if (connect(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)))
	{
		printf("connect fail\n");
		return -1;
	}

//	char buffer[1024] = {0};
//	char recvbuffer[1024] = {0};
//	int len = 0;

//	int length = 0;
	TestPkg a = {0};
	a.len = sizeof(TestPkg);
	a.cmd = 1;
	strcpy(a.str, "test");

	send(sockfd, (char *)&a, sizeof(a), 0);
	
//	while (length = recv(sockfd, buffer, 1024, 0) > 0)	
//	{
//		memcpy(recvbuffer + len, buffer, length);
//		len += length;
//	}
	
	recv(sockfd, (char *)&a, sizeof(a), 0);

	printf("recv data %s", a.str);

	return 0;
}
