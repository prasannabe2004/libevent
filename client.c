#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <event.h>
#include <errno.h>
#include <net/ethernet.h>

#define D_READ_BUFFER 255
#define D_MAX_BUFFER 4095

void sendMsg (int i32ClientFd, struct sockaddr_in * sAddr)
{
	char i8Buffer [D_MAX_BUFFER + 1] = {0};
	int i32RcvSize = -1;
	sprintf(i8Buffer,"%s","Hello World 5001");
	printf("Sending %s\n",i8Buffer);
	i32RcvSize = sendto (i32ClientFd, i8Buffer, D_MAX_BUFFER, 0, (struct sockaddr *)sAddr, sizeof(struct sockaddr));
	if (i32RcvSize < 0)
	{
		perror("Sendto:");
		return;
	}
	return;
}

int createUdpSocket()
{
	return socket (AF_INET, SOCK_DGRAM, 0);
}
int main (int argc, char **argv)
{
	int i32ClientFd = -1;
	struct sockaddr_in servAddr;
	memset (&servAddr, 0, sizeof (struct sockaddr_in));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servAddr.sin_port = htons (5000);
	i32ClientFd = createUdpSocket();
	if (i32ClientFd < 0)
	{
		printf("Socket creation failed: %s\n", strerror (errno));
		exit (EXIT_FAILURE);
	}
	sendMsg(i32ClientFd,&servAddr);
	close(i32ClientFd);
	exit (EXIT_SUCCESS);
}

