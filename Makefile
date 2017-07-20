LDLIBS = `pkg-config --libs ncurses`

OUTPUT_OPTION = -Wall -Wextra `pkg-config --cflags ncurses`

NAME = 2048

all: $(NAME)

$(NAME): $(NAME).o main.o

.phony: all clean

clean:
	rm -f $(NAME) *.o
