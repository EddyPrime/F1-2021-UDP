CC=gcc
CFLAGS= -g -O2

TARGET=app
dSYM = $(TARGET).dSYM

.PHONY: clean all windows windows64

.SILENT: clean

all:
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)

debug: CFLAGS += -Wall
debug: all

clean:
	$(RM) $(TARGET)
	
	if [ -d "${dSYM}" ]; then \
        rm -rf ${dSYM}; \
    fi \