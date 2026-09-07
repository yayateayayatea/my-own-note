#include <unistd.h> 
#include <sys/socket.h> 
#include <sys/wait.h>
#include <arpa/inet.h>
#include <string.h>


void send_to_server(char *message_to_server)
{
	int socket_fd;
	struct sockaddr *my_sockaddr; 
	struct sockaddr_in my_sockaddr_in; 

	socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	my_sockaddr_in.sin_family	= AF_INET; 
	my_sockaddr_in.sin_port		= htons(3030);
	inet_pton(AF_INET, "127.0.0.1", &my_sockaddr_in.sin_addr);

	my_sockaddr = (struct sockaddr *)&my_sockaddr_in; 
	if ((connect(socket_fd, my_sockaddr, sizeof(my_sockaddr_in))) == 0) {
		write(socket_fd, message_to_server, strlen(message_to_server));
	}

	close(socket_fd);
	return;
}

int main(void)
{
	char message_to_server[] = "hihi\n";
	int i; 

	for(i = 0; i < 10; i++){
		if (fork() == 0) {
			send_to_server(message_to_server); 
			_exit(0);
		}
	}

	while(wait(NULL) > 0);
	return 0; 
}