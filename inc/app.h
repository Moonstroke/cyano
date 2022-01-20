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
 *
 * \return \c 0 on success
 */
int parseCommandLineArgs(int argc, char **argv, unsigned int *board_width,
                         unsigned int *board_height, bool *wrap,
                         const char **game_rules, unsigned int *cell_pixels,
                         unsigned int *border_width, unsigned int *update_rate,
                         bool *use_vsync);


/**
 * Run the main event loop
 *
 * \param[in] boardwindow The application's boardwindow to run
 * \param[in] update_rate The number of times the board evolves per second
 * \param[in] use_vsync   Whether the update rate is fixed to the monitor's
 *                        refresh rate
 */
void runApp(struct boardwindow *boardwindow, unsigned int update_rate,
           bool use_vsync);


/**
 * Destroy and terminate the application.
 */
void terminateApp(void);


#endif /* APP_H */
