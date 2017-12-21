# SDLife

> Conway's Game of Life in C using the SDL


## What is it?

The SDL ([Simple DirectMedia Layer][sdl]) is a low-level C
library that provides access to mouse, keyboard and joystick events, audio but
mostly *graphics*.

I started using it for my main project, a
[Point & Click library in C][c-pnc].

But I always wanted to make my very own Game of Life with graphics, and the SDL
being awesome, I went for it.

The Game of Life is a *cellular automaton*, a 0-player game where *alive* or
*dead* cells on a grid interact.


## Technicals

### Compilation

The program was coded under *buntu 17.04, uses the version `2.0.5` of the SDL,
and compiled with `gcc`; however the code should be standard compliant (compiled
with `-pedantic -Wall` and every syntax warning possible enabled).

Apart from pre-installed packages, the source depends on the APT package
`libsdl2-dev` to compile (and dependencies packages, of course).

This project also depends on another of mine, the [logging system in C][log.git]

> Header available [here][log.h],
> and archive [here][liblog.a]


### Execution

The program accepts several command-line options to specify parameters of the
board:

Short option|Long option|Description|Conflicts?
:----:|:---:|:----:|:----:
`-w WIDTH`|`--board-width WIDTH`|The width of the board|None
`-h HEIGHT`|`--board-height  HEIGHT`|The height of the board|None
`-b BORDER`|`--border-size`|The size of the gap between two cells|`--no-border`
`-c SIZE`|`--cell-size`|The size of the representation of one cell, in pixels|None
`-n`|`--no-border`|Disable the borders between the cells|`--border-size`, because equivalent to `-b0`
`-r RATE`|`--update-rate RATE`|The frequency of the generations|`--vsync`
`-R RULE`|`--game-rule RULE`|The evolution rule<sup>[1](#1)</sup> of the game|None
`-v`|`--vsync`|Follow vertical synchronization for refresh rate|`--update-rate`
`-W`|`--wrap`|Create a *toroidal*<sup>[2](#2)</sup> grid instead of a rectangular one|None


***

## Footnotes

<a name="1">1</a>: The game rule is a character string of the form `B<m>/S<n>`
where `<m>` is the concatenation of the different number of alive neighbors one
dead cell needs to be born on the next generation, and `<n>` is the
concatenation of the numbers of alive neighbors one alive cell needs to survive
to the next generation. (Note that the `/` character is optional.)

For example, the strings `B3/S23` and `B3S23` both represent the original rules
of the Game of Life as designed by Conway. (`B3S32` is valid too, but not very
orthodox)

<a name="2">2</a>: A toroidal grid is, by opposition to a rectangular (or
standard) one, is a configuration of the board in which each cell crossing a
wall will re-enter the board on the opposite side instead of disappearing. It
allows to virtually replicate an infinite grid -- except that ships might wrap
around and interact with the original pattern that launched it, resulting in
behaviour that would not have occurred on a truly infinite grid.


[sdl]: http://www.libsdl.org "The SDL website"
[c-pnc]: https://github.com/Moonstroke/C-SDL-Point-Click.git "C+SDL / Point & Click"
[log.git]: https://github.com/Moonstroke/C-log.git "C / log"
[log.h]: https://drive.google.com/uc?id=1abiyY2pTgT5ADHqDJs_0YMSv3cDdelAq "log.h"
[liblog.a]: https://drive.google.com/uc?id=10YVKVufUiqVuuotLexu5ZQHP4d_UvKq0 "liblog.a"
