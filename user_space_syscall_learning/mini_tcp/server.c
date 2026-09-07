
#include <unistd.h> 
#include <sys/socket.h> 
#include <arpa/inet.h>
#include <netinet/in.h> 

#ifndef SERVER_H
#define SERVER_H

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
	int socket_fd; 
	int bind_result;
	int listen_result;
	int client_fd;
	int byte_read; 

	char client_buffer[BUFFER_SIZE];

	struct sockaddr_in my_sockaddr_in;
	struct sockaddr *my_sockaddr;

	socklen_t client_sock_len;

	/*data*/

	/*socket()*/
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd != -1) {
		write(CONSOLE, "socket success: ", sizeof("socket success: ") - 1);
		write_int(socket_fd);
	} else {
		write(CONSOLE, "socket failed", sizeof("socket failed") - 1);
		goto exit;
	}

	/*bind()*/
	my_sockaddr_in.sin_family	= AF_INET; 
	my_sockaddr_in.sin_port		= htons(3030);
	inet_pton(AF_INET, "127.0.0.1", &my_sockaddr_in.sin_addr);

	my_sockaddr = (struct sockaddr *)(&my_sockaddr_in);
	bind_result = bind(socket_fd, my_sockaddr, sizeof(my_sockaddr_in));

	if (bind_result != -1) {
		write(CONSOLE, "bind success with port: 3030\n", sizeof("bind success with port: 3030\n") - 1);
	} else {
		write(CONSOLE, "bind failed", sizeof("bind failed") - 1);
		goto exit;
	}

	/*listen()*/
	listen_result = listen(socket_fd, 5);
	if (listen_result != -1) {
		write(CONSOLE, "listening...\n", sizeof("listening...\n") - 1);
	} else {
		write(CONSOLE, "failed to listen", sizeof("failed to listen") - 1);
		goto exit;
	}

	/*accept()*/
	client_sock_len = sizeof(my_sockaddr_in);
	client_fd = accept(socket_fd, my_sockaddr, &client_sock_len);
	if (client_fd != -1) {
		write(CONSOLE, "client catch: ", sizeof("client catch: ") - 1);
		write_int(client_fd);
	} else {
		write(CONSOLE, "bad client", sizeof("bad client") - 1);
		goto exit; 
	}

	/*communication*/
	write(client_fd, "hi from server", sizeof("hi from server") - 1); 
	byte_read = read(client_fd, client_buffer, BUFFER_SIZE);
	write(CONSOLE, "from client: ", sizeof("from client: ") - 1);
	write(CONSOLE, client_buffer, byte_read);

	/*exit*/
	close(socket_fd);
	close(client_fd);

	exit: 
	return 0 ;
}