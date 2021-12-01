#include <stdio.h>
#include <string.h>		//strcpy, strlen
#include <unistd.h>		//close
#include <sys/socket.h>		//socket
#include <sys/types.h>
#include <netinet/in.h>		//sockaddr_in
#include <stdlib.h>		//atoi

int main(int argc, char *argv[])
{
	if(argc != 2)
	{
		printf("Usage: %s <port-no>\n", argv[0]);
		return -1;
	}

	int server_sockfd = 0;
	char msg_buff[256] = {'\0'};
	unsigned int msg_len = 0, send_len = 0, recv_len = 0, cliAddr_len = 0;
	struct sockaddr_in server_addr, client_addr;

	server_sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(server_sockfd < 0)
	{
		printf("Failed to create the UPD socket\n");
		return -1;
	}

	memset(&server_addr, '\0', sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(atoi(argv[1]));

	if(bind(server_sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("Unable to bind the UDP server socket\n");
		close(server_sockfd);
		return -1;
	}

	memset(&client_addr, '\0', sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_port = htons(atoi(argv[1]));
	client_addr.sin_addr.s_addr = INADDR_ANY;
	cliAddr_len = sizeof(client_addr);

	while(1)
	{
		printf("Waiting for a message from client\n");
		recv_len = recvfrom(server_sockfd, msg_buff, sizeof(msg_buff), 0, (struct sockaddr *)&client_addr, &cliAddr_len);
		if(recv_len > 0)
			printf("Received '%s' from upd client\n", msg_buff);
		else
			printf("Failed to receive the message from UDP client\n");

		memset(msg_buff, '\0', sizeof(msg_buff));
		strcpy(msg_buff, "Hello UDP client");
		msg_len = strlen(msg_buff);
		send_len = sendto(server_sockfd, msg_buff, msg_len, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
		if(send_len > 0)
			printf("Sent message successfully to UDP client\n");
		else
			printf("Failed to send the message to UDP client\n");
	}
	close(server_sockfd);
	return 0;
}
