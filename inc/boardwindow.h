/**
 * \file "boardwindow.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief This file contains the definition of the structure used to manage the
 *        graphical rendering of a board of the Game of Life.
 *
 * It communicates with the SDL through its abstract structure \a SDL_Window.
 */

#ifndef BOARDWINDOW_H
#define BOARDWINDOW_H


#include <SDL2/SDL_render.h>
#include <stdbool.h>

#include "board.h"

/**
 * \brief The flags used to initialize the \a SDL_Window.
 */
#define WINDOW_FLAGS   SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_FOCUS | SDL_WINDOW_INPUT_FOCUS

/**
 * \brief The (static) default flags for the renderer.
 */
#define REN_DEFT_FLAGS SDL_RENDERER_ACCELERATED
/**
 * \brief The renderer flags used to initialize the renderer, with a parameter
 *        for (optionally enabled) vertical synchronization.
 */
#define RENDERER_FLAGS(use_vsync) (use_vsync ? REN_DEFT_FLAGS | SDL_RENDERER_PRESENTVSYNC : REN_DEFT_FLAGS)

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
	struct board *board;
	/**< The board included in the window. */
	SDL_Window *win;
	/**< The technical type used by the SDL to display a window. */
	SDL_Renderer *ren;
	/**< The renderer of the SDL associated with the window. */
	unsigned int cell_pixels;
	/**< The length of one cell'srepresentation, in pixels. */
	unsigned int border_width;
	/**< The size of the gap separating the cells. */
	int sel_x,
	/**< The \c x coordinate of the currently selected cell. */
	    sel_y;
	/**< The \c y coordinate of the currently selected cell. */
};


/**
 * \brief Creates a new board window.
 *
 * \param[in] board        The board to handle
 * \param[in] cell_pixels  The dimension, in pixels, of the representation of a
 *                         single cell
 * \param[in] border_width The width of the border separating the cells
 * \param[in] window_title The title to give to the window
 * \param[in] use_vsync    Whether to synchronize the update of the board with
 *                         the monitor
 *
 * \return A new board window
 */
struct boardwindow *newBoardWindow(struct board *board,
                                   unsigned int cell_pixels,
                                   unsigned int border_width,
                                   const char *window_title, bool use_vsync);


/**
 * \brief Deallocates a board window
 *
 * \param[in,out] self The board window to free
 */
void freeBoardWindow(struct boardwindow *self);


/**
 * \brief Updates the state of a board window
 *
 * \param[in,out] self The board window to update
 *
 * \note The \e update is a distinct action from the rendering.
 */
void updateBoardWindow(struct boardwindow *self);

/**
 * \brief Renders, on display, the board window.
 *
 * \param[in] self The board window to render
 */
void renderBoardWindow(const struct boardwindow *self);


/**
 * \brief Retrieve the coordinate, in cell units, of the cell whose position is
 *        under the mouse cursor.
 *
 * \param[in]  self The board window
 * \param[out] x    The abscissa of the mouse cursor
 * \param[out] y    The ordinate of the mosue pointer
 */
void getHoverCoord(const struct boardwindow *self, int *x, int *y);


#endif // BOARDWINDOW_H
