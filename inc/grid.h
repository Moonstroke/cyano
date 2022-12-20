/* SPDX-License-Identifier: CECILL-2.1 */
/**
 * \file "grid.h"
 * \author Joachim "Moonstroke" MARIE
 *
 * \version 0.1
 *
 * \brief This file contains the definition of a game grid and declarations of
 *        functions to manipulate it.
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
	unsigned int width; /**< The width of the grid. */
	unsigned int height; /**< The height of the grid. */
	char *cells; /**< The data of the grid cells. */
	/** The rulestring determining the evolution of the game. */
	char rule[22];
	/** A flag indicating whether the state on one side of the grid affects the
	    opposite side. */
	bool wrap;
};


/**
 * \brief Represents the state of a single cell. More expressive than \c bool
 */
enum cell_state {
	DEAD, /**< The cell is dead: inactive */
	ALIVE /**< The cell is alive: active */
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
int init_grid(struct grid *grid, unsigned int width, unsigned int height,
              bool wrap);


/**
 * \brief Flags to specify the format of a grid representation: plain text,
 *        run length-encoded, or unknown.
 */
enum grid_format {
	GRID_FORMAT_UNKNOWN, /**< Unknown grid representation format */
	GRID_FORMAT_PLAIN, /**< 1:1 textual representation of the grid */
	/** The grid is encoded by compressing runs of identical cells */
	GRID_FORMAT_RLE
};


/**
 * \brief Initialize the grid to the state described in the pattern string.
 *
 * The given string must be a representation of a Life grid, as a rectangular
 * text with dots representing dead cells and at-signs for live cells. All lines
 * must be of the same length, which is interpreted as the grid width, and the
 * number of lines gives the grid height.
 *
 * \note This function resets the grid, so callers must pass either an
 *       unititialized grid or a grid that has been passed to \c free_grid
 *       beforehand to avoid leaking memory.
 *
 * \param[out] grid   The grid to initialize
 * \param[in]  repr   The pattern, as a string of dots and at-signs
 * \param[in]  format Flag for the format of the grid representation
 * \param[in]  wrap   If \c true, set up the grid as toroidal
 *
 * \return \c 0 iff the grid was correctly initialized, a negative value
 *         otherwise
 */
int load_grid(struct grid *grid, const char *repr, enum grid_format format,
              bool wrap);


/**
 * \brief Deallocate memory used by a grid.
 *
 * \param[in,out] grid The grid to free
 */
void free_grid(struct grid *grid);


/**
 * \brief Get the state of a cell from the grid.
 *
 * \param[in] grid The game grid
 * \param[in] row   The row to get
 * \param[in] col   The column to get
 *
 * \return \c ALIVE if the cell at (row, col) is alive, or \c DEAD if the cell
 *         is dead or coordinates are invalid.
 */
enum cell_state get_grid_cell(const struct grid *grid, int row, int col);


/**
 * \brief Invert the state of a cell.
 *
 * \param[in,out] grid The grid
 * \param[in]     row  The row of the cell to toggle
 * \param[in]     col  The column of the cell
 *
 * \return The new state of the cell, or \c DEAD if the coordinates are invalid
 */
enum cell_state toggle_cell(struct grid *grid, int row, int col);


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
int update_grid(struct grid *grid);


/**
 * \brief Clear the grid.
 *
 * \e Kills all the cells in the grid, by turning them off.
 *
 * \param[in,out] grid The grid to clear
 */
void clear_grid(struct grid *grid);


/**
 * \brief Return a representation of the current state of the grid in the
 *        specified format.
 *
 * \note The returned string is allocated dynamically; callers must take care of
 * freeing it after use.
 *
 * \note If the given format is \c GRID_FORMAT_UNKNOWN, the returned
 *       representation defaults to plain-text.
 *
 * \param[in] grid   The grid
 * \param[in] format The format of the representation to generate
 *
 * \return A string representation of the current state of the grid, or \c NULL
 *         on error
 */
char *get_grid_repr(const struct grid *grid, enum grid_format format);


#endif /* grid_H */
