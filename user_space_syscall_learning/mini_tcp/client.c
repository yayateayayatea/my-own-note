#include <unistd.h> 
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h> 

#ifndef CLIENT_H
#define CLIENT_H

#define CONSOLE		1
#define BUFFER_SIZE	1024

#endif 

void write_int(int input_int)
{
	char c_int = (char)(input_int + 0x30);
	write(CONSOLE, &c_int, 1);
	write(CONSOLE, "\n", 1);
	return;
}

int main(void) 
{
	/*bss*/
	int client_fd; 
	int server_fd; 
	int connect_result;
	int byte_read; 
	
	char server_buffer[BUFFER_SIZE]; 

	struct sockaddr *server_sockaddr; 
	struct sockaddr_in server_sockaddr_in; 

	/*socket()*/
	client_fd = socket(AF_INET, SOCK_STREAM, 0); 
	if (client_fd != -1) {
		write(CONSOLE, "client socket created: ", sizeof("client socket created: ") - 1);
		write_int(client_fd);
	} else {
		write(CONSOLE, "client socket failed", sizeof("client socket failed") - 1); 
		goto exit; 
	}

	/*connect()*/
	server_sockaddr_in.sin_family	= AF_INET; 
	server_sockaddr_in.sin_port	= htons(3030);
	inet_pton(AF_INET, "127.0.0.1", &server_sockaddr_in.sin_addr);

	server_sockaddr = (struct sockaddr *)&server_sockaddr_in;
	connect_result = connect(client_fd, server_sockaddr, sizeof(server_sockaddr_in));
	if (connect_result != -1) {
		write(CONSOLE, "client connected\n", sizeof("client connected\n") - 1);
	} else {
		write(CONSOLE, "connection error", sizeof("connection error") - 1);
		goto exit;
	}

	/*communication*/
	server_fd = client_fd; 
	byte_read = read(server_fd, server_buffer, BUFFER_SIZE);
	write(CONSOLE, "from server: ", sizeof("from server: ") - 1);
	write(CONSOLE, server_buffer, byte_read); 

	write(server_fd, "hi from client", sizeof("hi from client") - 1);

	/*exit*/
	close(client_fd);
	exit: 
	return 0; 
}