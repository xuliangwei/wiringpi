#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include "pi-i2c.h"

#define EEPROM_ADDRESS	0x50
#define EEPROM_PAGESIZE	8

int eeprom_open(const char* i2c_adapter, int slave_address)
{
	int result = 0;
	int fd = open(i2c_adapter, O_RDWR);
	if (fd < 0) {
		result = -errno;
		goto exit;
	}

	if (ioctl(fd, I2C_SLAVE, slave_address) < 0) {
		result = -errno;
		goto exit;
	}

	return fd;

exit:
	if (fd >= 0)
		close(fd);
	return result;
}

void eeprom_close(int fd)
{
	if (fd >= 0) {
		close(fd);
	}
}

int eeprom_write_page(int fd, int address, uint8_t *buf, size_t len)
{
	if (!buf || len <= 0)
		return -EINVAL;

	uint8_t	command = (uint8_t)(address & 0xff);
	len = len > EEPROM_PAGESIZE ? EEPROM_PAGESIZE : len;
	int result = i2c_smbus_write_i2c_block_data(fd, command, len, buf);
	if (result == 0)
		result = len;
	return result;
}

int eeprom_read_page(int fd, int address, uint8_t *buf, size_t len)
{
	if (!buf || len <= 0)
		return -EINVAL;

	uint8_t	command = (uint8_t)(address & 0xff);
	return i2c_smbus_read_i2c_block_data(fd, command, len, buf);
}

int eeprom_write(int fd, int address, uint8_t *buf, size_t len)
{
	const int	c_retry_times = 3;
	int		retry = 0;
	size_t	send = 0;
	int		result = -EIO;

	if (!buf || len <= 0)
		return -EINVAL;

	while ((int)len > 0)
	{
		size_t	size = (EEPROM_PAGESIZE - address % EEPROM_PAGESIZE);
		if (size > len)
			size = len;
		result = eeprom_write_page(fd, address, buf+send, size);
		if (result < 0)
		{
			if (retry++ < c_retry_times)
				continue;
			else
				goto exit;
		}
		else if (result != (int)size)
		{
			result += send;
			goto exit;
		}

		send += size;
		len -= size;
		address += size;
		retry = 0;
		usleep(5000);
	}
	result = send;

exit:
	return result;
}

int eeprom_read(int fd, int address, uint8_t *buf, size_t len)
{
	const int	c_retry_times = 3;
	int		retry = 0;
	size_t	recv = 0;
	int		result = 0;

	if (!buf || len <= 0)
		return 0;

	uint8_t	data[I2C_SMBUS_BLOCK_MAX];
	while ((int)len > 0)
	{
		size_t	size = (EEPROM_PAGESIZE - address % EEPROM_PAGESIZE);
		if (size > len)
			size = len;
		result = eeprom_read_page(fd, address, data, size);
		if (result < 0)
		{
			if (retry++ < c_retry_times)
				continue;
			else
				goto exit;
		}

		memcpy(buf+recv, data, result);
		recv += result;
		address += result;
		len -= result;
		retry = 0;
	}
	result = recv;

exit:
	return result;
}


