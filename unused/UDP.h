/*
*   UDP.h creates a thread that listens to port 12345 for incoming UDP packets and 
*   it handles the requests (commands) from the packets
*/

#define MSG_MAX_LEN 1500
#define PORT        12345
#define MAX_PACKET_SIZE 1500

#ifndef _UDP_H_
#define _UDP_H_
void listenToUPD(void);
#endif