EXEC := crc_verifier
CC := gcc
CFLAGS := -std=gnu99 -Wall -Wextra -pedantic -O2

INCDIR := inc
SRCDIR := src

SOURCES := $(wildcard $(SRCDIR)/*.c)
INCLUDES := -I $(INCDIR)

all: clean $(EXEC)

$(EXEC):
	$(CC) $(CFLAGS) $(INCLUDES) $(SOURCES) -o $(EXEC)

valrun:
	valgrind -v --leak-check=full --show-leak-kinds=all ./$(EXEC) test_dir test_dir/test_crc_db

clean:
	rm -f $(EXEC)
