#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>

#define DEBUG 0

int get_temperature(double *temperature)
{
	const char	*c_device = "/sys/bus/w1/devices/28-800000264ccc/w1_slave";
	int fd = open(c_device, O_RDONLY);
	if (fd < 0) {
		printf("open %s failed.(%d)\n", c_device, -errno);
		return -errno;
	}

	char buf[128] = {0};
	ssize_t read_bytes = read(fd, buf, sizeof(buf));
	if (read_bytes < 0) {
		printf("read failed.(%d)\n", -errno);
		close(fd);
		return -errno;
	}

#if DEBUG
	printf("read %d bytes.\n", read_bytes);
	printf("%s\n", buf);
#endif

	if (strstr(buf, "crc=") && strstr(buf, "YES")) {
		const char *t = strstr(buf, "t=");
		if (t) {
			if (temperature)
			*temperature = (double)atol(t+2) / 1000;
		}
	}
	close(fd);
	return 0;
}

int main(void) {
	const double c_delta = 1;
	double last_temperature;
	double temperature = -300;

	do {
		last_temperature = temperature;
		int status = get_temperature(&temperature);
		if ( status < 0) {
			printf("Read temerature failed. error no: %d\n", status);
			return status;
		}
	} while (abs(temperature - last_temperature) > c_delta);
	printf("Temperature: %.3f\n", temperature);
	return 0;
}
