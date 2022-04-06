CC=gcc
CFLAGS= -g -O2

TARGET=app
TARGET_DIR=src

AVR=avr

.PHONY: clean all windows windows64

all:
	$(CC) $(CFLAGS) $(TARGET_DIR)/$(TARGET).c -o $(TARGET)

allDebug:
	$(CC) $(CFLAGS) -Wall $(TARGET_DIR)/$(TARGET).c -o $(TARGET)

windows:
	i686-w64-mingw32-gcc $(CFLAGS) $(TARGET_DIR)/$(TARGET).c -o app.exe

windows64:
	x86_64-w64-mingw32-gcc $(CFLAGS) $(TARGET_DIR)/$(TARGET).c -o app.exe

clean:
	$(RM) $(TARGET)