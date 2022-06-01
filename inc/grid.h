/**
 * \file "grid.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief This file contains the definition of the structure representing a
 *        grid of John Conway's <b>Game of Life</b>.
 *
 * This game is a 0-player game, which means its evolution only depends on its
 * initial state and the \a rule that orders it.
 *
 * The principle of the game is simple: given a (virtually) infinite
 * 2-dimensional grid of cells, each \e cell can be either \b dead or \b alive,
 * and on the next step of the game (within the biological register, we speak of
 * \e generations) the state of the cell is determined by the state of its
 * \b eight closest neighbors (\e Moore neighborhood).
 *
 * The rule is:
 * - if the cell has less than \c 2 alive neighbors, it dies (underpopulation)
 * - if the cell is alive and has \c 2 or \c 3 alive neighbors, it remains alive
 *   (survival)
 * - if the cell is alive and has more than \c 3 alive neighbors, it dies
 *   (overpopulation)
 * - and if the cell is dead and has exactly \c 3 alive neighbors, it is born
 *   (reproduction).
 *
 * The rule guiding the evolution of a Game of Life grid can be reduced to a
 * simpler expression, containing only the number of alive neighbors a cell
 * needs to be born and the number of alive neighbors it needs to stay alive, or
 * in a short form:
 * \code B<number of alive neighbors to be born>S<numberof neighbors to survive>
 * \endcode.  So the original rule of the Game of Life as designed by Conway
 * can be expressed as \c B3/S23. Ths format is called the *Golly* format.
 * (\e Golly is a renown Life-simulation software which uses this format to
 * characterize a Life-like cellular automata).
 *
 * Some famous other rules are:
 * - \b Seeds, of form \c B2/S, in this scheme, <em>no cell survives more than a
 *   single generation</em>;
 * - \b HighLife, of rule \c B36/S23,
 * - <b>Day & Night</b>, of rule \c B3678/S34678, a cellular automaton were the
 *   \e dead and \e alive states are symmetrical, meaning that dead cells mirror
 *   the behavior of alive cells.
 */

#ifndef GRID_H
#define GRID_H


#include <stdbool.h>


/**
 * \brief The default number of cells in a row of the grid.
 */
#define DEFAULT_GRID_WIDTH  80

/**
 * \brief The default number of cells in a column of the grid.
 */
#define DEFAULT_GRID_HEIGHT 60

/**
 * \brief The default rule of evolution for the <i>Game of Life</i>, as
 * originally devised by Conway.
 */
#define DEFAULT_GRID_RULE "B3/S23"


/**
 * \brief The type representing the grid of the game.
 */
struct grid {
	unsigned int w; /**< The width of the grid. */
	unsigned int h; /**< The height of the grid. */
	char *cells; /**< The data of the grid cells. */
	/** The rule determining the evolution of the game, as a string in \e Golly
	    format. */
	const char *rule;
	/** A flag indicating whether the state on one side of the grid affects the
	    opposite side. */
	bool wrap;
};


/**
 * \brief Initialize an uninitialized grid with custom parameters.
 *
 * \param[out] grid  The grid to initialize
 * \param[in]  width  The number of cells in one row
 * \param[in]  height The number of cells in one column
 * \param[in]  wrap   If \c true, set up the grid as toroidal
 *
 * \return \c 0 iff the grid was correctly initialized, a negative value
 *         otherwise
 */
int initGrid(struct grid *grid, unsigned int width, unsigned int height,
              bool wrap);


/**
 * \brief Initialize the grid to the state described in the pattern string.
 *
 * The given string must be a representation of a Life grid, as a rectangular
 * text with dots representing dead cells and at-signs for live cells. All lines
 * must be of the same length, which is interpreted as the grid width, and the
 * number of lines gives the grid height.
 *
 * \note This function resets the grid, so callers must pass either an
 *       unititialized grid or a grid that has been passed to \c freeGrid
 *       beforehand to avoid leaking memory.
 *
 * \param[out] grid The grid to initialize
 * \param[in]  repr  The pattern, as a string of dots and @s
 * \param[in]  wrap   If \c true, set up the grid as toroidal
 *
 * \return \c 0 iff the grid was correctly initialized, a negative value
 *         otherwise
 */
int loadGrid(struct grid *grid, const char *repr, bool wrap);


/**
 * \brief Deallocate memory used by a grid.
 *
 * \param[in,out] grid The grid to free
 */
void freeGrid(struct grid *grid);


/**
 * \brief Get the state of a cell from the grid.
 *
 * \param[in] grid The game grid
 * \param[in] i     The row to get
 * \param[in] j     The column to get
 *
 * \return \c true if the cell at (i, j) is \e "alive", or \c false if the cell
 *         is \e "dead" or coordinates are invalid.
 */
bool getGridCell(const struct grid *grid, int i, int j);


/**
 * \brief Invert the state of a cell.
 *
 * \param[in,out] grid The grid
 * \param[in]     x     The row of the cell to toggle
 * \param[in]     y     The column of the cell
 *
 * \return The new state of the cell (\c true means \e alive), or \c false if
 *         the coordinates are invalid
 */
bool toggleCell(struct grid *grid, unsigned int x, unsigned int y);


/**
 * \brief Update the grid to the next generation.
 *
 * The grid is iterated, and each cell is updated according to the rule
 * determining the grid.
 *
 * \param[in,out] grid The grid to update
 *
 * \return \c 0 if no error occurred (memory allocation, iteration)
 */
int updateGrid(struct grid *grid);


/**
 * \brief Clear the grid.
 *
 * \e Kills all the cells in the grid, by turning them off.
 *
 * \param[in,out] grid The grid to clear
 */
void clearGrid(struct grid *grid);


/**
 * \brief Return a textual representation of the current state of the grid.
 *
 * The returned string consists of lines of at-signs and dots to respectively
 * represent live and dead cells, the lines separated with newline characters.
 *
 * \note The returned string is allocated dynamically; callers must take care of
 * freeing it after use.
 *
 * \param[in] grid The grid
 *
 * \return A string representation of the current state of the grid
 */
char *getGridRepr(const struct grid *grid);


#endif /* grid_H */