#include <unistd.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/epoll.h> 

#define CONSOLE         1
#define BUFFER_SIZE     1024


int main(void)
{
        /* --bss-- */
        char message[20];
        char client_data[BUFFER_SIZE];

        int i;
        int byte_read; 
        int e_poll_fd;
        int server_fd; 
        int client_fd;
        int request_number;

        socklen_t sockaddr_in_len;

        struct sockaddr *server_sockaddr;
        struct sockaddr_in server_sockaddr_in;
        struct epoll_event event;
        struct epoll_event event_pool[BUFFER_SIZE];

        /* --data-- */

        /* --rodata-- */
        const int port = 3030;
        const char ip[] = "127.0.0.1";

        /* --text-- */

        /* set up socket */
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        
        /* bind */
        memset(&server_sockaddr_in, 0, sizeof(server_sockaddr_in));

        server_sockaddr_in.sin_family   = AF_INET;
        server_sockaddr_in.sin_port     = htons(port);
        inet_pton(AF_INET, ip, &(server_sockaddr_in.sin_addr));

        sockaddr_in_len = sizeof(server_sockaddr_in);
        server_sockaddr = (struct sockaddr *)&server_sockaddr_in;
        bind(server_fd, server_sockaddr, sockaddr_in_len);

        /* listen */
        listen(server_fd, 5);
        memcpy(message, "listening...", sizeof("listening..."));
        write(CONSOLE, message, strlen(message));

        /* epoll */
        e_poll_fd = epoll_create1(EPOLL_CLOEXEC);

        /* register new fd (server fd)*/
	memset(&event, 0, sizeof(event));
        event.events	= EPOLLIN; 
	event.data.fd	= server_fd; 
	epoll_ctl(e_poll_fd, EPOLL_CTL_ADD, server_fd, &event);

	/* epoll wait */
	while (1) {
		request_number = epoll_wait(e_poll_fd, event_pool, BUFFER_SIZE, -1);

		for (i = 0; i < request_number; i++) {
			if (event_pool[i].data.fd == server_fd) {
				client_fd = accept(server_fd, NULL, NULL);

				memset(&event, 0, sizeof(event));
				event.events	= EPOLLIN; 
				event.data.fd	= client_fd;
				epoll_ctl(e_poll_fd, EPOLL_CTL_ADD, client_fd, &event);
			} else {
				byte_read = read(event_pool[i].data.fd, client_data, BUFFER_SIZE);
				write(CONSOLE, client_data, byte_read);
			}
		}
	}



        /* exit */
        close(server_fd);
        close(client_fd);
        close(e_poll_fd);
        return 0;
}