#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>

#define CONSOLE	1

void parse_eth_header(unsigned char *buffer)
{
        printf("=== ethernet header (0 - 13) ===\n");
        
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

void parse_ip_header(unsigned char *buffer)
{       
        printf("\n\n=== ip header ===\n");
        printf("version + header length (14): %02X \n", buffer[14]);
        printf("DSCP/ECN (15): %02X \n", buffer[15]);
        
        printf("total length (16-17): ");
        printf("%02X %02X \n", buffer[16], buffer[17]);

        printf("identification (18-19): ");
        printf("%02X %02X \n", buffer[18], buffer[19]);

        printf("flags + fragment offset (20-21): ");
        printf("%02X %02X \n", buffer[20], buffer[21]);

        printf("TTL (22): %02X \n", buffer[22]);
        printf("protocol (23): %02X \n", buffer[23]);

        printf("checksum (24-25): ");
        printf("%02X %02X \n", buffer[24], buffer[25]);

        printf("source IP (26-29): ");
        printf("%02X %02X %02X %02X \n", buffer[26], buffer[27], buffer[28], buffer[29]);

        printf("destination IP (30-33): ");
        printf("%02X %02X %02X %02X \n", buffer[30], buffer[31], buffer[32], buffer[33]);

        return; 
}

void parse_tcp_header(unsigned char *buffer)
{
        printf("\n\n=== tcp header ===\n");
        
        printf("source port (34-35): ");
        printf("%02X %02X \n", buffer[34], buffer[35]);

        printf("destination port (36-37): ");
        printf("%02X %02X \n", buffer[36], buffer[37]);

        printf("sequence number (38-41): ");
        printf("%02X %02X %02X %02X \n", buffer[38], buffer[39], buffer[40], buffer[41]);

        printf("acknowledgment number (42-45): ");
        printf("%02X %02X %02X %02X \n", buffer[42], buffer[43], buffer[44], buffer[45]);

        printf("data offset + reserved + flags (46-47): ");
        printf("%02X %02X \n", buffer[46], buffer[47]);

        printf("window size (48-49): ");
        printf("%02X %02X \n", buffer[48], buffer[49]);

        printf("checksum (50-51): ");
        printf("%02X %02X \n", buffer[50], buffer[51]);

        printf("urgent pointer (52-53): ");
        printf("%02X %02X \n", buffer[52], buffer[53]);

        return;
}

void parse_udp_header(unsigned char *buffer)
{
        printf("\n\n=== udp header ===\n");
        
        printf("source port (34-35): ");
        printf("%02X %02X \n", buffer[34], buffer[35]);

        printf("destination port (36-37): ");
        printf("%02X %02X \n", buffer[36], buffer[37]);

        printf("length (38-39): ");
        printf("%02X %02X \n", buffer[38], buffer[39]);

        printf("checksum (40-41): ");
        printf("%02X %02X \n", buffer[40], buffer[41]);

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
	byte_read	= recvfrom(sock_fd, buffer, 1024, 0, (struct sockaddr *)&c_sock, &c_sock_len);

	if (byte_read == -1)
		goto exit;
	printf("by received: %d\n", byte_read);

	parse_eth_header(buffer);
	if (
		buffer[12] == 0x08 &&
		buffer[13] == 0x00
	) {
		parse_ip_header(buffer);
	}

	if (buffer[23] == 0x06) {
		parse_tcp_header(buffer);
	} else if (buffer[23] == 0x11) {
		parse_udp_header(buffer);
	}

	exit: 
	close(sock_fd);
	return 0; 
}