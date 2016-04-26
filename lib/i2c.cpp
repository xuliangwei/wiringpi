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

int i2c_smbus_write_i2c_block_data(int fd, uint8_t command, uint8_t len, uint8_t *values)
{
	if (!values || len <= 0 || len > I2C_SMBUS_BLOCK_MAX)
		return -EINVAL;

	i2c_smbus_ioctl_data	ioctl_data;
	i2c_smbus_data 			data;

	data.block[0] = len;
	memcpy(&data.block[1], values, len);
	ioctl_data.read_write = I2C_SMBUS_WRITE;
	ioctl_data.command = command;
	ioctl_data.size = I2C_SMBUS_I2C_BLOCK_DATA;
	ioctl_data.data = &data;

	return ioctl(fd, I2C_SMBUS, &ioctl_data);
}

int i2c_smbus_read_i2c_block_data(int fd, uint8_t command, uint8_t len, uint8_t *values)
{
	if (!values)
		return -EINVAL;

	i2c_smbus_ioctl_data	ioctl_data;
	i2c_smbus_data 			data;

	ioctl_data.read_write = I2C_SMBUS_READ;
	ioctl_data.command = command;
	ioctl_data.size = I2C_SMBUS_I2C_BLOCK_DATA;
	ioctl_data.data = &data;

	data.block[0] = len;
	int result = ioctl(fd, I2C_SMBUS, &ioctl_data);
	if (result < 0)
		return result;
	memcpy(values, &data.block[1], data.block[0]);
	return data.block[0];
}
