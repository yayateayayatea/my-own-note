#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

int main(void)
{	
	/* -- bss */
	int sock_fd; 
	int client_fd;
	int byte_read;
	int bind_check;
	char client_message[1024];
	socklen_t sock_len;
	socklen_t client_sock_len;
	struct sockaddr_in s_sock_in;
	struct sockaddr_in c_sock_in;

	/* -- data */
	int port	= 3030;
	char ip[]	= "127.0.0.1";
	char message[]	= "listening...\n";
	char message2[]	= "task done\n";
	char message3[]	= "bind error\n";


	/* --text */
	sock_fd		= socket(AF_INET, SOCK_STREAM, 0);

	memset(&s_sock_in, 0, sizeof(struct sockaddr_in));
	s_sock_in.sin_family	= AF_INET;
	s_sock_in.sin_port	= htons(port);
	inet_pton(AF_INET, ip, &s_sock_in.sin_addr);
	
	sock_len	= sizeof(s_sock_in);
	bind_check	= bind(sock_fd, (struct sockaddr *)&s_sock_in, sock_len);
	if (bind_check == -1) 
		goto bind_error;
	
	write(1, message, sizeof(message) - 1);
	listen(sock_fd, 5);

	memset(&c_sock_in, 0, sizeof(struct sockaddr_in));
	client_sock_len	= sizeof(c_sock_in);
	client_fd	= accept(sock_fd, (struct sockaddr *)&c_sock_in, &client_sock_len);
	byte_read	= read(client_fd, client_message, 1024);
	write(client_fd, client_message, byte_read);
	
	write(1, message2, sizeof(message2) - 1);
	close(client_fd);
	goto safe_exit;

	bind_error: 
	write(1, message3, sizeof(message3) - 1);
	close(sock_fd);

	safe_exit: 
	return 0; 
}