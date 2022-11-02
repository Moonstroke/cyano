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

#include "grid.h"



/**
 * \brief Helper macro that converts a grid dimension (in number of cells) to a
 * window dimension (in pixels).
 *
 * \note The resulting size includes borders on both sides.
 *
 * \param[in] gw   The grid window whose atributes to use
 * \param[in] size The size to convert
 *
 * \return The size in pixels of the given number of cells in the given window
 */
#define GRID_SIZE_TO_WIN_SIZE(gw, size) \
	((size) * ((gw)->cell_pixels + (gw)->border_width) + (gw)->border_width)

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
struct grid_window {
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
 * \param[out] grid_win     The grid window to initialize
 * \param[in]  grid         The grid to handle
 * \param[in]  cell_pixels  The dimension, in pixels, of the representation of a
 *                          single cell
 * \param[in]  border_width The width of the border separating the cells
 * \param[in]  window_title The title to give to the window
 *
 * \return \c 0 on success, a negative value on error
 */
int init_grid_window(struct grid_window *grid_win, struct grid *grid,
                     unsigned int cell_pixels, unsigned int border_width,
                     const char *window_title);


/**
 * \brief Deallocate a grid window
 *
 * \param[in,out] grid_win The grid window to free
 */
void free_grid_window(struct grid_window *grid_win);


/**
 * \brief Render, on display, the grid window.
 *
 * \param[in] grid_win The grid window to render
 */
void render_grid_window(const struct grid_window *grid_win);


/**
 * \brief Transforms the window coordinates to a grid cell location.
 *
 * \param[in]  grid_win    The grid window
 * \param[in]  window_x    The X window coordinate
 * \param[in]  window_y    The Y window coordinate
 * \param[out] cell_col    The column number of the cell under
 *                         (window_x,window_y)
 * \param[out] cell_row    The row number of the cell under (window_x,window_y)
 *
 * \note If either of the coordinates points over a border, \c -1 is returned in
 *       the corresponding cell location.
 */
void get_cell_loc(const struct grid_window *grid_win, int window_x,
                  int window_y, int *cell_col, int *cell_row);


/**
 * \brief Retrieve the coordinates, in cell units, of the cell whose position is
 *        under the mouse cursor.
 *
 * \param[in]  grid_win    The grid window
 * \param[out] cell_col    The column number of the hovered cell
 * \param[out] cell_row    The row number of the hovered cell
 *
 * \note If either of the coordinates points over a border, \c -1 is returned in
 *       the corresponding cell location.
 */
void get_hovered_cell_loc(const struct grid_window *grid_win, int *cell_col,
                          int *cell_row);


#endif /* GRIDWINDOW_H */
