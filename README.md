# 2048

A simple ncurses implementation of 2048

Mostly wrote this to learn how to use ncurses

It saves your game when you 'q'uit, loads it when you run the game.

There is no game over detection, and I will make some improvements...

## Installing

You may have to install ncurses:

On Ubuntu/Linux Mint:
```sh
sudo apt-get install libncurses5-dev
```

On Fedora:
```sh
sudo dnf install ncurses-devel
```

then,

```sh
git clone https://github.com/acgimael/2048.git
cd 2048
make
```

If everything is ok:
```sh
./2048
```

The game depends on ncurses, and has been tested on Fedora 25 with the
following:

- gcc 6.3.1
- gnu make 4.1
- ncurses 6

and on Ubuntu 14.04

## Playing

Press any of the arrow keys **twice** to make a move. This was the way
I found not to have the move triggered multiple times.

Press 'q' to quit.
Press 'n' to start a new game.

## Design decisions

a.k.a.

*gotos are evil, and so am I*

all jokes aside, having everything global and using goto have been
conscious design decisions, rather than rookie mishaps ;)

Here I gather a few of the design decisions I believe might be frowned
at:

- Making everything global:

  Why I did it:

      I had no plans of having more than one instance of the game
      running in the same process, and I did not want to encapsulate
      the whole thing, especially for such a simple game. It just did
      not seem practical for my barely existent requirements.

- Having a fixed table of numbers to strings of the tile number:

  Why I did it:

      I did not plan to support boards more than 4x4, and as such I
      estimated an arbitrary maximum and just stuck with
      it. Calculating the numbers on the fly would probably be
      acceptable, but I figured that if they got to a certain point
      (i.e., having more than 7 digits, which I also chose
      arbitrarily) then the whole display would be messed up. If the
      player exceeds the maximum of this table, the next numbers will
      be displayed as 2^n. Cheaty, I know. If the player exceeds 255,
      well, I'll be damned.

- Having a flat array for a board:

  Why I did it:

      It was basically easier to deal with the data of a flat array,
      as opposed to dealing with it in a 2d array. I do realize that
      when you declare something such as arr[y][x] that the compiler
      probably allocates an x * y memory region on the stack, and that
      you probably could dump it with fwrite and read it back with
      fread, while also understanding that if that actually worked it
      would probably be *highly* unportable. I basically thought this
      would be, if ever slightly, better than writing a loop to
      dump/read every row of the array.

- Writing and reading to file without concern for endianness:

  Why I did it:

      Who would really care for this is probably a saner question.

- Using goto:

  Why I did it:

      I could either use goto or call a function. Though it seemed
      really unlikely that the whole stack would be taken by calls to
      a function, I just thought that in this particular case it would
      make sense to use goto. Or rather, that there would be no
      particular advantage to using functions.
