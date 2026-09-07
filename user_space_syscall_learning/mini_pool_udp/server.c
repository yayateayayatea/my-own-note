#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <string.h>

#define CONSOLE		1
#define BUFFER_SIZE	1024


int main()
{	
	/* bss */
	int byte_read;
	int socket_fd;
	int sockaddr_in_len;
	int epoll_fd;
	char client_buffer[BUFFER_SIZE];
	struct sockaddr *p_sockaddr; 
	struct sockaddr_in s_sockaddr_in;
	struct sockaddr_in c_sockaddr_in;
	struct epoll_event s_event;
	struct epoll_event sl_event[BUFFER_SIZE];

	/* data */
	int port_number		= 3030;
	char ip_addr[] 		= "127.0.0.1";
	char message[]		= "listening...\n";

	/* text */
	/* socket */
	socket_fd	= socket(AF_INET, SOCK_DGRAM, 0);

	/* bind */
	memset(&s_sockaddr_in, 0, sizeof(struct sockaddr_in));
	s_sockaddr_in.sin_family	= AF_INET;
	s_sockaddr_in.sin_port		= htons(port_number);
	inet_pton(AF_INET, ip_addr, &s_sockaddr_in.sin_addr);

	p_sockaddr	= (struct sockaddr *)&s_sockaddr_in;
	sockaddr_in_len	= sizeof(s_sockaddr_in);
	bind(socket_fd, p_sockaddr, sockaddr_in_len);

	/* create epoll, register socket fd */
	epoll_fd	= epoll_create1(EPOLL_CLOEXEC);

	memset(&s_event, 0, sizeof(struct epoll_event));
	s_event.events	= EPOLLIN;
	s_event.data.fd	= socket_fd;
	epoll_ctl(epoll_fd, EPOLL_CTL_ADD, socket_fd, &s_event);

	/* epoll wait*/
	write(CONSOLE, message, sizeof(message) - 1);
	while (1) {
		epoll_wait(epoll_fd, sl_event, BUFFER_SIZE, -1);

		sockaddr_in_len	= sizeof(c_sockaddr_in);
		byte_read	= recvfrom(
			socket_fd, 
			client_buffer, 
			BUFFER_SIZE, 
			0, 
			(struct sockaddr *)&c_sockaddr_in,
			&sockaddr_in_len
		);

		write(CONSOLE, client_buffer, byte_read);
	}

	/* exit */
	close(socket_fd);
	close(epoll_fd);
	return 0; 
}