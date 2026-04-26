# 2048

A simple ncurses implementation of 2048

Mostly wrote this to learn how to use ncurses

It saves your game when you quit, loads when you run the game.

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

## Playing

Press any of the arrow keys to make a move.

Press 'q' to quit.  
Press 'n' to start a new game.

## Converting old save games

I doubt there would be a lot of people that want to convert their old save game
files to the new format I just introduced, but hey, I thought it would be nice
to just provide a bit of advice if that _was_ the case. So, basically, I just
introduced a "magic number" as a small header to the file, which is just
"2048ACG", followed by a single byte, the version number. If you'd like to
convert old files to this new format, you can run the following small snippet of
code in the terminal:

```sh
cp save.game save.game.backup && printf '2048ACG\x1' | cat - save.game > tmp && mv temp save.game
```

This small command creates a copy of the file as a backup, and then simply
prepends the newly introduced "magic number" to the file.

Or, if you'd prefer to use a hex file editor, you can just add, at the very
beginning of the file, the characters "2048ACG", followed by a single byte set
to 1.
