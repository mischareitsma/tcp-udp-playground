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

struct options {
	CLIENT_SERVER client_server;
	TCP_UDP tcp_udp;
	char *ip;
	int port;
};

int parse_arguments(struct options *options, int argc, char **argv);

#endif /* __OPTIONS_H__ */
