CC = gcc
CFLAGS = -Wall -Werror -Wextra -std=c11
SRC = src/main.c
OUT = mysh

all: $(OUT)

$(OUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUT) $(SRC)

clean:
	rm -f $(OUT)

run: all
	./$(OUT)
