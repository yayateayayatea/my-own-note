

struct icmp_header {
	unsigned char	type;
	unsigned char	code;
	unsigned short	checksum;
	unsigned int	parameter;
} __attribute__((packed));

struct ip_header {
	unsigned char	version_header_length; 
	unsigned char	escp_ecn;
	unsigned short	total_length;
	unsigned short	identification;
	unsigned short	flags;
	unsigned char	ttl;
	unsigned char	protocol;
	unsigned short	checksum;
	unsigned int 	source_ip;
	unsigned int 	destination_ip;
} __attribute__((packed));

struct ethernet_header {
	unsigned char	destination_mac[6];
	unsigned char	source_mac[6];
	unsigned short	type;
} __attribute__((packed));


unsigned int checksum(void *pointer, int size)
{
	unsigned int checksum	= 0; 
	

	return checksum; 
}

int main(void)
{
	return 0;
}