/**
 * \file "board.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief This file contains the definition of the structure representing a
 *        board of the <b>Game of Life</b>.
 *
 * This game is a 0-player game, which means its evolution only depends on its
 * initial state and the set of \a rules that order it.
 *
 * The principle of the game is simple: given a (virtually) infinite
 * 2-dimensional grid of cells, each \e cell can be either \b dead or \b alive,
 * and on the next step of the game (within the biological register, we speak of
 * \e generations) the state of the cell is determined by the state of its
 * \b eight closest neighbors (\e Moore neighborhood).
 * The rules are as following:
 *
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
 * \code B<number of alive neighbors to be born>S<numberof neighbors to survive> \endcode.
 * So the original rules of the Game of Life as designed by Conway can be
 * expressed as \c B2S23.
 *
 * Some famouse other rules are :
 * - \b Seeds, of form \c B2S, in this scheme, <em>no cell survives more than a
 *   single generation</em>;
 * - \b HighLife, of rule B36S23,
 * - <b>Day & Night</b>, a cellular automaton were the \e dead and \e alive
 *   states are symmetrical, meaning that dead cells mirror in behavior that of
 *   alive cells.
 */

#ifndef LIFE_H
#define LIFE_H


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
 * \brief A type to represent the rules of the cellular automaton.
 *
 * The rules correspond to the equations defining the state of one cell at time
 * \code n + 1 \endcode according to the states of its eight surrounding
 * neighbors at time \c n.
 */
typedef struct {
	const char *b,
	/**< Every character is a number of alive neighbors each cell needs to be
	     born on the next generation.*/
	           *s;
	/**< Every character is a number of alive neighbors each cell needs to
	     survive to the next generation. */
} Rules;

/**
 * \brief The type representing the board of the game.
 */
typedef struct board {
	unsigned int w,
	/**< The width of the board. */
	             h;
	/**< The height of the board. */
	bool *cells;
	/**< The data of the board cells. */
	Rules rules;
	/**< The rules determining this game. */
	bool *(*getCell)(const struct board*, int, int);
	/**< The function the board uses to retrieve a cell on itself. */
} Board;


/**
 * \brief Initializes an uninitialized board with custom parameters.
 *
 * \param[in,out] board  The board to initialize
 * \param[in]     width  The number of cells in one row
 * \param[in]     height The number of cells in one column
 * \param[in]     wrap   If \c true, set up the grid as toroidal
 *
 * \return \c true iff the board was correctly initialized
 */
bool initBoard(Board *board, unsigned int width, unsigned int height, bool wrap);

/**
 * \brief Deallocates memory used by a board.
 *
 * \param[in,out] board The board to free
 */
void freeBoard(Board *board);


/**
 * \brief Switches the state of a cell: \e alive if it was \e dead, \e dead if
 *        it was \e alive.
 *
 * \param[in,out] board The board
 * \param[in]     x     The abscissa of the cell to toggle
 * \param[in]     y     The ordinate of the cell
 *
 * \return The new state of the cell (\c true means \e alive)
 */
bool toggleCell(Board *board, unsigned int x, unsigned int y);


/**
 * \brief Retrieve the rules used in this board.
 *
 * \param[in] board The board
 *
 * \return The rules of the board
 */
Rules getRules(const Board *board);

/**
 * \brief Update the rules of the board
 *
 * \param[in,out] board The board
 * \param[in]     rules The new rules
 */
void setRules(Board *board, Rules rules);


/**
 * \brief Updates the board to the next generation.
 *
 * The board is iterated, and each cell is updated according to the rules
 * determining the board.
 *
 * \param[in,out] board The board to update
 *
 * \return \c true if no error occurred (memory allocation, iteration)
 */
bool updateBoard(Board *board);

/**
 * \brief Clears the board.
 *
 * \e Kills all the cells in the board, by turning them dead.
 *
 * \param[in,out] board The board to clear
 */
void clearBoard(Board *board);


#endif // LIFE_H
