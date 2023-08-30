CC=gcc
RM=rm

CFLAGS=-c -Wall
LDFLAGS=

DESTDIR=
BINDIR=/usr/bin

SOURCES=r485c.c r485lib.c
OBJECTS=$(SOURCES:.c=.o)

EXECUTABLE=r485c

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

all: $(SOURCES) $(EXECUTABLE)
        
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

clean:
	$(RM) -rf  $(EXECUTABLE) *.o 

install: $(EXECUTABLE)
	mkdir -p $(DESTDIR)/$(BINDIR)
	install -m 0755 $< $(DESTDIR)/$(BINDIR)
