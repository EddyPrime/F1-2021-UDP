#define ACM0 "/dev/ttyACM0"

int serialport_init();
ssize_t serialport_write(int fd, void *buf, size_t n);
void serialport_close(int fd);