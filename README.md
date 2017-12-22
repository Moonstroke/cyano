# SDLife

## Conway's Game of Life in C using the SDL


### What is it?

#### The Game of Life

##### Original

The Game of Life is a cellular automaton, a 0-player game where *alive* or
*dead* cells on a grid interact.

The interaction is determined by an evolution rule, which is function of the
cell's current state and the state of its eight closer neighbors:

    ###
    #@#
    ###

where `@` is the cell, the `#` are its neighbors. (This scheme is know as the
Moore neighborhood).

The original rule of the game, as designed by Conway, is as follow:
 - If the cell is alive and has fewer than 2 alive neighbors, it dies
   (**underpopulation**)
 - If the cell is alive and has 2 or 3 alive neighbors, it stays alive to the
   next generation (**survival**)
 - If the cell is alive and has more than 4 alive neighbors, it dies
   (**overpopulation**)
 - And finally, if the cell is dead and has exactly 3 alive neighbors, it is
   born on the next generation (**reproduction**)

The original rules were chosen by Conway for their particularly interesting
behavior; he discovered the existence of patterns that do not evolve (*still
lifes*), repeat over time (*oscillators*) and even patterns that move through
the grid! (*spaceships*) Here are some famous examples:

The *block*, the simplest still life

    @@
    @@

The *blinker*, simplest oscillator (period 2):

    @
    @
    @

evolves into

    @@@

and repeats.

The *glider*, simplest and first discovered spaceship:

     @
    @
    @@@


A still life can be seen as a period-1 oscillator, and a spaceship as an
oscillator of not constant position.

The question of the finiteness of the evolution of a pattern has been solved
with the iscovery of the *Glider gun*, a pattern that emits gliders, discovered
by Bill Gosper:

                            @
                          @ @
                @@      @@            @@
               @   @    @@            @@
    @@        @     @   @@
    @@        @   @ @@    @ @
              @     @       @
               @   @
                @@


##### Golly format

Golly is a renown Life-simulation software, and it uses a particular
characterization of the evolution rules of the Game of Life.

The rules of the game can be reduced to a more concise expression: a cell is
born on the next generation if it has 2 or 3 alive neighbors, and it survives if
it has 3 alive neighbors; otherwise it dies or stays dead. The *Golly* format
represents the latter in a short fashion, which is a character string of the
form `Bx/Sy`, where `x` is the concatenation of the different number of alive
neighbors one dead cell needs to be born on the next generation, and `y` is the
concatenation of the numbers of alive neighbors one alive cell needs to survive
to the next generation.

So Conway's rules can be expressed, in this format, as `B3/S23`.


##### Variants of the Game of Life

The interest of the Golly format is that it allows to classify with simplicity
the differents variations of the original Game of Life.

Though Conway focused on the rules he chose for their particularities, other
rules have been studied, and several presented enough interest for enthusiasts
to look into.

These rules were often dubbed by their discoverer(s), and their name generally
reflects one of their particularity.

The following tables references a number of Life-like cellular automata (2
states, no distinction of the neighbors by their position around the cell,
evolution function only of the *state* of the 9 cells); there are cellular
automata that do not follow these conditions, but they are not implemented in
the program for now.

Name |Rule (in Golly)|Description
:---:|:---:|:---
**2x2**|B36S125|This variant has the ability to evolve in blocks of `2x2` cells
**34 Life**, or Life 3-4|B34S34| Dubbed after the fact that a cell is born or survives if it has 3 or 4 alive neighbors
**Amoeba**|B357/S1358|Large areas form that resemble amoebas and can assimilate smaller ones
**Assimilation**|B345/S4567|A rule similar to, but stabler than, Diamoeba
**Coagulations**|B378/S235678|An explofing rule that creates stains during its expansion
**Conway's Life**, or **Original**|B3/S23|The original rule of the game
**Coral**|B3/S45678|Creates structures that grow slowly and with architecture resembling coral
**Corrosion of Conformity**|B3/S124| A decaying variation of **Mazectric**
**Day & Night**|B3678/S34678|In this rule, dead cells have the same behavior than live cells and reverse patterns can be created
**Diamoeba**|B35678/S5678|Creates *"amoeabas"* in shape of diamonds
**Flakes**, or **Life without Death** (or **LwoD**)|B3/S012345678|Creates magnificent structures like snow flakes (note that the cells, once born, do not die)
**Gnarl**|B1/S1|Start with a single cell, and behold!
**HighLife**, or **Highlife**|B36/S23|A rule similar to Conway's, but interesting with the existence of a simple *replicator* (a pattern that creates 2 copies of itself)
**InverseLife**, or **Inverse life**|B0123478/S34678|A *"negative"* version of Conway's rule
**Long Life**, or **Long life**|B345/S5|Patterns with very high longevity
**Maze**|B3/S12345|Expanding structures forming a vast maze
**Mazectric**|B3/S1234|A variant of **Maze** where the maze corridors are straighter and longer
**Move**|B368/S245|A slowly evolving rule, with many spaceships and puffers
**Pseudo Life**, or **Pseudo life**|B357/S238|Looks like Conway's, but no common pattern from it behave the same way.
**Replicator**|B1357/S1357|Every pattern is a replicator
**Seeds**|B2/S|Though every cell dies on the next generation, most patterns explode
**Serviettes**|B234/S|Produces beutiful persian rugs-like geometries
**Stains**|B3678/235678|
**WalledCities**|B3678/235678|Stabilizes into *"cities"*, areas of high activities delimited by a continuous wall



#### The SDL

The SDL ([Simple DirectMedia Layer][sdl]) is a low-level C
library that provides access to mouse, keyboard and joystick events, audio but
mostly *graphics*.

I started using it for my main project, a
[Point & Click library in C][c-pnc].

But I always wanted to make my very own Game of Life with graphics, and the SDL
being awesome, I went for it.



### Technicals

#### Compilation

The program was coded under *buntu 17.04, uses the version `2.0.5` of the SDL,
and compiled with `gcc`; however the code should be standard compliant (compiled
with `-pedantic -Wall` and every syntax warning possible enabled).

Apart from pre-installed packages, the source depends on the APT package
`libsdl2-dev` to compile (and dependencies packages, of course).

This project also depends on another of mine, the [logging system in C][log.git]

> Header available [here][log.h],
> and archive [here][liblog.a]


#### Execution

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
`-R RULE`|`--game-rule RULE`|The evolution rule of the game|None
`-v`|`--vsync`|Follow vertical synchronization for refresh rate|`--update-rate`
`-W`|`--wrap`|Create a *toroidal*<sup>[1](#1)</sup> grid instead of a rectangular one|None


#### Documentation

The header files are documented, [Doxygen][dox]-style. The generated
documentationdirectory is not included, so in order to have access to it you
must compile the docs yourself -- this means of course that you need Doxygen
installed.


***


### Footnotes

<a name="1">1</a>: A toroidal grid is, by opposition to a rectangular (or
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
[dox]: http://www.doxygen.org/index.html "Doxygen website"
