#include <stdio.h>
#include <string.h>		//strcpy, strlen
#include <unistd.h>		//close
#include <sys/socket.h>		//socket
#include <sys/types.h>
#include <netinet/in.h>		//sockaddr_in
#include <stdlib.h>		//atoi

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		printf("Usage: %s <port-no>\n", argv[0]);
		return -1;
	}

	int client_sockfd = 0;
	char msg_buff[256] = {'\0'};
	unsigned int msg_len = 0, send_len = 0, recv_len = 0, servAddr_len = 0;
	struct sockaddr_in server_addr;

	client_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(client_sockfd < 0)
	{
		printf("Failed to create the UDP socket\n");
		return -1;
	}

	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(atoi(argv[1]));
	server_addr.sin_addr.s_addr = INADDR_ANY;
	servAddr_len = sizeof(server_addr);

	strcpy(msg_buff, "Hello UDP server");
	msg_len = strlen(msg_buff);
	send_len = sendto(client_sockfd, msg_buff, msg_len, 0, (struct sockaddr *)&server_addr, servAddr_len);
	if(send_len > 0)
		printf("Sent message to UPD server successfully\n");
	else
		printf("Failed to send the message to UDP server\n");

	memset(msg_buff, '\0', sizeof(msg_buff));
	recv_len = recvfrom(client_sockfd, msg_buff, sizeof(msg_buff), 0, (struct sockaddr *)&server_addr, &servAddr_len);
	if(recv_len > 0)
		printf("Received '%s' from UDP server\n", msg_buff);
	else
		printf("Failed to receive the message from UDP server\n");

	close(client_sockfd);
	return 0;
}
