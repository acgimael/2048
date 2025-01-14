LDLIBS = `pkg-config --libs ncurses`

OUTPUT_OPTION = -march=native -mtune=native -O3 -Wall -Wextra `pkg-config --cflags ncurses`

NAME = 2048

all: $(NAME)

$(NAME): $(NAME).o main.o

.phony: all clean

clean:
	rm -f $(NAME) *.o
