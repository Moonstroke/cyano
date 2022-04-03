/**
 * \file "gridwindow.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief This file contains the definition of the structure used to manage the
 *        graphical rendering of a grid of the Game of Life.
 *
 * It communicates with the SDL through its abstract structure \c SDL_Window.
 */

#ifndef GRIDWINDOW_H
#define GRIDWINDOW_H


#include <SDL2/SDL_render.h>
#include <stdbool.h>

#include "grid.h"



/**
 * \brief default size in pixels, for the cell's representation.
 */
#define DEFAULT_CELLS_PIXELS 16

/**
 * \brief The default update rate of the grid.
 */
#define DEFAULT_UPDATE_RATE 25

/**
 * \brief The default width of the border between the cells.
 */
#define DEFAULT_BORDER_WIDTH 1


/**
 * \brief The type handling the graphical display of the grid.
 */
struct gridwindow {
	/** The grid included in the window. */
	struct grid *grid;
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
 * \brief Initialize the grid window with the given values.
 *
 * \param[out] gridwindow  The grid window to initialize
 * \param[in]  grid        The grid to handle
 * \param[in]  cell_pixels  The dimension, in pixels, of the representation of a
 *                          single cell
 * \param[in]  border_width The width of the border separating the cells
 * \param[in]  window_title The title to give to the window
 * \param[in]  use_vsync    Whether to synchronize the update of the grid with
 *                          the monitor
 *
 * \return \c 0 on success, a negative value on error
 */
int initGridWindow(struct gridwindow *gridwindow, struct grid *grid,
                    unsigned int cell_pixels, unsigned int border_width,
                    const char *window_title, bool use_vsync);


/**
 * \brief Deallocate a grid window
 *
 * \param[in,out] gridwindow The grid window to free
 */
void freeGridWindow(struct gridwindow *gridwindow);


/**
 * \brief Render, on display, the grid window.
 *
 * \param[in] gridwindow The grid window to render
 */
void renderGridWindow(const struct gridwindow *gridwindow);


/**
 * \brief Transforms the window coordinates to a grid cell location.
 *
 * \param[in]  gridwindow The grid window
 * \param[in]  x           The X window coordinate
 * \param[in]  y           The Y window coordinate
 * \param[out] i           The column number of the cell under (x,y)
 * \param[out] j           The row number of the cell under (x,y)
 *
 * \note If either of the coordinates points over a border, \c -1 is returned in
 *       the corresponding cell location.
 */
void getCellLoc(const struct gridwindow *gridwindow, int x, int y, int *i,
                  int *j);


/**
 * \brief Retrieve the coordinates, in cell units, of the cell whose position is
 *        under the mouse cursor.
 *
 * \param[in]  gridwindow The grid window
 * \param[out] i           The column number of the hovered cell
 * \param[out] j           The row number of the hovered cell
 *
 * \note If either of the coordinates points over a border, \c -1 is returned in
 *       the corresponding cell location.
 */
void getHoveredCellLoc(const struct gridwindow *gridwindow, int *i, int *j);


#endif /* GRIDWINDOW_H */
