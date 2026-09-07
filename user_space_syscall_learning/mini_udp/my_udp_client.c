#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void)
{
	int client_fd; 
	int byte_read;
	socklen_t server_sockaddr_in_len;
	char server_buffer[1024];
	struct sockaddr *server_sockaddr;
	struct sockaddr_in server_sockaddr_in;

	
	client_fd	= socket(AF_INET, SOCK_DGRAM, 0);

	memset(&server_sockaddr_in, 0, sizeof(server_sockaddr_in));
	server_sockaddr_in.sin_family	= AF_INET; 
	server_sockaddr_in.sin_port	= htons(3030);
	inet_pton(AF_INET, "127.0.0.1", &server_sockaddr_in.sin_addr);

	server_sockaddr	= (struct sockaddr *)&server_sockaddr_in;
	sendto(
		client_fd,
		"hi hi from client\n", 
		sizeof("hi hi from client\n") - 1, 
		0, 
		server_sockaddr, 
		sizeof(server_sockaddr_in)
	);

	server_sockaddr_in_len = sizeof(server_sockaddr_in);
	byte_read = recvfrom(
		client_fd,
		server_buffer, 
		1024, 
		0, 
		server_sockaddr, 
		&server_sockaddr_in_len
	);

	write(1, server_buffer, byte_read);
	close(client_fd);
	return 0;
}