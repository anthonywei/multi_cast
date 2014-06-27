/*server.c - 多播服务程序*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>


#define MCAST_PORT 8168				
#define MCAST_ADDR "224.0.0.1"	

int main(int argc, char**argv)
{
	int fd;
	int i = 0;
	int sendLen = 0;
	unsigned char dataBuf[6] = {0};
	struct sockaddr_in mcast_addr;     
	fd = socket(AF_INET, SOCK_DGRAM, 0);         /*建立套接字*/
	if (fd == -1)
	{
		perror("socket()");
		return -1;
	}
	memset(dataBuf, 0, sizeof(dataBuf));
	memset(&mcast_addr, 0, sizeof(mcast_addr));
	mcast_addr.sin_family = AF_INET;			/*设置协议族类行为AF*/
	mcast_addr.sin_addr.s_addr = inet_addr(MCAST_ADDR);	/*设置多播IP地址*/
	mcast_addr.sin_port = htons(MCAST_PORT);	/*设置多播端口*/
	dataBuf[0] = 0x01;
	dataBuf[1] = 0x02;
	dataBuf[2] = 0x03;
	dataBuf[3] = 0x04;
	dataBuf[4] = 0x05;
	dataBuf[5] = 0x06;
	/*向多播地址发送数据*/
	while(1) 
	{
		sendLen = sendto(fd, dataBuf, 6, 0, (struct sockaddr*)&mcast_addr, sizeof(mcast_addr)) ;
		for(i = 0;i < 6;i++)
		{
			printf("dataBuf[%d] = 0x%x\n", i, dataBuf[i]);
		}
		if(sendLen < 0)
		{
			perror("sendto()");
			return -2;
		}      
		sleep(5);
	}
	return 0;
}
