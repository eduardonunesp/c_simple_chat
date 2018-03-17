CC=gcc
CFLAGS=-c -Wall
LDFLAGS=
SOURCES= \
main.c \
user.c \
room.c \
server.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=chat_server

all: $(SOURCES) $(EXECUTABLE)

clean:
	rm -rf $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@
