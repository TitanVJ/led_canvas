/*
 * Some of the code was taken from my own solution to assignment 2 and
 * my assignment 2 solution was based of brian fraser guides and solutions
 * https://opencoursehub.cs.sfu.ca/bfraser/solutions/433/06-linuxprogramming/demo_udpListen.c
 */


#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>			
#include <unistd.h> 
#include <pthread.h>
#include <ctype.h>
#include "UDP.h"
#include <stdbool.h>

void listenToUPD(void)
{
	//netcat -u 192.168.7.2 12345
	printf("    netcat -u 192.168.7.2 %d\n", PORT);
	
	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;                   
	sin.sin_addr.s_addr = htonl(INADDR_ANY);    
	sin.sin_port = htons(PORT);                 
	
	
	int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);
	
	int connect = bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));
	if(connect == -1){
		printf("bind failed to connect to port %d\n",PORT);
	}	
	
		
	struct sockaddr_in sinRemote;
	unsigned int sin_len = sizeof(sinRemote);
	/*char messageRx[MSG_MAX_LEN];
		
	int bytesRx = recvfrom(socketDescriptor,
			messageRx, MSG_MAX_LEN - 1, 0,
			(struct sockaddr *) &sinRemote, &sin_len);

		
	messageRx[bytesRx] = 0;*/
	//printf("Message received (%d bytes): \n\n'%s'\n", bytesRx, messageRx);
		
    char messageTx[MSG_MAX_LEN];

		/*
		*    UPTIME
		*/
		
	sprintf(messageTx, "jumpMouse");	
        
	sin_len = sizeof(sinRemote);
	sendto( socketDescriptor,
		messageTx, strlen(messageTx),
		0,
		(struct sockaddr *) &sinRemote, sin_len);

    printf(" netcat -u 192.168.7.2 %d\n", PORT);

	close(socketDescriptor);
	return;
	
}
