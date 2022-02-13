CC=gcc
CFLAGS= -g -O2 -Wall

TARGET=app
TARGET_DIR=src

AVR=avr

.PHONY: clean all windows windows64

all:
	make -C $(AVR)
	$(CC) $(CFLAGS) $(TARGET_DIR)/$(TARGET).c $(AVR)/serial.c -o $(TARGET)

windows:
	make -C $(AVR)
	i686-w64-mingw32-gcc $(CFLAGS) $(TARGET_DIR)/$(TARGET).c -o app.exe

windows64:
	make -C $(AVR)
	x86_64-w64-mingw32-gcc $(CFLAGS) $(TARGET_DIR)/$(TARGET).c -o app.exe

clean:
	make -C $(AVR) clean
	$(RM) $(TARGET)