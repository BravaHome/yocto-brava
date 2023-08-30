# Makefile for the Modbus test programs
# (c) 2022 ibeeto, Rudolf J Streif, rudolf.streif@ibeeto.com

# Package Info
NAME = modbus-test
VERSION_MAJOR = 1
VERSION_MINOR = 0
VERSION = $(VERSION_MAJOR).$(VERSION_MINOR)

# Files
EXECSRC = modbus-server.c modbus-client.c
EXECOBJ = $(EXECSRC:.c=.o)
TARGET  = $(NAME)

HEADERS = modbus-registers.h

# Toolchain
INSTALL ?= install

# Define extra flags here
EXTRACFLAGS = -Wall -fPIC
#LDPATH = -L.

#INC=-Imodbus
LIB=-lmodbus


all: $(TARGET)

$(TARGET): $(EXECOBJ)
	$(CC) $(LDFLAGS) $(LDPATH) $(LIB) $^ -o $@
        
%.o : %.c
	$(CC) $(CFLAGS) $(EXTRACFLAGS) $(INC) -c $<

.PHONY: clean
clean:
	rm -rf $(TARGET) $(EXECOBJ)
