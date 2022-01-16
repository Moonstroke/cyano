/**
 * \file "board.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief This file contains the definition of the structure representing a
 *        board of John Conway's <b>Game of Life</b>.
 *
 * This game is a 0-player game, which means its evolution only depends on its
 * initial state and the set of \a rules that order it.
 *
 * The principle of the game is simple: given a (virtually) infinite
 * 2-dimensional grid of cells, each \e cell can be either \b dead or \b alive,
 * and on the next step of the game (within the biological register, we speak of
 * \e generations) the state of the cell is determined by the state of its
 * \b eight closest neighbors (\e Moore neighborhood).
 *
 * The rules are as following:
 * - if the cell has less than \c 2 alive neighbors, it dies (underpopulation)
 * - if the cell is alive and has \c 2 or \c 3 alive neighbors, it remains alive
 *   (survival)
 * - if the cell is alive and has more than \c 3 alive neighbors, it dies
 *   (overpopulation)
 * - and if the cell is dead and has exactly \c 3 alive neighbors, it is born
 *   (reproduction).
 *
 * The rules guiding the evolution of a Game of Life board can be reduced to a
 * simpler expression, containing only the number of alive neighbors a cell
 * needs to be born and the number of alive neighbors it needs to stay alive, or
 * in a short form:
 * \code B<number of alive neighbors to be born>S<numberof neighbors to survive>
 * \endcode.  So the original rules of the Game of Life as designed by Conway
 * can be expressed as \c B3/S23. Ths format is called the *Golly* format.
 * (\e Golly is a renown Life-simulation software which uses this format to
 * characterize a Life-like cellular automata).
 *
 * Some famouse other rules are:
 * - \b Seeds, of form \c B2/S, in this scheme, <em>no cell survives more than a
 *   single generation</em>;
 * - \b HighLife, of rule \c B36/S23,
 * - <b>Day & Night</b>, of rule \c B3678/S34678, a cellular automaton were the
 *   \e dead and \e alive states are symmetrical, meaning that dead cells mirror
 *   the behavior of alive cells.
 */

#ifndef BOARD_H
#define BOARD_H


#include <stdbool.h>


/**
 * \brief The default number of cells in a row of the board.
 */
#define DEFAULT_BOARD_WIDTH  80

/**
 * \brief The default number of cells in a column of the board.
 */
#define DEFAULT_BOARD_HEIGHT 60

/**
 * \brief The default rules of evolution for the <i>Game of Life</i>, as
 * originally devised by Conway.
 */
#define DEFAULT_BOARD_RULES "B3/S23"

/**
 * \brief The type representing the board of the game.
 */
struct board {
	unsigned int w,
	/**< The width of the board. */
	             h;
	/**< The height of the board. */
	char *cells;
	/**< The data of the board cells. */
	const char *rules;
	/**< The rules determining the evolution of the game, as a string in
	     \e Golly format. */
	bool wrap;
	/**< A flag indicating whether the state on one side of the board affects
	     the opposite side. */
};


/**
 * \brief Initializes an uninitialized board with custom parameters.
 *
 * \param[out] board  The board to initialize
 * \param[in]     width  The number of cells in one row
 * \param[in]     height The number of cells in one column
 * \param[in]     wrap   If \c true, set up the grid as toroidal
 *
 * \return \c 0 iff the board was correctly initialized, a negative value
 *         otherwise
 */
int initBoard(struct board *board, unsigned int width, unsigned int height,
              bool wrap);

/**
 * \brief Deallocates memory used by a board.
 *
 * \param[in,out] board The board to free
 */
void freeBoard(struct board *board);


/**
 * \brief Gets the status of a cell from the board.
 *
 * \param[in] board The game board
 * \param[in] i     The row to get
 * \param[in] j     The column to get
 *
 * \return \c true if the cell at (i, j) is \e "alive", or \c false if the cell
 *         is \e "dead" or coordinates are invalid.
 */
bool getBoardCell(const struct board *board, int i, int j);


/**
 * \brief Inverts the state of a cell.
 *
 * \param[in,out] board The board
 * \param[in]     x     The row of the cell to toggle
 * \param[in]     y     The column of the cell
 *
 * \return The new state of the cell (\c true means \e alive), or \c false if
 *         the coordinates are invalid
 */
bool toggleCell(struct board *board, unsigned int x, unsigned int y);


/**
 * \brief Retrieves the rules of evolution used for this board, as a string.
 *
 * \param[in] board The board
 *
 * \return The rules of the board, in \e Golly format
 */
const char *getRules(const struct board *board);

/**
 * \brief Updates the rules of the board, using a string to represent the rules.
 *
 * \param[in,out] board The board
 * \param[in]     rules The new rules, as a string in \e Golly format
 *
 * \note The standard rules are represented by the string \c "B3/S23"
 */
void setRules(struct board *board, const char *rules);


/**
 * \brief Updates the board to the next generation.
 *
 * The board is iterated, and each cell is updated according to the rules
 * determining the board.
 *
 * \param[in,out] board The board to update
 *
 * \return \c 0 if no error occurred (memory allocation, iteration)
 */
int updateBoard(struct board *board);

/**
 * \brief Clears the board.
 *
 * \e Kills all the cells in the board, by turning them off.
 *
 * \param[in,out] board The board to clear
 */
void clearBoard(struct board *board);


#endif // BOARD_H
