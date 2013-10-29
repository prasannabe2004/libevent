#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdarg.h>
#include <limits.h>
#include <fcntl.h>
#include <event.h>
#include <errno.h>
#include <linux/if.h>

#define D_READ_BUFFER 255
#define D_MAX_BUFFER 4095

#define LIBEVENT

int createDgramSocket (void)
{
	return socket (AF_INET, SOCK_DGRAM, 0);
}

int bindToSocket (int i32Socket, struct sockaddr_in *pSockAddr)
{
	return bind (i32Socket, (struct sockaddr *)pSockAddr, sizeof (struct sockaddr));
}

void readEvent (int i32Socket, short event, void *pVoidArg)
{
	char i8Buffer [D_MAX_BUFFER + 1] = {0};
	struct sockaddr_in clientAddr;
	memset (&clientAddr, 0, sizeof (struct sockaddr_in));
	int i32RcvSize = -1;
	int sockSize = sizeof (struct sockaddr);

	i32RcvSize = recvfrom (i32Socket, i8Buffer, D_MAX_BUFFER, 0, (struct sockaddr *)&clientAddr, (socklen_t *)&sockSize);

	if (i32RcvSize < 0)
	{
		return;
	}
	printf("Mesage received=%s\n",i8Buffer);

	return;
}

int main (int argc, char **argv)
{
	struct sockaddr_in servAddr1;
	struct sockaddr_in servAddr2;
	struct event servEvent1;
	struct event servEvent2;
	int i32ServerFd1 = -1;
	int i32ServerFd2 = -1;

	i32ServerFd1 = createDgramSocket ();
	i32ServerFd2 = createDgramSocket ();

	if (i32ServerFd1 < 0 || i32ServerFd2 < 0)
	{
		printf("Socket creation failed: %s\n", strerror (errno));
		exit (EXIT_FAILURE);
	}

	memset (&servAddr1, 0, sizeof (struct sockaddr_in));
	memset (&servAddr2, 0, sizeof (struct sockaddr_in));

	servAddr1.sin_family = AF_INET;
	servAddr1.sin_addr.s_addr = htonl (INADDR_ANY);
	servAddr1.sin_port = htons (5000);

	servAddr2.sin_family = AF_INET;
	servAddr2.sin_addr.s_addr = htonl (INADDR_ANY);
	servAddr2.sin_port = htons (5001);

	if (bindToSocket (i32ServerFd1, &servAddr1) < 0)
	{
		printf("Socket bind failed: %s\n", strerror (errno));
		exit (EXIT_SUCCESS);
	}
	if (bindToSocket (i32ServerFd2, &servAddr2) < 0)
	{
		printf("Socket bind failed: %s\n", strerror (errno));
		exit (EXIT_SUCCESS);
	}
#ifdef LIBEVENT
	event_init ();

	event_set (&servEvent1, i32ServerFd1, EV_READ|EV_PERSIST, readEvent, &servEvent1);
	event_set (&servEvent2, i32ServerFd2, EV_READ|EV_PERSIST, readEvent, &servEvent2);

	event_add (&servEvent1, NULL);
	printf("Added event to listen on port 5000\n");
	event_add (&servEvent2, NULL);
	printf("Added event to listen on port 5001\n");

	event_dispatch();
#else
	readEvent(i32ServerFd1,0,NULL);
#endif
	exit (EXIT_SUCCESS);
	}

