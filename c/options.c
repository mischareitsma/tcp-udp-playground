#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "options.h"

/*
	parser.add_argument('--server', '-s', dest='server',
		action='store_true', help='Run TCP server')
	parser.add_argument('--client', '-c', dest='client',
		action='store_true', help='Run TCP client')
	
	parser.add_argument('--host', dest='host', required=True,
		type=str, help='Connect to host')
	parser.add_argument('--port', dest='port', required=True,
		type=int, help='Connect to port')
	
	parser.add_argument('--message', '-m', dest='message', required=False,
		type=str, default='Some message', help='Message to echo')
*/

int malloc_and_copy_string(char **dest, char *string);

/*
 * Parse the arguments for the TCP/UDP server/client functions.
 * The following arguments are supported:
 * --server, -s: Flag indicating to run as a server
 * --client, -c: Flag indicating to run as a client
 * --host, -h: Hostname or IP address for TCP/UDP socket
 * --port, -p: Port number for the TCP/UDP socket
 * --message, -m: MEssage to send as client to the server
 */
int parse_arguments(struct options *options, int argc, char **argv, int verbose)
{

	int is_server = 0;
	int is_client = 0;

	static struct option long_opts[] = {
		{"server", no_argument, 0, 's'},
		{"client", no_argument, 0, 'c'},
		{"host", required_argument, 0, 'h'},
		{"port", required_argument, 0, 'p'},
		{"message", required_argument, 0, 'm'},
		{0, 0, 0, 0}
	};

	int c;

	while (1) {
		int opt_idx = 0;
		c = getopt_long(argc, argv, "sch:p:m:", long_opts, &opt_idx);

		// No more options to parse, break.
		if (c == -1)
			break;
		
		switch (c) {
			// All cases have short options, so no case 0: required.
			case 's':
				if (verbose)
					printf("Option --server / -s found.\n");
				is_server = 1;
				break;
			case 'c':
				if (verbose)
					printf("Option --client / -c found.\n");
				is_client = 1;
				break;
			case 'h':
				if (verbose)
					printf("Option --host / -h found with value '%s'.\n", optarg);
				malloc_and_copy_string(&(options->host), optarg);
				break;
			case 'p':
				if (verbose)
					printf("Option --port / -p found with value: %s.\n", optarg);
				options->port = atoi(optarg);
				break;
			case 'm':
				if (verbose)
					printf("Option --message / -m found with value: '%s'.\n", optarg);
				malloc_and_copy_string(&(options->message), optarg);
				break;
		}

	}

	// Should have server or client, but not both
	if (! (is_server ^ is_client)) {
		printf("Either client or server must be specified, not both or neither\n");
		return 1;
	}

	if (is_server)
		options->client_server = SERVER;
	else
		options->client_server = CLIENT;


	return 0;
}

void clear_options(struct options *options)
{
	if (options->host)
		free(options->host);
	
	if (options->message)
		free(options->message);
}

int malloc_and_copy_string(char **dest, char *string)
{
	// Length, including terminating null.
	int len = strlen(string) + 1;
	*dest = malloc(sizeof(char) * len);
	if (*dest == NULL)
		return 0;
	memcpy(*dest, string, len);

	return len;
}

void print_options(struct options *options)
{
	char *client_server;
	if (options->client_server == CLIENT)
		client_server = "client";
	else if (options->client_server == SERVER)
		client_server = "server";
	else
		client_server = "unknown";

	char *tcp_upd;
	if (options->tcp_udp == TCP)
		tcp_upd = "tcp";
	else if (options->tcp_udp == UDP)
		tcp_upd = "udp";
	else
		tcp_upd = "unknown";

	printf("options: {\n");
	printf("\t\"CLIENT_SERVER\": \"%s\",\n", client_server);
	printf("\t\"TCP_UDP\": \"%s\",\n", tcp_upd);
	printf("\t\"host\": \"%s\",\n", 
		options->host ? options->host : "null");
	printf("\t\"port\": %d,\n", options->port);
	printf("\t\"message\": \"%s\"\n}\n",
		options->message ? options->message : "null");
}
