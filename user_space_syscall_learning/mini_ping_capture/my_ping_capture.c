#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

#define CONSOLE	1

void parse_eth_header(unsigned char *buffer)
{
        printf("\n=== ethernet header (0 - 13) ===\n");
        
        printf("destination (0 - 5): ");
        printf("%02X %02X %02X %02X %02X %02X\n", 
                buffer[0], buffer[1], buffer[2], buffer[3], buffer[4], buffer[5]);

        printf("source (6 - 11): ");
        printf("%02X %02X %02X %02X %02X %02X\n", 
                buffer[6], buffer[7], buffer[8], buffer[9], buffer[10], buffer[11]);

        printf("type (12 - 13): ");
        printf("%02X %02X\n", 
                buffer[12], buffer[13]);

        return;
}

void parse_icmp_header(unsigned char *buffer)
{
        printf("\n=== icmp header ===\n");
        
        printf("type (34): %02X \n", buffer[34]);
        printf("code (35): %02X \n", buffer[35]);

        printf("checksum (36-37): ");
        printf("%02X %02X \n", buffer[36], buffer[37]);

        printf("rest of header / parameters (38-41): ");
        printf("%02X %02X %02X %02X \n", buffer[38], buffer[39], buffer[40], buffer[41]);

        return;
}

int main(void) 
{
	/* == bss == */
	int i;
	int sock_fd; 
	int byte_read;
	unsigned char buffer[1024];
	socklen_t c_sock_len;
	struct sockaddr_ll c_sock;

	/* == data ==*/

	/* == text == */
	sock_fd		= socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

	c_sock_len	= sizeof(struct sockaddr_ll);
	while ((byte_read = recvfrom(sock_fd, buffer, 1024, 0, (struct sockaddr *)&c_sock, &c_sock_len)) != -1) {
                if (buffer[12] == 0x08 && buffer[13] == 0x00 && buffer[23] == 0x01) {
                        printf("bytes received: %d\n", byte_read);
                        parse_eth_header(buffer);
                        parse_icmp_header(buffer);
                }
        }

	printf("failed to recvfrom\n");
	close(sock_fd);
	return 0; 
}