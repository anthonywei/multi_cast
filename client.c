/*client.c - �ಥ�ͻ�����*/

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <malloc.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <stdarg.h>
#include <fcntl.h>

#define MCAST_PORT 8168	/*�˿ں�*/
#define MCAST_ADDR "121.199.45.110"	/*�ಥ��ַ*/

int main(int argc, char*argv[])
{
	int loop = 1;
	int addr_len;
	int fd;
	int err = 0;
	int recvLen = 0;
	int nread = 0;
	int i = 0;
	unsigned char netbuff[32] = {0};
	unsigned char localbuff[32] = {0};
	struct sockaddr_in local_addr;	/*����*/
	struct ip_mreq mreq;	/*�ಥ*/

	/*�����׽���*/
	fd = socket(PF_INET, SOCK_DGRAM, 0);
	if(fd == -1)
	{
		perror("socket:SOCK_DGRAM");
		return -1;
	}
	
	/*��ʼ����ַ*/
	memset(&local_addr, 0, sizeof(local_addr));
	local_addr.sin_family = AF_INET;
	local_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	local_addr.sin_port = htons(MCAST_PORT);

	/* ����bind֮ǰ�������׽ӿ�ѡ�����öಥIP֧��*/
	loop = 1;
	if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &loop, sizeof(loop)) < 0)
	{
		perror("setsockopt:SO_REUSEADDR");
		return -1;
	}
	
	/*��socket*/
	err = bind(fd, (struct sockaddr *)&local_addr, sizeof(local_addr));
	if(err < 0)
	{
		perror("bind()");
		return -1;
	}

	/* ���ûػ���� */
	loop = 1;
	err = setsockopt(fd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
	if(err < 0)
	{
		perror("setsockopt:IP_MULTICAST_LOOP");
		return -1;
	}

	/* ����һ���ಥ�顣��һ������Linux�ںˣ��ض����׽ӿڼ������ܶಥ����*/
	mreq.imr_multiaddr.s_addr = inet_addr(MCAST_ADDR);	/*�ಥ��ַ*/
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);	/*����ӿ�Ĭ��*/
	if(mreq.imr_multiaddr.s_addr == -1)
	{
		perror("224.0.0.1 not a legal multicast address");
		return -1;
	}
	/*����������ಥ��*/
	err = setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
	if (err < 0)
	{
		perror("setsockopt:IP_ADD_MEMBERSHIP");
	}

	/*��������*/
	while(1)
	{
		memset(netbuff, 0, 32);
		memset(localbuff, 0, 32);
		addr_len = sizeof(local_addr);
		recvLen = 32;
		nread = recvfrom(fd, netbuff, recvLen, 0, (struct sockaddr *)&local_addr, (socklen_t*)&addr_len);/*����*/
		memcpy(localbuff, netbuff, 32);
		for(i = 0;i < 6;i++)
		{
			printf("localbuff[%d] = 0x%x\n", i, localbuff[i]);
		}

#if 0
		if(nread > 0)
		{
				/*TODO:���ݴ���*/
		}
#endif
  		sleep(2);
	}

	/*�˳��ಥ��*/
	err = setsockopt(fd, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof(mreq));
	if(err < 0) 
	{
		perror("setsockopt:IP_DROP_MEMBERSHIP");
	}

	close(fd);
	return -1;
}
