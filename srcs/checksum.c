
#include "head.h"
// Calculating the Check Sum
uint16_t		checksum(void *b, int len)
{
	uint16_t		*buf = b;
	uint32_t		sum=0;
	uint16_t		result;

	while (len > 1)
	{
		sum += *buf++;
		len -= 2;
	}
	if (len == 1)
		sum += *(uint8_t*)buf;
	sum = (sum >> 16) + (sum & 0xFFFF);
	sum += (sum >> 16);
	result = ~sum;
	return result;
}
