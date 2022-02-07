/**
 * \file "boardwindow.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief This file contains the definition of the structure used to manage the
 *        graphical rendering of a board of the Game of Life.
 *
 * It communicates with the SDL through its abstract structure \c SDL_Window.
 */

#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H


#include <SDL2/SDL_render.h>
#include <stdbool.h>

#include "board.h"



/**
 * \brief default size in pixels, for the cell's representation.
 */
#define DEFAULT_CELLS_PIXELS 16

/**
 * \brief The default update rate of the board.
 */
#define DEFAULT_UPDATE_RATE 25

/**
 * \brief The default width of the border between the cells.
 */
#define DEFAULT_BORDER_WIDTH 1


/**
 * \brief The type handling the graphical display of the board.
 */
struct boardwindow {
	/** The board included in the window. */
	struct board *board;
	/** The technical type used by the SDL to display a window. */
	SDL_Window *win;
	/** The renderer of the SDL associated with the window. */
	SDL_Renderer *ren;
	/** The length of one cell's representation, in pixels. */
	unsigned int cell_pixels;
	/** The size of the gap separating the cells. */
	unsigned int border_width;
	int sel_x; /**< The \c x coordinate of the currently selected cell. */
	int sel_y; /**< The \c y coordinate of the currently selected cell. */
	char error_msg[64]; /**< The error message if an operation fails */
};


/**
 * \brief Initialize the board window with the given values.
 *
 * \param[out] boardwindow  The board window to initialize
 * \param[in]  board        The board to handle
 * \param[in]  cell_pixels  The dimension, in pixels, of the representation of a
 *                          single cell
 * \param[in]  border_width The width of the border separating the cells
 * \param[in]  window_title The title to give to the window
 * \param[in]  use_vsync    Whether to synchronize the update of the board with
 *                          the monitor
 *
 * \return \c 0 on success, a negative value on error
 */
int initBoardWindow(struct boardwindow *boardwindow, struct board *board,
                    unsigned int cell_pixels, unsigned int border_width,
                    const char *window_title, bool use_vsync);


/**
 * \brief Deallocate a board window
 *
 * \param[in,out] boardwindow The board window to free
 */
void freeBoardWindow(struct boardwindow *boardwindow);


/**
 * \brief Render, on display, the board window.
 *
 * \param[in] boardwindow The board window to render
 */
void renderBoardWindow(const struct boardwindow *boardwindow);


/**
 * \brief Retrieve the coordinates, in cell units, of the cell whose position is
 *        under the mouse cursor.
 *
 * \param[in]  boardwindow The board window
 * \param[out] i           The column number of the hovered cell
 * \param[out] j           The row number of the hovered cell
 */
void getHoveredCellLoc(const struct boardwindow *boardwindow, int *i, int *j);


#endif /* BOARDWINDOW_H */
