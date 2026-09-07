#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

void send_message(int i) 
{
	int sock_fd;
	struct sockaddr_in s_sock; 
	char i_char	= (char)(i + 0x30);

	sock_fd	= socket(AF_INET, SOCK_DGRAM, 0);

	memset(&s_sock, 0, sizeof(s_sock));
	s_sock.sin_family	= AF_INET; 
	s_sock.sin_port		= htons(3030);
	inet_pton(AF_INET, "127.0.0.1", &s_sock.sin_addr);

	sendto(sock_fd, &i_char, 1, 0, (struct sockaddr *)&s_sock, sizeof(s_sock));

	return;
}

int main(void)
{
	int i;
	pid_t pid;  

	for (i = 0; i < 10; i++) {
		pid = fork();
		if (pid == 0){
			send_message(i);
			_exit(0);
		} 
	}

	for (i = 0; i < 10; i++) {
		wait(NULL);
	}
}