#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

#include "options.h"

struct options options;

void run_server();
void run_client();
void upper(char *s);

int main(int argc, char **argv)
{
	printf("Simple UDP echo start\n");

	options.tcp_udp = UDP;

	int stat = parse_arguments(&options, argc, argv, 1);

	if (stat)
		goto end;

	print_options(&options);

	if (options.client_server == SERVER)
		run_server();
	else
		run_client();

end:
	clear_options(&options);

	printf("Simple UDP echo stop\n");
}

void run_server()
{
	int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

	if (socket_fd < 0) {
		printf("Failed to create socket\n");
		return;
	}

	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = inet_addr(options.host),
		.sin_port = htons(options.port)
	};

	int stat = bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	if (stat != 0) {
		printf("Failed to bind socket\n");
		return;
	}

	while (1) {
		struct sockaddr_in client;
		unsigned int client_addr_len = sizeof(client);

		char msg[1025];
		/* TODO: Instead of memset, could do:
		int n = recvfrom();
		msg[n] = '\0';
		*/
		memset(msg, 0, 1025);

		recvfrom(socket_fd, msg, 1024, MSG_WAITALL,
			(struct sockaddr*)&client, &client_addr_len);

		printf("Received %s from %s:%d\n", msg, 
			inet_ntoa(client.sin_addr), client.sin_port);

		upper(msg);

		// TODO: MSG_CONFIRM is in the geeksforgeeks example, however,
		// is not defined on MacOS. Could do a #inded #def MSG_CONFIGRM 0
		sendto(socket_fd, msg, strlen(msg), 0,
			(struct sockaddr*)&client, client_addr_len);
	}

	// Should put this under a interupt trap.
	close(socket_fd);
}

void run_client()
{
	int socket_fd = socket(AF_INET, SOCK_DGRAM, 0);

	if (socket_fd < 0) {
		printf("Failed to create socket\n");
		return;
	}

	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = inet_addr(options.host),
		// Do not use the same port as the server, but require same input
		.sin_port = htons(options.port)
	};

	// int stat = bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

	// if (stat != 0) {
	// 	printf("Failed to bind socket\n");
	// 	return;
	// }

	printf("Sending message: %s\n", options.message);
	sendto(socket_fd, options.message, strlen(options.message), 0, 
		(struct sockaddr*)&server_addr, sizeof(server_addr));

	struct sockaddr_in reply_addr;
	unsigned int reply_addr_len = sizeof(reply_addr);

	char reply[1025];
	memset(reply, 0, 1025);

	recvfrom(socket_fd, reply, 1024, MSG_WAITALL,
		(struct sockaddr*)&reply_addr, &reply_addr_len);

	printf("Reply from server: %s\n", reply);

}


void upper(char *s)
{
	int len = strlen(s);
	int i;
	for (i = 0; i < len; i++) {
		char upper = toupper(s[i]);
		s[i] = upper;
	}

}
