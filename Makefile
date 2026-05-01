CFLAGS = -Os -Wall -Wextra

LDLIBS = $$(pkg-config --libs ncurses)

NAME = 2048

$(NAME): $(NAME).o main.o

.PHONY: clean
clean:
	rm -f $(NAME) *.o
