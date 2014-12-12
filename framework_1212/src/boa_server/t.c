#include <stdio.h>
#include <stdlib.h>
typedef union __NET_IPV4 {
	unsigned int	int32;
	unsigned char	str[4];
} NET_IPV4;
#define TRUE 1
#define FALSE 0
static int netsplit( char *pAddress, void *ip )
{
	unsigned int ret;
	NET_IPV4 *ipaddr = (NET_IPV4 *)ip;

	if ((ret = atoi(pAddress + 9)) > 255)
		return FALSE;
	ipaddr->str[3] = ret;

	*( pAddress + 9 ) = '\0';
	if ((ret = atoi(pAddress + 6)) > 255)
		return FALSE;
	ipaddr->str[2] = ret;

	*( pAddress + 6 ) = '\0';
	if ((ret = atoi(pAddress + 3)) > 255)
		return FALSE;
	ipaddr->str[1] = ret;

	*( pAddress + 3 ) = '\0';
	if ((ret = atoi(pAddress + 0)) > 255)
		return FALSE;
	ipaddr->str[0] = ret;

	printf("ipaddr->str = %d.\r\n", ipaddr->str[3]);
	printf("ipaddr->str = %d.\r\n", ipaddr->str[2]);
	printf("ipaddr->str = %d.\r\n", ipaddr->str[1]);
	printf("ipaddr->str = %d.\r\n", ipaddr->str[0]);
	return TRUE;
}
int main(int argc, char **argv)
{
	NET_IPV4 ipaddrnew ;
	char ipaddr[12];
	memset(&ipaddrnew, 0, sizeof(ipaddrnew));
	memset(ipaddr, 0x00, 12);
	ipaddr[0] = '1';
	ipaddr[1] = '9';
	ipaddr[2] = '2';
	ipaddr[3] = '0';
	ipaddr[4] = '0';
	ipaddr[5] = '1';
	ipaddr[6] = '1';
	ipaddr[7] = '0';
	ipaddr[8] = '0';
	ipaddr[9] = '0';
	ipaddr[10] = '0';
	ipaddr[11] = '0';
	//netsplit(&ipaddrnew, ipaddr);
	netsplit(ipaddr, &ipaddrnew);
}
