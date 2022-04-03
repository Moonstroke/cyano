/**
 * \file "app.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief This header provides functions that define the main steps in the
 *        execution of the program: initialization, running, termination.
 */
#ifndef APP_H
#define APP_H


#include <stdbool.h>

#include "gridwindow.h"



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
 * This invocation will open a grid of \c 72 cells per \c 42, without borders
 * between the cells, each cell wide of \c 8 pixels and updating synchronously
 * with the vertical refreshing of the monitor.
 *
 * \param[in]  argc         The number of arguments
 * \param[in]  argv         The command-line arguments
 *
 * \param[out] grid_width   The number of columns in the app's grid
 * \param[out] grid_height  The number of rows in the app's grid
 * \param[out] wrap         Whether the grid's borders connect
 * \param[out] game_rules   The name or \e Golly representation of the rules that
 *                          govern the evolution of the grid
 *
 * \param[out] cell_pixels  The dimension in pixels of a single cell in the
 *                          window
 * \param[out] border_width The thickness of the grid separating the cells
 * \param[out] update_rate  The number of times the grid evolves per second
 * \param[out] use_vsync    Whether the update rate is fixed to the monitor's
 *                          refresh rate
 * \param[out] in_file      The path to the file from which to read
 * \param[out] out_file     The pat to the file where to write
 *
 * \return \c 0 on success
 */
int parseCommandLineArgs(int argc, char **argv, unsigned int *grid_width,
                         unsigned int *grid_height, bool *wrap,
                         const char **game_rules, unsigned int *cell_pixels,
                         unsigned int *border_width, unsigned int *update_rate,
                         bool *use_vsync, const char **in_file,
                         const char **out_file);


/**
 * Run the main event loop
 *
 * \param[in] gridwindow The application's gridwindow to run
 * \param[in] update_rate The number of times the grid evolves per second
 * \param[in] use_vsync   Whether the update rate is fixed to the monitor's
 *                        refresh rate
 * \param[in] repr        The representation of the initial state to reset to
 * \param[in] out_file    The path to the file where to write the grid state
 */
void runApp(struct gridwindow *gridwindow, unsigned int update_rate,
           bool use_vsync, const char *repr, const char *out_file);


/**
 * Destroy and terminate the application.
 */
void terminateApp(void);


#endif /* APP_H */
