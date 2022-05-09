/**
 ** server.c  -  a server program that uses the socket interface to tcp 
 ** added to by Dylan Wu, Feb 13
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include "speakd.h"

extern char *inet_ntoa( struct in_addr );

#define NAMESIZE		255
#define BUFSIZE			81
#define listening_depth		2

char error_str[BUFSIZE];

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

/* receive message from client, store message, and name if applicable */
void receive_msg(char *buffer, short client_fd, int *n)
{
    fputs("Client: ", stdout);
    while( (*n = recv( client_fd, buffer, BUFSIZE, 0)) > 0 ) 
    {
        buffer[*n] = '\0';
      
        if (strcmp("x\n", buffer) == 0) 
          break;

        check(strcmp("xx\n", buffer) == 0, "Closing connection");

        fputs(buffer, stdout);
    }
}

/* send message to client, store message in messages */
void send_msg(char *buffer, short client_fd, int* n)
{
    fputs("Server: ", stdout);
    while( fgets( buffer, BUFSIZE, stdin ) != NULL )
    {
          int len = strlen(buffer);
          if( (*n = send(client_fd, buffer, len, 0)) != len) 
          {
            check(*n < 0, "client send");
            sprintf(error_str, "client sent %d bytes, attempted %d\n", *n, len);
            fprintf(stderr, error_str);

            // send error to the client
            send(client_fd, error_str, strlen(error_str), 0);
            break;
          } 

          check(strcmp("xx\n", buffer) == 0, 
            "Closing connection");

          if (strcmp("x\n", buffer) == 0) break;
    }
}

void server( int server_number )
{
  int c, i;
  int	n, len;
  short fd, client_fd;
  struct sockaddr_in address, client;
  struct hostent *node_ptr;
  char local_node[NAMESIZE];
  char buffer[BUFSIZE+1];

  int num_msgs = 0;

  /*  get the internet name of the local host node on which we are running  */
  check(gethostname(local_node, NAMESIZE) < 0, "server gethostname");

  fprintf(stderr, "server running on node %s\n", local_node);

  /*  get structure for local host node on which server resides  */
  check((node_ptr = gethostbyname( local_node )) == NULL, 
    "server gethostbyname");


  /*  set up Internet address structure for the server  */
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  memcpy(&address.sin_addr, node_ptr->h_addr, node_ptr->h_length);
  address.sin_port = htons(server_number);

  fprintf(stderr, "server full name of server node %s, internet address %s\n",
      node_ptr->h_name, inet_ntoa(address.sin_addr));

  /*  open an Internet tcp socket  */
  check(( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ), 
    "server socket");

  /*  bind this socket to the server's Internet address  */
  check( bind( fd, (struct sockaddr *)&address, sizeof(address) ) < 0, 
    "server bind");

  /*  now find out what local port number was assigned to this server  */
  len = sizeof(address);
  check( getsockname( fd, (struct sockaddr *)&address, &len ) < 0, 
    "server getsockname");

  /*  we are now successfully established as a server  */
  fprintf(stderr, "server at internet address %s, port %d\n",
      inet_ntoa(address.sin_addr), ntohs(address.sin_port));

  /*  start listening for connect requests from clients  */
  check( listen( fd, listening_depth ) < 0, "server listen");

  /*  now accept a client connection (we'll block until one arrives)  */
  len = sizeof(client);
  check( (client_fd = accept(fd, (struct sockaddr *)&client, &len)) < 0, 
    "server accept");

  /*  we are now successfully connected to a remote client  */
  fprintf(stderr, "server connected to client at Internet address %s, port %d\n",
      inet_ntoa(client.sin_addr), ntohs(client.sin_port));


  // main loop of sending and receiving
  while (1)
  {
    receive_msg(buffer, client_fd, &n);
    send_msg(buffer, client_fd, &n);
  }

  if( n < 0 )
    perror( "server read" );

  /*  close the connection to the client  */
  check( close(client_fd) < 0, "server close connection to client" );

  /*  close the "listening post" socket by which server made connections  */
  check( close(fd) < 0, "server close");
}
