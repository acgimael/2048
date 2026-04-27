LDLIBS = $$(pkg-config --libs ncurses)

CFLAGS = -march=native -mtune=native -O3 -Wall -Wextra

NAME = 2048

$(NAME): $(NAME).o main.o

clean:
	rm -f $(NAME) *.o

.phony: clean
