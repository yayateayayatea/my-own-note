#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <string.h>

int main(void)
{
	int sock_fd; 
	int byte_read;
	char server_buffer[1024];
	socklen_t sock_len;
	struct sockaddr_in s_sock_in;


	sock_fd		= socket(AF_INET, SOCK_STREAM, 0);

	memset(&s_sock_in, 0, sizeof(struct sockaddr_in));
	s_sock_in.sin_family	= AF_INET;
	s_sock_in.sin_port	= htons(3030);
	inet_pton(AF_INET, "127.0.0.1", &s_sock_in.sin_addr);

	sock_len	= sizeof(s_sock_in);
	connect(sock_fd, (struct sockaddr *)&s_sock_in, sock_len);

	write(sock_fd, "hihi", sizeof("hihi") - 1);
	byte_read	= read(sock_fd, server_buffer, 1024);
	write(1, server_buffer, byte_read);

	close(sock_fd);
	return 0;
}