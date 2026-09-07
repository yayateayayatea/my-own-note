#include <unistd.h>
#include <string.h>
#include <sys/socket.h> 
#include <arpa/inet.h> 

#define CONSOLE		1
#define BUFFER_SIZE	1024

int main(void)
{
	/* bss */
	int server_fd;
	int sockaddr_in_len;
	int byte_read;

	char client_buffer[BUFFER_SIZE];

	struct sockaddr *server_sockaddr; 
	struct sockaddr *client_sockaddr;
	struct sockaddr_in server_sockaddr_in;
	struct sockaddr_in client_sockaddr_in;

	/* data */
	int port_number		= 3030;
	char message01[]	= "from client: \n";
	char ip_address[]	= "127.0.0.1"; 

	/* text */
	/* socket */
	server_fd = socket(AF_INET, SOCK_DGRAM, 0);
	
	/* bind */
	memset(&server_sockaddr_in, 0, sizeof(server_sockaddr_in));
	server_sockaddr_in.sin_family		= AF_INET;
	server_sockaddr_in.sin_port		= htons(port_number);
	inet_pton(AF_INET, ip_address, &server_sockaddr_in.sin_addr);

	server_sockaddr = (struct sockaddr *)&server_sockaddr_in;
	bind(server_fd, server_sockaddr, sizeof(server_sockaddr_in));

	/* rx */
	client_sockaddr = (struct sockaddr *)&client_sockaddr_in;
	sockaddr_in_len = sizeof(struct sockaddr_in);
	byte_read = recvfrom(
		server_fd,
		client_buffer,
		BUFFER_SIZE,
		0, 
		client_sockaddr, 
		&sockaddr_in_len
	);

	/*verify from server*/
	write(CONSOLE, message01, sizeof(message01));
	write(CONSOLE, client_buffer, byte_read);

	/* tx */
	sendto(
		server_fd, 
		client_buffer, 
		byte_read, 
		0, 
		client_sockaddr, 
		sizeof(struct sockaddr_in)
	);

	/* exit */
	close(server_fd);
	return 0; 
}