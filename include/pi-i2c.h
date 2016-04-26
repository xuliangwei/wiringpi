#ifndef _I2C2_H
#define _I2C2_H

#ifdef __cplusplus
extern "C" {
#endif

int i2c_smbus_write_i2c_block_data(int fd, uint8_t command, uint8_t len, uint8_t *values);
int i2c_smbus_read_i2c_block_data(int fd, uint8_t command, uint8_t len, uint8_t *values);

#ifdef __cplusplus
}
#endif

#endif
