#include <stdint.h>

int serialport_init();
ssize_t serialport_write(int fd, uint8_t* buf, size_t n);
void serialport_close(int fd);