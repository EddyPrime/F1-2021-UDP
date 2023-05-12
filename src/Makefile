CC=gcc
CFLAGS= -g -O2

TARGET=app
dSYM = $(TARGET).dSYM

AVR=avr

.PHONY: clean all windows windows64

.SILENT: clean

all:
	$(CC) $(CFLAGS) $(TARGET).c -o $(TARGET)

allDebug:
	$(CC) $(CFLAGS) -Wall $(TARGET).c -o $(TARGET)

windows:
	i686-w64-mingw32-gcc $(CFLAGS) $(TARGET).c -o app.exe

windows64:
	x86_64-w64-mingw32-gcc $(CFLAGS) $(TARGET).c -o app.exe

clean:
	$(RM) $(TARGET)
	
	if [ -d "${dSYM}" ]; then \
        rm -rf ${dSYM}; \
    fi \