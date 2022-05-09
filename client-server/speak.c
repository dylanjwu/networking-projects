/**
 ** client.c  -  a server program that uses the socket interface to tcp 
 ** added to by Dylan Wu, Feb 13
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "speak.h"

#define NAMESIZE		255
#define BUFSIZE			81

extern char *inet_ntoa( struct in_addr );

void error_exit(char *message)
{
    perror( message );
    exit(1);
}

// helper function that takes a condition and error message
void check(int condition, char *message)
{
  if (condition)
  {
    error_exit(message);
  }
}

// receive message from the server
void receive_msg(char *buffer, short fd, int *n)
{
	fputs("Server: ", stdout);
	int c, i;

	while( (*n = recv( fd, buffer, BUFSIZE, 0)) > 0 )
	{
		buffer[*n] = '\0';

		if (strcmp("x\n", buffer) == 0) 
			break;

		check (strcmp("xx\n", buffer) == 0, "Closing connection");

		fputs(buffer, stdout);
	}
}

void send_msg(char *buffer, short fd, int *n)
{
	fputs("Client: ", stdout);

	while( fgets( buffer, BUFSIZE, stdin ) != NULL )
	{
		int len = strlen(buffer);

		if( (*n = send(fd, buffer, len, 0)) != len )
		{
			check(*n < 0, "client send");
			fprintf(stderr, "client sent %d bytes, attempted %d\n", *n, len);
			break;
		}

		check(strcmp("xx\n", buffer) == 0, "Closing connection");

		if (strcmp("x\n", buffer) == 0) break;

	}
}

void client( int server_number, char *server_node )
{
	int length;
	int n, len;
	short fd;
	struct sockaddr_in address;
	struct hostent *node_ptr;
	char local_node[NAMESIZE];
	char buffer[BUFSIZE];

	/*  get the internet name of the local host node on which we are running  */
	check( gethostname( local_node, NAMESIZE ) < 0, "client gethostname");

	fprintf(stderr, "client running on node %s\n", local_node);

	/*  get the name of the remote host node on which we hope to find server  */
	if( server_node == NULL )
		server_node = local_node;

	fprintf(stderr, "client about to connect to server at port number %d on node %s\n",
			server_number, server_node);

	/*  get structure for remote host node on which server resides  */
	check( (node_ptr = gethostbyname( server_node )) == NULL,
		"client gethostbyname");

	/*  set up Internet address structure for the server  */
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	memcpy(&address.sin_addr, node_ptr->h_addr, node_ptr->h_length);
	address.sin_port = htons(server_number);

	fprintf(stderr, "client full name of server node %s, internet address %s\n",
			node_ptr->h_name, inet_ntoa(address.sin_addr));

	/*  open an Internet tcp socket  */
	if( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0,
		"client socket");

	/*  connect this socket to the server's Internet address  */
	if( connect( fd, (struct sockaddr *)&address, sizeof(address) ) < 0,
		"client connect");

	/*  now find out what local port number was assigned to this client  */
	len = sizeof(address);
	if( getsockname( fd, (struct sockaddr *)&address, &length ) < 0,
		"client getsockname");

	/*  we are now successfully connected to a remote server  */
	fprintf(stderr, "client at internet address %s, port %d\n",
			inet_ntoa(address.sin_addr), ntohs(address.sin_port));

	/*  transmit data from standard input to server  */

	// main loop of sending and receiving
	while (1) 
	{
		send_msg(buffer, fd, &n);
		receive_msg(buffer, fd, &n);
	}

	/*  close the connection to the server  */
	check(close(fd) < 0, "client close");
}
