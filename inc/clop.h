/**
 * \file "clop.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief This file defines the function to use to process command-line options.
 *
 * Considering that the program has been compiled under Unix in the current
 * directory as \a sdlife, an example of invocation can be:
 * \code{.sh}
 * $ ./sdlife -w73 -h42 -n -c8 -v
 * \endcode
 * This invocation will open a board of \c 72 cells per \c 42, without borders
 * between the cells, each cell wide of \c 8 pixels and updating synchronously
 * with the vertical refreshing of the monitor.
 */

#ifndef CLOP_H
#define CLOP_H


#include <getopt.h>
#include <stdbool.h>
#include <stdlib.h>


/**
 * The short options string.
 */
static const char *const OPTSTRING = "b:c:h:nr:R:vw:W";

/**
 * The long options array.
 */
static const struct option const LONGOPTS[] = {
	{"board-width",  required_argument, NULL, 'w'},
	{"board-height", required_argument, NULL, 'h'},
	{"border-size",  required_argument, NULL, 'b'},
	{"cell-size",    required_argument, NULL, 'c'},
	{"no-border",    no_argument,       NULL, 'n'},
	{"game-rules",   required_argument, NULL, 'R'},
	{"update-rate",  required_argument, NULL, 'r'},
	{"vsync",        no_argument      , NULL, 'v'},
	{"wrap",         no_argument      , NULL, 'W'},
	{"", 0, NULL, 0}
};

/**
 * \brief Specifies the variables to store command line options and arguments
 *        into.
 *
 * \param[in] board_width  The variable to hold the value for the width of the
 *                         board, in number of cells
 * \param[in] board_height The height of the board, in cells
 * \param[in] cell_pixels  The dimension of a square representing a cell, in
 *                         pixels
 * \param[in] update_rate  The number of times to update the board, per second
 * \param[in] border_width The width of the border separating cells
 * \param[in] use_vsync    Whether to follow vsync for board update or not --
 *                         either this or \a update_rate, but not both
 * \param[in] wrap         Sets the grid as rectangular or toroidal (i.e. a cell
 *                         crossing a wall will appear on the other side of the
 *                         board instead of just dying)
 * \param[in] game_rules   The evolution rules of the game, in \e Golly format
 *                         (\c Bm/Sn where m is the number of alive neighbors)
 */
void setvars(unsigned int *board_width,
             unsigned int *board_height,
             unsigned int *cell_pixels,
             unsigned int *update_rate,
             unsigned int *border_width,
             bool *use_vsync,
             bool *wrap,
             const char **game_rules);


/**
 * \brief Processes the provided arguments and stores the parameters into the
 *        memory zones set in \a setvars().
 *
 * \param[in] argc    The number of arguments to process
 * \param[in] argv    The vector of arguments to process
 * \param[in] optstr  The short options to look for in the vector
 * \param[in] longopt The long options
 *
 * \return \c 0 if the processing ended correctly (no unknown option was
 *         found, nor any argument was missing or invalid), a negative value
 *         otherwise
 */
int getvals(const int argc, const char *const argv[], const char *optstr, const struct option longopt[]);


#endif // CLOP_H
