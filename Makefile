TARGET = my_shell

CC = gcc
CFLAGS = -Wall -Wextra -Werror

SRC = main.c input_parser.c shell_loop.c helper.c shell_builtins.c
OBJ = $(SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(TARGET)

re: fclean all