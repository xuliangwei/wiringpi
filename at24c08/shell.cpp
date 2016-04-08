#include "shell.h"
#include <stdlib.h>

int parse_long(int argc, char *argv[], long *data)
{
	char	*p, *endp;

	if (argc < 1 || !data)
		return -1;

	p = argv[0];
	*data = strtol(p, &endp, 0);
	if (endp == p)
		return 0;

	return 1;
}

int parse_int(int argc, char *argv[], int *data)
{
	long	ldata;	
	int		result;

	if (!data)
		return -1;

	result = parse_long(argc, argv, &ldata);
	*data = (int)ldata;
	return result;
}

int parse_byte_array(int argc, char *argv[], uint8_t *buf, int buf_size)
{
	char	*p, *endp;
	int		len = 0;

	if (argc < 1)
		return -1;
	if (!buf || buf_size <= 0)
		return -1;

	for (int i = 0; i < argc; i++)
	{
		p = argv[i];
		buf[len] = (uint8_t)strtol(p, &endp, 16);
		if (endp == p)
			break;
		len++;
		if (len >= buf_size)
			break;
	}

	return len;
}

size_t dump_line(int address, uint8_t *buf, size_t len, size_t bytes_in_line)
{
	char	ch;
	int		offset = address - address % bytes_in_line;
	if (len == 0)
		return 0;
	if (len > (bytes_in_line - (address - offset)))
		len = bytes_in_line - (address - offset);
	printf("%04x: ", offset);
	for (int i = 0; i < address - offset; i++)
		printf("   ");
	for (int i = 0; i < len; i++)
		printf("%02x ", buf[i]);
	for (int i = 0; i < bytes_in_line - len - (address - offset); i++)
		printf("   ");
	printf("    ");
	for (int i = 0; i < address - offset; i++)
		printf(" ");
	for (int i = 0; i < len; i++)
	{
		if (buf[i] < 0x20 || buf[i] >= 0x7f)
			ch = '.';
		else
			ch = (char)buf[i];
		printf("%c", ch);
	}
	for (int i = 0; i < bytes_in_line - len - (address - offset); i++)
		printf(" ");
	printf("\n");
	return len;
}

void dump_buffer(int address, uint8_t *buf, size_t len)
{
	const int	c_bytes_in_line = 16;

	printf("      +0 +1 +2 +3 +4 +5 +6 +7 +8 +9 +A +B +C +D +E +F\n");
	printf("-----------------------------------------------------\n");
	while (len > 0)
	{
		size_t	result = dump_line(address, buf, len, c_bytes_in_line);

		len -= result;
		address += result;
		buf += result;
	}
}

