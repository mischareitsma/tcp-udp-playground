#ifndef __OPTIONS_H__
#define __OPTIONS_H__

enum CLIENT_SERVER {
	CLIENT = 1,
	SERVER = 2
};

enum TCP_UDP {
	TCP = 1,
	UDP = 2
};

// TODO: Message into the options feels iffy. might need to rename struct
struct options {
	enum CLIENT_SERVER client_server;
	enum TCP_UDP tcp_udp;
	char *host;
	int port;
	char *message;
};

int parse_arguments(struct options *options, int argc, char **argv, int verbose);
void clear_options(struct options *options);
void print_options(struct options *options);

#endif /* __OPTIONS_H__ */
