SDLife
======

> A Game of Life implementation in C using the SDL v2


## 1. Presentation of the Game of Life

### 1.1. Cellular automata

The Game of Life is a mathematical game devised in 1970 by John Conway. It is
the most famous *cellular automaton*, a simulation game taking no interaction
from the player (Conway called it a zero-player game) consisting in a grid of
"cells" evolving over time between different states according to a specific set
of rules and constituting many unique multicellular patterns. Every such set of
rules designates a specific cellular automaton.

Initially a mathematical curiosity, cellular automata gained popularity for
their display of complex and seemingly organical behavior, with cell patterns
that no longer evolve, others that repeat, and others that displace across the
grid. Macroscopically, observing the continuous evolution of a grid evokes the
(accelerated) growth of microorganisms cultures, hence the name.

The fact that the complex interactions between cells arise directly from a
seemingly simple ruleset has been source of study and reflexion, and the
principle was successfully applied to entirely different fields, such as
finance, or fluid dynamics.

The systematical nature of the game makes it an excellent development
exercise--this very project is just one example of the countless Life
implementations existing either online or on hacker kids' laptops.

A striking property of some cellular automata, including Conway's, is their
Turing-completeness, which means that they can emulate a universal computation
machine, and in theory perform any mathematical computation.


### 1.2. Grid properties

The evolution of the game takes into account the eight cells directly neighbors
of each cell. This selection is called Moore neighborhood of range 1.

The cells neighbor to the central cell (represented by the `@`) according to
this criterion are represented here by `#` characters:

    .....
    .###.
    .#@#.
    .###.
    .....

The game is *binary*, there are only two states a cell can be in: "alive", or
"dead".

The grid is *orthogonal*: the cells are arranged along perpendicular axes.

The evolution is *outer-totalistic*, meaning that the next state is a function
of the total of live neighbor cells and the current state of the cell. (A fully
*totalistic* cellular automaton is one where the cell is included in the total
number of live cells.)

The grid is *isotropic*, meaning the neighbor cells contribute indistinctly to
the evolution rule, the location of a cell bears no meaning in itself (only
matters the number of live and dead neighbors).

The grid is *homogenous*, the rule applies indiscriminately everywhere in the
same way.

Cellular automata that exhibit these properties are called *Life-like cellular
automata*, or *LLCA*.


#### 1.2.1. Size of the grid

The theoretical grid has an infinite size, so that the patterns can evolve
without bounds, however the physical reality does not allow the implementation
of such a grid.

The boundaries are usually implemented as "walls", beyond which no cell can be
born, and thus cells on the border of the grid have three less neighbors (five
for cells in a corner) and their evolution will be affected.

A solution to this problem is to use a grid large enough to let the pattern
grow without hitting the boundaries.

A usual implementation to mimic infinite grid, is by wrapping the grid, meaning
that patterns that cross a boundary reappear on the opposite side. Such a grid
is often called *toroidal*, since it behaves like a torus (a three-dimensional
geometrical figure shaped like a donut, or a bike tube). This is not ideal
though, as patterns can travel back and interact with their origin (for example
a glider circling all the way back and destroy the gun that emitted it).


### 1.3. Cell state transitions

Since there are only two states for a cell, it can only go through four
transitions:

 - from alive, it stays alive (survival)
 - from alive, it turns dead (death)
 - from dead, it turns alive (birth)
 - from dead, it stays dead.

Every generation, the transition is chosen according to the number of live
neighbors. If a live cell has too much live neighbors, it dies of
overpopulation, if too few, it dies of underpopulation. Only if it has an
acceptable number of neighbors can it survive, or be born.

The original rule dictates that a cell is born if it has exactly three live
neighbors, and that it survives if it has two or three live neighbors.


### 1.4. Cell patterns

The original rule allows for various types of patterns to exist. There are
immobile patterns (*still lifes*), that repeat over time (*oscillators*), that
displace across the grid (*spaceships*), that emit smaller patterns (*guns*),
that move and leave a trail of debris behind them (*puffers*), that move and
generate smaller spaceships in their wake (*rakes*), etc.


#### 1.4.1. Basic patterns in the original Game of Life

In the following examples, `@` are used to represent live cells and `.` dead
ones.

The *block*, a simple square, the most basic still life:

    @@
    @@

The *tub*, a hollow plus sign, the only other still life with four cells:

    .@.
    @.@
    .@.

The *blinker*, a period-2 oscillator alternating between a horizontal and a
vertical line of three cells:

    ...
    @@@
    ...

becomes

    .@.
    .@.
    .@.

The *barberpole*, a period-2 ocillator that can be extended indefinitely:

    Bipole    Tripole    Quadpole
    ....@@    .....@@    .......@@
    ...@.@    ....@.@    ......@.@
    ......    .......    .........
    .@.@..    ..@.@..    ....@.@..
    .@@...    .......    .........    etc.
    ......    @.@....    ..@.@....
    ......    @@.....    .........
    ......    .......    @.@......
    ......    .......    @@.......

become

    Bipole    Tripole    Quadpole
    ....@@    .....@@    .......@@
    .....@    ......@    ........@
    ..@.@.    ...@.@.    .....@.@.
    .@....    .......    .........
    .@@...    .@.@...    ...@.@...    etc.
    ......    @......    .........
    ......    @@.....    .@.@.....
    ......    .......    @........
    ......    .......    @@.......

The *glider*, the smallest spaceship, moves diagonally by one cell in four
generations:

    .@..    ....    ....    ....    ....
    ..@.    @.@.    ..@.    @...    ..@.
    @@@.    .@@.    @.@.    ..@@    ...@
    ....    .@..    .@@.    .@@.    .@@@

The glider is probably the most important pattern of the Game of Life, notably
in the construction of Turing machines, as it constitutes a support for
information transmission (the presence of a glider can be interpreted as a `1`
bit and its absence a `0` bit, for instance) and it is also used to collide
with other patterns to elicit interesting reactions. In fact, there is a hunt
for smallest configurations of glider generating specific patterns.

Of all the existing patterns the glider is also the most recognizable pattern,
making it the epitome of the Game of Life.


#### 1.4.2. Consideration on the speed of spaceships

Since the transition only accounts for the immediate neighbor cells, a
spaceship cannot move by more than one cell in any direction by generation.
This limit is called `c`, in reference to the speed of light, the physical
speed limit.

The speed of a spaceship is therefore represented as a fraction of `c`: the
speed of the glider is thus `c/4`.

There are no `c` spaceships in Life, although Life-like variants, especially
explosive ones, comprise some.

In other, non-Life-like flavours of cellular automata, the evolution rule can
consider cells further than the immediate neighbors, allowing the transmission
of information to more than one cell par generation, allowing for a greater
value for `c`. In these versions, it is therefore possible to find spaceships
that move more than one cell per generation (but their speed will never be
greater than their value of `c`).


### 1.5. Rulestring formula

The rulestring is a condensed format allowing to represent summarily, but
completely, all Life-like CA, by listing the exact numbers of live neighbors a
cell requires to be alive the next generation. Only the birth and survival
transitions are described, any configuration that does not match either state
leads to a dead cell. Since there are less than ten neighbor cells, the numbers
can be conveniently represented by a single digit each.

The digits for the birth and survival transitions are written in ascending
order, preceded by the initial of the transition (B or S) and the sections are
separated by a single slash character.

The original rule is written, according to this formula, as `B3/S23`: the cell
is born if it has three neighbors, it survives with two or three, otherwise it
will be dead, whether it dies or was not alive in the first place.

The format presented here (*B* preceding *S*) is also called B/S notation, to
differentiate from the S/B notation (with *S* before *B*) that has fallen into
disuse.

Technically, for absolute terseness either the B and S or the slash can be
dropped (and the program happily accepts formulas omitting either), but keeping
them increases readability and is a matter of personal taste.


### 1.6. Variants of Life

In fifty years, many cellular automata have been explored, and some exhibiting
interesting properties have been noted, and even given a name, usually related
to their specificities.

Here is a list of the names of Life-like cellular automata this program
recognizes, along with their rulestring:

 - 2x2 (B36/S125)
   This variant has the ability to evolve in square blocks of two-by-two cells
 - 34 Life, or Life 3-4 (B34/S34)
   Named after its transition numbers, both three and four
 - Amoeba (B357/S1358)
   Large areas form that resembles amoebas and that can assimilate smaller ones
 - Assimilation (B345/S4567)
   A rule similar, albeit stabler, to Diamoeba
 - Coagulations (B378/S235678)
   An exploding rule that creates stains during its expansion
 - Coral (B3/S45678)
   Creates slow-growing structures resembling coral
 - Corrosion of Conformity (B3/S124)
   A deacaying variant of Mazectric
 - Day & Night (B3678/S34678)
   In this variant, patterns of dead cells over live cells behave similarly to
   patterns of live cell over dead ones (as if its colors were inverted)
 - Diamoeba (B35678/S5678)
   Creates diamond-shaped amoebas
 - Flakes, Life without Death, or LwoD (B3/S012345678)
   Creates beautiful snowflake-like structures (note that cells, once born,
   never die)
 - Gnarl (B1/S1)
   Start with a single cell, and see for yourself.
 - HighLife, or Highlife (B36/S23)
   A rule mostly similar to the original, but interesting for the presence of
   the *replicator*, a pattern that evolves into two versions of itself
 - InverseLife, aka. Inverse life (B0123478/S34678)
   A "negative" version of the original rule
 - Long Life, or Long life (B345/S5)
   Patterns with high longevity
 - Maze (B3/S12345)
   Structures expand slowly and form labyrinthic patterns
 - Mazectric (B3/S1234)
   A variant of Maze with longer and straighter corridors
 - Move (B368/S245)
   A slowly evolving rule, with many spaceships and puffers
 - Pseudo Life, or Pseudo life (B357/S238)
   Looks like the original rule, but no pattern from it behave the same way
 - Replicator (B1357/S1357)
   Every pattern replicates itself
 - Seeds (B2/S)
   Every living cell unconditionally dies one the next generation. Nevertheless
   most patterns evolve explosively
 - Serviettes (B234/S)
   Produces magnificent Persian rug-like geometries
 - Stains (B3678/S235678)
   Evolves into big stable "ink" stains
 - WalledCities (B45678/S2345)
   Stabilizes into cities, areas of high activity surrounded by a continuous
   wall


## 2. The program

This program is a simulator for the Life-like cellular automata identified
above.


### 2.1. Execution

#### 2.1.1. Command-line options

The program is invoked from the command-line and accepts a set of options.

<table>
  <tr>
    <th>Category</th>
    <th>Short option (with optional argument)</th>
    <th>Long option</th>
    <th>Description</th>
    <th>Default value</th>
    <th>Conflict with another option</th>
  </tr>
  <tr>
    <td rowspan="5">Grid management</td>
    <td><code>-w WIDTH</code></td>
    <td><code>--width</code></td>
    <td>Specifies the width of the grid</td>
    <td><code>80</code></td>
    <td><code>-i</code>, <code>-S</code></td>
  </tr>
  <tr>
    <td><code>-h HEIGHT</code></td>
    <td><code>--height</code></td>
    <td>Specifies the height of the grid</td>
    <td><code>60</code></td>
    <td><code>-i</code>, <code>-S</code></td>
  </tr>
  <tr>
    <td><code>-S SIZE</code></td>
    <td><code>--square-size</code></td>
    <td>Specifies the grid as square and gives its size</td>
    <td>None</td>
    <td><code>-w</code> et <code>-h</code>, because equivalent to <code>-w SIZE
-h SIZE</code></td>
  </tr>
  <tr>
    <td><code>-W</code></td>
    <td><code>--wrap</code></td>
    <td>Enables wrapping grid (cf. section 1.2.1.)</td>
    <td>False</td>
    <td>None</td>
  </tr>
  <tr>
    <td><code>-R RULE</code></td>
    <td><code>--game-rule</code></td>
    <td>Specifies the Life variant to run (either by its name if
recognized, or its rulestring)</td>
    <td><code>B3/S23</code></td>
    <td>None</td>
  </tr>
  <tr>
    <td rowspan="4">Grid display</td>
    <td><code>-b BORDER</code></td>
    <td><code>--border</code></td>
    <td>The size of the gap between two cells, in pixels</td>
    <td><code>1</code></td>
    <td><code>-n</code></td>
  </tr>
  <tr>
    <td><code>-n</code></td>
    <td><code>--no-border</code></td>
    <td>Disables the gap between cells</td>
    <td>False</td>
    <td><code>-b</code>, because equivalent to <code>-b0</code></td>
  </tr>
  <tr>
    <td><code>-c CELL_SIZE</code></td>
    <td><code>--cell-size</code></td>
    <td>Gives the size of a cell, in pixels</td>
    <td><code>16</code></td>
    <td>None</td>
  </tr>
  <tr>
    <td><code>-r RATE</code></td>
    <td><code>--update-rate</code></td>
    <td>The number of generations per second</td>
    <td><code>25</code></td>
    <td>None</td>
  </tr>
  <tr>
    <td rowspan="4">File I/O</td>
    <td><code>-f FILE</code></td>
    <td><code>--file</code></td>
    <td>Gives the name of the file to read from and write to</td>
    <td>None</td>
    <td><code>-i</code> and <code>-o</code></td>
  </tr>
  <tr>
    <td><code>-i INPUT_FILE</code></td>
    <td><code>--input-file</code></td>
    <td>Gives the path to the file from which to read the grid's initial
configuration</td>
    <td>None</td>
    <td><code>-f</code>, <code>-w</code> and <code>-h</code></td>
  </tr>
  <tr>
    <td><code>-o OUTPUT_FILE</code></td>
    <td><code>--output-file</code></td>
    <td>Gives the path to the file to write the grid state to</td>
    <td>None</td>
    <td><code>-f</code></td>
  </tr>
  <tr>
    <td><code>-F FORMAT</code></td>
    <td><code>--format</code></td>
    <td>Specifies the format of the grid representation in the input file.
Either *plain*, *plaintext* or *RLE* (case-insensitive)</td>
    <td>None</td>
    <td>None</td>
  </tr>
</table>

Any file path argument (to `-f`, `-i` or `-o`) can be `-`, which specifies to
read from the standard input stream or write to the standard output stream. It
is the only case where the argument to `-f` does not refer to the same file for
input and output: `-f-` is a shortcut to `-i- -o-`.


#### 2.1.2. Graphical interface

The graphical interface of the program is minimalistic; only the grid is
displayed. The cells are displayed as squares; white represent dead cells and
black, live ones. There is a medium-grey border around the cells to distinguish
them visually. The active cell is marked with a semi-opaque grey mask: when it
is alive, it will appear as dark grey, when dead, it will appear light grey.

Only the cell size and border width (or presence altogether) can be changed
(see previous section); the colors are not currently configurable.

There are two evolution modes in the program. It can be run continuously at the
rate specified on the command-line (see above), or be in a paused state, giving
the user time to modify the cells or draw full patterns before they evolve. In
this state, the grid can still be updated by steps.


#### 2.1.3. Mouse and keyboard interaction

The program window can be interacted with using the mouse and the keyboard.
Moving the mouse cursor changes the active cell to the one under the mouse tip.
The left mouse button can be clicked to toggle the state of the active cell (if
alive, the cell will die, if already dead it will be born). The button can also
be kept pressed and the mouse dragged, to toggle every cell the cursor hovers.
If the mouse is dragged too quickly, some cells may be skipped: this is due to
a known limitation in the implementation of the regulation of the program
update frequency.

The keyboard can also be used to change the active cell, using the arrow keys.
Some keys, or key combinations, also have a defined action:

<table>
  <tr>
    <th>Key</th>
    <th>Action</th>
  </tr>
  <tr>
    <td><code>Esc</code></td>
    <td rowspan="3">Quit the program</td>
  </tr>
  <tr>
    <td><code>Ctrl</code> + <code>Q</code></td>
  </tr>
  <tr>
    <td><code>Ctrl</code> + <code>W</code></td>
  </tr>
  <tr>
    <td><code>Space</code></td>
    <td>Toggle run mode</td>
  </tr>
  <tr>
    <td><code>Enter</code></td>
    <td>When paused, update the grid once</td>
  </tr>
  <tr>
    <td><code>T</code></td>
    <td>Toggle the active cell</td>
  </tr>
  <tr>
    <td><code>R</code></td>
    <td>Reset the grid to the configuration in the input file. If no file
was specified, do nothing</td>
  </tr>
  <tr>
    <td><code>W</code></td>
    <td>Write the current state to the given output file. If the
file does not exist, create it. If no file was specified, do nothing</td>
  </tr>
  <tr>
    <td><code>C</code></td>
    <td>Clear the grid (kill all cells)</td>
  </tr>
  <tr>
    <td><code>H</code></td>
    <td>Display in the console a summary of the mouse and key
actions</td>
  </tr>
</table>


#### 2.1.4. File input/output

The program can read and write to text files whose content describe a grid
state (dimensions, state of cells and sometimes rulestring). These file can
come in two distinct formats, *plain text* and *RLE* (run-length encoding).

The format of the input file can be specified with the `-F` command-line
option. If the format is not specified, the program will try to guess the
format. First from its name: if it ends with `.rle`, the RLE format is assumed;
on the contrary if it ends with `.cells`, the plain text format is assumed
instead. Otherwise, the program tries to interpret the contents of the input
file as RLE, then if it does not matches as plain text, before failing.

The option `-F` can also be used to override the format that would be guessed
from the file name or contents, although it is not advised to name grid pattern
files with a non-matching extension.

Both formats are supported for writing to the output file. There is no
command-line option to specify the output file format; it is determined from
the file extension: if it is `.rle`, the RLE format is used, in all other
cases, including if it is `.cells`, the plain-text format is used.


##### 2.1.4.1. Plain text format

The plain text format is a textual representation of the grid as a rectangular
block of text with `.` for dead cells and `@` or `O` for live ones. Although
both characters are recognized, they should not be mixed; if the data contains
both a warning message will be printed on the standard error. The grid must be
written fully, the number of lines gives the height of the grid and the number
of characters per line (which must be identical throughout the lines) gives the
grid width. Lines starting with a `!` are considered comments and are ignored.
Such comments can appear anywhere in the file (before and after the pattern,
but also in the middle of it) and are not concerned by line width restrictions.
However, the bang must be the first character of the line, and the comment runs
to the next line break.

Example: a down-left-oriented glider in plain text

    .@.
    ! this is a comment
    @..
    @@@


##### 2.1.4.2. RLE format

RLE is a compressed format where a range of contiguous cells in the same state
are represented with a single cell and the number of cells in the range. This
format accepts a header line specifying the dimensions of the grid and
optionally the rulestring. The header is mandatory; since the configuration is
compressed, blank line endings can be omitted, and the grid dimensions cannot
be determined solely from the grid data.

Live cells are denoted by a `o`, dead cells by `b`, a `$` represents the end of
a row (and the start of the next) and `!` indicates the end of the pattern.
Whitespace is non significant (but is not allowed within a run compression),
this allows to wrap lines anywhere without constraint. A `#` starts a comment,
and text until the next line break will be ignored. Everything after the
terminating `!` is also considered comment text and will not be parsed.

Example : the southwestward glider in RLE format

    # Comment at the top of the file
    x = 3, y = 3, rule = B3/S23
    bo$
    b$ # Same-line comments!
    3o!
    This text after the terminating ! is not parsed


### 2.2. Developement

The code is written in C, the ubiquitous "low-level" language. It began as a
project to learn and familiarize with the language, and eventually grew into
something actually usable.

I strived to keep the headers well-documented, in case anyone is interested in
taking a look; the code formatting is homogenous, readable (for me, at least)
and obscure sections are commented.


#### 2.2.1. The SDL

The program uses the [SDL](http://www.libsdl.org "The SDL website") (Simple
DirectMedia Layer) version 2 for the graphical part. It is a low-level
graphical library; it provides a powerful yet easy to use API. It is also
widespread and supported on the majority of platforms, which improves the
portability of the project.


#### 2.2.2. Environment

Supported platforms are GNU/Linux, notably Debian distributions, and Windows.

The code was developed initially under Ubuntu 17, then Debian 9; Debian-based
distros are supported, and major GNU/Linux platforms should be as well. Under
MS Windows, the code is developed using Visual Studio 17 and compiles natively,
however GNU compilation using MinGW or Cygwin should be doable, but I have not
tried.

For *nixes, the only package necessary for the sources to compile is
`libsdl2-dev`. Recommended packages are `make` to provide the compilation tool
and `doxygen` for the documentation processor.

For Win32, the developer tools must be installed (they come with Visual Studio)
to provide `nmake` and the compiler/linker. The SDL and doxygen (if desired)
must be installed and configured manually.


#### 2.2.3. Compilation

The code is written following the C11 standard, and uses C-exclusive
constructs, so C++ is not supported and the code is not expected to compile in
it.

The compilation process is handled with `make` on GNU platforms, and `nmake`
under Windows (use of compile and run features of VS are not yet ready), with a
Makefile provided for each platform with the usual rules:
 - `all` to build the executable in the `out` directory,
 - `clean` to delete object files in the `obj` directory,
 - `doc` to generate the documentation in `doc` directory,
 - `cleandoc` to remove the latter directory,
 - `distclean` to reset the project in a clean state,
and file-based rules to compile individual object files.

Development under GNU/Linux is made with GCC and with MSVC under Windows,
with all compilation warnings enabled (`-Wall -Wextra -pedantic` and `/Wall`
respectively), in an effort to make the code as close to standard-compliant as
possible. I also refrained from using GNU-specific extensions to the language
(some POSIX functions are called, but no code constructs like pragmas or
`__attribute__` are used).


#### 2.2.4. Documentation

The definitions of the structures and functions declarations in the headers are
fully (hopefully; or at least mostly) documented in
[Doxygen](http://www.doxygen.org/index.html "Doxygen website") syntax. The
documentation files are not part of the repository, but can be generated quite
easily (cf. previous section). The resulting documentation is in HTML and can
be read with any browser; the entry point is the file `doc/html/index.html`.


## 3. Considered evolutions

This project was revived after a substantial hiatus, and becomes increasingly
viable (although utterly redundant). Here is a set of evolutions I have in mind
for the program, some of which shorter-term than others.

 - Achieving a stable version 1.0  
   This is probably more a spiritual guideline than a concrete goal; it depends
   on me only to decide when to officially put the version tag out, but I want
   to be entirely satisfied with the state of the project for that.
 - OpenGL implementation  
   I am working locally on a version backed with OpenGL for the rendering, it
   is functional, but barely, and needs a lot of polishing.
 - Increased configurability  
   The colors of the grid display, the characters used for the plain text file
   format are fixed (even hard-coded). A means to configure these would be
   welcome, but I am not sure of how to introduce it; I do not want the program
   to depend on an external config file, and I am reluctant to introduce yet
   more command-line options. Macros could be a solution, but that would mean
   no runtime configuration.
 - A better plain text file format  
   Says it all. As implemented the plain text format is sufficient, but poor;
   support for comments, omitted blank line endings would be a plus.
 - GUI  
   To decorate the interface, and augment it with controls, and menus.
   Unfortunaltely I do not know any SDL GUI toolkit, and developing one myself
   from scratch is too big a project to even consider it.
 - Headless run mode  
   This would introduce a totally different type of execution (that would
   justify adding a command-line option). The idea would be to provide input
   and output files and a number of generations, and write to the output file
   the configuration in the input file after this many generations. It would
   be a sort of pattern incubator.
 - Non-totalistic or anisotropic Life-like CA  
   While I do not consider non-binary CA (the grid is backed by a bit array,
   changing the number of states would be a huge work I do not want to tackle),
   this type of CA is one that would be interesting to take on. It would
   involve updating the evolution and next-state-calculation code, that could
   use some reworking anyway.
 - Internationalization  
   Mostly for French, at least initially, and would affect the README and docs.
   This would involve twice the amount of documentation to maintain, which is
   already a full-time job by itself.
