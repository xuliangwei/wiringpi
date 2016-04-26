#ifndef _AT24C08_H
#define _AT24C08_H

#ifdef __cplusplus
extern "C" {
#endif

int eeprom_open(const char* i2c_adapter, int slave_address);
void eeprom_close(int fd);
int eeprom_write(int fd, int address, uint8_t *buf, size_t len);
int eeprom_read(int fd, int address, uint8_t *buf, size_t len);

#ifdef __cplusplus
}
#endif

#endif

