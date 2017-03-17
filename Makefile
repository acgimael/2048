LDLIBS = `pkg-config --libs ncurses`

OUTPUT_OPTION = -Wall -Wextra `pkg-config --cflags ncurses`

NAME = 2048

all: $(NAME)

$(NAME).o: $(NAME).c

.phony: all clean

clean:
	rm -f 2048 2048.o
