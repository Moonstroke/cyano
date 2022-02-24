#ifndef APP_H
#define APP_H


#include <stdbool.h>

#include "boardwindow.h"



/**
 * Initialize the context of the application.
 *
 * @return \c 0 on success
 */
int initApp(void);


/**
 * Extract the options and values passed to the program in the given variables.
 *
 * Considering that the program is being run in a sh-like environment in the current
 * directory as \c sdlife, an example of invocation can be:
 * \code{.sh}
 * $ ./sdlife -w73 -h42 -n -c8 -v
 * \endcode
 * This invocation will open a board of \c 72 cells per \c 42, without borders
 * between the cells, each cell wide of \c 8 pixels and updating synchronously
 * with the vertical refreshing of the monitor.
 *
 * \param[in]  argc         The number of arguments
 * \param[in]  argv         The command-line arguments
 *
 * \param[out] board_width  The number of columns in the app's board
 * \param[out] board_height The number of rows in the app's board
 * \param[out] wrap         Whether the board's borders connect
 * \param[out] game_rules   The name or \e Golly representation of the rules that
 *                          govern the evolution of the board
 *
 * \param[out] cell_pixels  The dimension in pixels of a single cell in the
 *                          window
 * \param[out] border_width The thickness of the grid separating the cells
 * \param[out] update_rate  The number of times the board evolves per second
 * \param[out] use_vsync    Whether the update rate is fixed to the monitor's
 *                          refresh rate
 * \param[out] file         The file with which to communicate (reading, writing)
 *
 * \return \c 0 on success
 */
int parseCommandLineArgs(int argc, char **argv, unsigned int *board_width,
                         unsigned int *board_height, bool *wrap,
                         const char **game_rules, unsigned int *cell_pixels,
                         unsigned int *border_width, unsigned int *update_rate,
                         bool *use_vsync, const char **file);


/**
 * Run the main event loop
 *
 * \param[in] boardwindow The application's boardwindow to run
 * \param[in] update_rate The number of times the board evolves per second
 * \param[in] use_vsync   Whether the update rate is fixed to the monitor's
 *                        refresh rate
 * \param[in] repr        The representation of the initial state to reset to
 * \param[in] out_file    The path to the file where to write the board state
 */
void runApp(struct boardwindow *boardwindow, unsigned int update_rate,
           bool use_vsync, const char *repr, const char *out_file);


/**
 * Destroy and terminate the application.
 */
void terminateApp(void);


#endif /* APP_H */
