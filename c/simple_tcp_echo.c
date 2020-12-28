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
	printf("Simple TCP echo start\n");

	options.tcp_udp = TCP;

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

	printf("Simple TCP echo stop\n");

}

void run_server()
{
	printf("Running server\n");
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	printf("Allocated server fd: %d\n", socket_fd);

	struct sockaddr_in server_address;

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(options.host);
	server_address.sin_port = htons(options.port);

	int stat;

	if ((stat = bind(socket_fd, (struct sockaddr*)&server_address,
			sizeof(server_address))) != 0) {
		printf("Failed to create server socket, return val %d\n", stat);
		return;
	}

	if (listen(socket_fd, 5) != 0) {
		printf("Failed to listen\n");
		return;
	}

	while(1) {
		printf("Waiting for client connection\n");

		struct sockaddr_in client;
		unsigned int client_size = sizeof(client);
		int client_fd = accept(socket_fd,
			(struct sockaddr*)&client, &client_size);

		if (client_fd < 0) {
			printf("Failed to accept client connection\n");
			return;
		}

		printf("Accepted client connect from \"%s:%d\"\n",
			inet_ntoa(client.sin_addr), client.sin_port);

		char msg[1025];
		memset(msg, 0, 1025);
		recv(client_fd, msg, 1024, 0);

		printf("Client message: \"%s\"\n", msg);

		upper(msg);

		send(client_fd, msg, strlen(msg), 0);
		close(client_fd);
	}

}

void run_client()
{
	printf("Running client\n");

	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_fd < 0) {
		printf("Created socket with file descriptor %d\n", socket_fd);
		return;
	}

	struct sockaddr_in server_addr = {
		.sin_family = AF_INET,
		.sin_addr.s_addr = inet_addr(options.host),
		.sin_port = htons(options.port)
	};

	int stat = connect(socket_fd,
		(struct sockaddr*)&server_addr, sizeof(server_addr));

	if (stat != 0) {
		printf("Failed to connect to server\n");
		return;
	}

	printf("Sending message to server: %s\n", options.message);

	send(socket_fd, options.message, strlen(options.message), 0);

	char reply[1025];

	// Clear old junk
	memset(reply, 0, 1025);

	recv(socket_fd, reply, strlen(options.message), 0);

	printf("Reply message from server: %s\n", reply);

	close(socket_fd);
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
