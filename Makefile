CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LIBS = -lsqlite3 -lssl -lcrypto -lm

SRC = src/main.c src/db.c src/crypto.c src/ui.c
OBJ = $(SRC:.c=.o)

all: passmgr

passmgr: $(OBJ)
	$(CC) $(OBJ) -o passmgr $(LIBS)

clean:
	rm -f $(OBJ) passmgr data/database.db