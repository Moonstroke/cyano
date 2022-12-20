/* SPDX-License-Identifier: CECILL-2.1 */
/**
 * \file "app.h"
 * \author Joachim "Moonstroke" MARIE
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
int init_app(void);


/**
 * Extract the options and values passed to the program in the given variables.
 *
 * Considering that the program is being run in a sh-like environment in the
 * current directory as \c cyano, an example of invocation can be:
 * \code{.sh}
 * $ ./cyano -w73 -h42 -n -c8 -v
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
 * \param[out] game_rule    The name of the rule or rulestring that govern the
 *                          evolution of the grid
 *
 * \param[out] cell_pixels  The dimension in pixels of a single cell in the
 *                          window
 * \param[out] border_width The thickness of the grid separating the cells
 * \param[out] update_rate  The number of times the grid evolves per second
 * \param[out] in_file      The path to the file from which to read
 * \param[out] out_file     The pat to the file where to write
 * \param[out] format       The grid representation format in the input file
 *
 * \return \c 0 on success
 */
int parse_cmdline(int argc, char **argv, unsigned int *grid_width,
                  unsigned int *grid_height, bool *wrap, const char **game_rule,
                  unsigned int *cell_pixels, unsigned int *border_width,
                  unsigned int *update_rate, const char **in_file,
                  const char **out_file, enum grid_format *format);


/**
 * Run the main event loop
 *
 * \param[in] gridwindow      The application's gridwindow to run
 * \param[in] update_rate     The number of times the grid evolves per second
 * \param[in] repr            A representation of the initial state to reset to
 * \param[in] format          The format of the \p repr, RLE or plain text
 * \param[in] out_file        The path to the file where to write the grid state
 * \param[in] out_file_format The format of the output file
 */
void run_app(struct grid_window *gridwindow, unsigned int update_rate,
             const char *repr, enum grid_format format, const char *out_file,
             enum grid_format out_file_format);


/**
 * Destroy and terminate the application.
 */
void terminate_app(void);


#endif /* APP_H */
