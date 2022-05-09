/**
 ** HW 3: Fundamentals of Computer Networking
 ** Project descripton: Simple web client for sending and receiving HTTP GET and OPTIONS requests
 ** Date: Mar 6 2022
 ** @author Dylan Wu
 **/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define NAMESIZE		255
#define BUFSIZE			255
#define HTTP_PORT 	 	80

extern char *inet_ntoa( struct in_addr );


// http request structure to store info parsed from CLI user
typedef struct http_req {
	char *hostname;
	char *path;
	char *method;
} http_req;


void error(char *message)
{
    perror( message );
	exit(1);
}

// helper function that takes a condition and error message
void check(int condition, char *message)
{
  if (condition)
  {
    error(message);
  }
}

// receive message from the server
void receive_msg(char *buffer, short fd, int *n)
{
	fputs("RESPONSE:\n ", stdout);
	int c, i;

	while ( (*n = recv( fd, buffer, BUFSIZE, 0)) > 0 )
	{
		buffer[*n] = '\0';
		fputs(buffer, stdout);
	}
}

// send message to the server
void send_msg(char *buffer, short fd, int *n)
{
		int len = strlen(buffer);

		if( (*n = send(fd, buffer, len, 0)) != len )
		{
			check(*n < 0, "client send");
			fprintf(stderr, "client sent %d bytes, attempted %d\n", *n, len);
		}
}

// set up socket, connect to server at port 80, and then send HTTP request
void send_http_request(http_req *req)
{

	int length;
	int n, len;
	short fd;
	struct sockaddr_in address;
	struct hostent *node_ptr;
	char local_node[NAMESIZE];
	char buffer[BUFSIZE];

	int server_number;
	char *server_node;

	server_node = req->hostname;

	server_number = HTTP_PORT;

	/*  get the internet name of the local host node on which we are running  */
	check( gethostname( local_node, NAMESIZE ) < 0, "client gethostname");

	fprintf(stderr, "client running on node %s\n", local_node);

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
	check( (fd = socket(AF_INET, SOCK_STREAM, 0)) < 0,
		"client socket");


	/*  connect this socket to the server's Internet address  */
	check( connect( fd, (struct sockaddr *)&address, sizeof(address) ) < 0,
		"client connect");

	/*  now find out what local port number was assigned to this client  */
	len = sizeof(address);
	check( getsockname( fd, (struct sockaddr *)&address, &length ) < 0,
		"client getsockname");

	/*  we are now successfully connected to a remote server  */
	fprintf(stderr, "client at internet address %s, port %d\n",
			inet_ntoa(address.sin_addr), ntohs(address.sin_port));


  	//construct request
  	sprintf(buffer, "%s %s HTTP/1.1\r\nHost: %s\r\nOrigin: %s\r\nConnection: close\r\n\r\n", 
  	req->method, req->path, req->hostname, local_node);

  	send_msg(buffer, fd, &n);
  	receive_msg(buffer, fd, &n);

	/*  close the connection to the server  */
	check(close(fd) < 0, "client close");
}

// print out fields of request struct
void print_request(http_req *req, int num){
	printf("======Request #%d=====\n", num);
	printf("Method: %s\n", req->method);
	printf("Hostname: %s\n", req->hostname);
	printf("Path: %s\n\n", req->path);
}

// function for printing out history of requests made
void print_request_history(http_req **history){
	http_req **req;

	int i = 0;
	for (req = history; *req != NULL; req++){
		print_request(*req, i);
		i++;
	}
}

int main(int argc, char *argv[]){

	char *line = NULL;
	size_t length = 0;
	ssize_t read = 0;
	int req_num = 0;

	// http_req **requests = (http_req**)malloc(sizeof(http_req*)*100);

	printf("\nWelcome to the Webber HTTP client. Construct and send your GET/OPTIONS requests below.");
	
	// infinite loop for sending requests
	while (1){
		printf("\n\nRequest #%d.\n", req_num);
		char *fields[] = {"Method", "Hostname", "Path"};

		http_req *req = (http_req*)malloc(sizeof(http_req));

		int j;
		int failure = 0;
		for (j = 0; j < 3; j++){

			printf("  %s: ", fields[j]);
			read = getline(&line, &length, stdin);

			char *l; 
			l = strdup(line); // duplicate the line string
			l[strlen(l)-1] = '\0'; //remove the new line character

			if (read == -1){ // break out of for loop if problem reading line
				failure = 1;
				break;
			}

			if (j == 0) {
				req->method = (char*)malloc(strlen(l)*sizeof(char));
				req->method = l;
			}

			else if (j == 1) {

				int i;
				char suffix[BUFSIZE];

				if (strlen(l) > 12){ // parse out the http protocol prefix

					if (strncmp(l, "https://www.", 12) == 0) {  // https
						for(i=12; i<strlen(l); i++)
							suffix[i-12] = l[i];

						req->hostname = (char*)malloc(sizeof(char)*strlen(l));
						req->hostname = suffix;
					}
					else if (strncmp(l, "http://www.", 11) == 0){ // http
						for(i=11; i<strlen(l); i++)
							suffix[i-11] = l[i];

						req->hostname = (char*)malloc(sizeof(char)*strlen(l));
						req->hostname = suffix;
					}

					else { // otherwise, take whole string
						req->hostname = (char*)malloc(sizeof(char)*strlen(l));
						req->hostname = l;
					}
				}
				
				else { // otherwise, take whole string
					req->hostname = (char*)malloc(sizeof(char)*strlen(l));
					req->hostname = l;
				}
			}

			else { //get path
				req->path = (char*)malloc(sizeof(char)*strlen(l));
				req->path = l;
			}

		}

		if (failure){
			printf("Could not read request\n");
			continue;
		} 

		// print request before sending it
		print_request(req, 0);

		send_http_request(req);
		// requests[req_num] = req; //store the request
		req_num++;
	}

	return 0;
}
