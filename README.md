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
