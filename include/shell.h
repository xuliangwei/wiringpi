#ifndef _SHELL_H
#define _SHELL_H

#include <stdint.h>
#include <stdio.h>

typedef int (*func_process)(int argc, char *argv[]);

typedef struct {
	const char		*name;
	func_process	func;
} processor;

int parse_long(int argc, char *argv[], long *data);
int parse_int(int argc, char *argv[], int *data);
int parse_byte_array(int argc, char *argv[], uint8_t *buf, int buf_size);

size_t dump_line(int address, uint8_t *buf, size_t len, size_t bytes_in_line);
void dump_buffer(int address, uint8_t *buf, size_t len);

#endif

