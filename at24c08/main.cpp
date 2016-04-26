#include <stdio.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include "shell.h"
#include "pi-i2c.h"
#include "at24c08.h"

#define EEPROM_ADDRESS	0x50
#define EEPROM_PAGESIZE	8

int eeprom_processor(int fd, int argc, char *argv[])
{
	int		address = 0;
	int		len = 0;
	uint8_t	buf[EEPROM_PAGESIZE * 32] = {0};
	int		result;

	if (argc < 2)
		return -1;

	if (strcmp(argv[0], "r") == 0)
	{
		if (parse_int(argc-1, argv+1, &address) != 1)
			return -1;
		if (argc > 2)
		{
			if (parse_int(argc-2, argv+2, &len) != 1)
				return -1;
		}
		if (len == 0)
			len = EEPROM_PAGESIZE - address % EEPROM_PAGESIZE;
		if (len > sizeof(buf))
			len = sizeof(buf);

		// Read eeprom
		result = eeprom_read(fd, address, buf, len);
		if (result > 0 && result == len)
		{
			dump_buffer(address, buf, result);
		}
		else
			printf("Read eeprom failed.(%d)\n", result);
	}
	else if (strcmp(argv[0], "w") == 0)
	{
		if (argc < 3)
			return -1;
		if (parse_int(argc-1, argv+1, &address) != 1)
			return -1;
		len = parse_byte_array(argc-2, argv+2, buf, sizeof(buf));
		if (len <= 0)
			return -1;

		dump_buffer(address, buf, len);

		result = eeprom_write(fd, address, buf, len);
		if (result <= 0 || result != len)
			printf("Write eeprom failed.(%d)\n", result);
	}

	return 0;
}

void usage()
{
	const char	*help_str =
		"Usage:\n"
		"\teeprom r <address> [len]             - Read eeprom data\n"
		"\teeprom w <address> <data> <data> ... - Write eeprom data\n";
	printf(help_str);
}

int main(int argc, char *argv[])
{
	if (argc < 3) {
		usage();
		return -1;
	}

	const char *i2c_adapter = "/dev/i2c-1";
	const int device_address = 0x50;
	int result = 0;
	int fd = eeprom_open(i2c_adapter, device_address);
	if (fd < 0) {
		printf("open %s failed. errno: %d\n", i2c_adapter, -errno);
		result = -errno;
		goto exit;
	}

	return eeprom_processor(fd, argc-1, argv+1);
	
exit:
	if (fd >= 0)
		eeprom_close(fd);
	return result;
}
