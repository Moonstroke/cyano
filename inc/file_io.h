/**
 * \file "file_io.h"
 * \author joH1
 *
 * \version 0.1
 *
 * \brief This file defines a set of functions to encapsulate the communication
 *        with files from their paths.
 */
#ifndef FILE_IO_H
#define FILE_IO_H



/**
 * \brief Read the content of the file of given path in a string and return it.
 *
 * If there is a trailing newline at the end of the file, this function removes
 * it.
 *
 * \note The returned value is allocated on the heap and needs to be freed.
 *
 * \param[in] path The path to the source file
 *
 * \return The content of the file, or \c NULL if an error occurred
 */
char *readFile(const char *path);


/**
 * \brief Write the given text to the file of given path.
 *
 * \note If the file does not exist, it will be created, otherwise its content
 *       will be overwritten.
 * \param[in] path The path to the destination file
 * \param[in] text The value to write in the file
 *
 * \return \c 0 on success, a negative value on error
 */
int writeFile(const char *path, const char *text);


#endif /* FILE_IO_H */
