# SDLife

> Conway's Game of Life in C using the SDL


## What is it?

The SDL ([Simple DirectMedia Layer](http://www.libsdl.org)) is a low-level C
library that provides access to mouse, keyboard and joystick events, audio but
mostly *graphics*.

I started using it for my main project, a
[Point & Click library in C](https://github.com/Moonstroke/C-SDL-Point-Click).

But I alway wanted to make my very own Game of Life with graphics, and the SDL
being awesome, I went for it.

The Game of Life is what is called a *cellular automaton*, a 0-player game where
*alive* or *dead* cells on a grid interact.


## Technicals

### Compilation

The program was coded under *buntu 17.04, uses the version 2.0 of the SDL, and
compiled with `gcc`; however the code should be standard compliant (compiled
with `-pedantic -Wall` and every syntax warning possible enabled).

Apart from pre-installed packages, the source depends on the APT package
`libsdl2-dev` to compile (and dependencies packages, of course).

This project also depends on another of mine, the [logging system in C](https://github.com/Moonstroke/C-log.git)

> Header available [here](https://drive.google.com/uc?id=1abiyY2pTgT5ADHqDJs_0YMSv3cDdelAq)
> And archive [here](https://drive.google.com/uc?id=10YVKVufUiqVuuotLexu5ZQHP4d_UvKq0)


### Execution

The program accepts several command-line options to specify parameters of the
board:

 - `-w WIDTH`, or `--board-width WIDTH`, the width of the board to `WIDTH`,
 - `-h HEIGHT`, or `--board-height  HEIGHT`, the height of the board to
   `HEIGHT`,
 - `-b BORDER`, or `--border-size`, the size of the gap between two cells,
 - `-c SIZE`, or `--cell-size`, the size of the representation of one cell, in
   pixels,
 - `-n`, or `--no-border`, to disable the borders between the cells (conflicts
   with `-b` because it is equivalent to `--border-size=0`)
 - `-r RATE`, or `--update-rate RATE`, the frequency of the generations,
 - `-v`, or `--vsync`, to follow vertical synchronization for refresh rate
   (conflicts with `--update-rate`)
 - `-W`, or `--wrap`, to create a toroidal grid instead of a rectangular one
   (i.e. Each cell crossing a wall will re-enter the board on the opposite side
   instead of disappearing)


