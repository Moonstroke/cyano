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


#include <stdbool.h>



/**
 * \brief Read the content of the file of given path in a string and return it.
 *
 * If the input path is simply \c "-", text will be read from the standard
 * input stream.
 *
 * If there is a trailing newline at the end of the file, this function removes
 * it.
 *
 * \note The returned value is allocated on the heap and needs to be freed.
 *
 * \param[in] path The path to the source file, or \c "-"
 *
 * \return The content of the file, or \c NULL if an error occurred
 */
char *readFile(const char *path);


/**
 * \brief Write the given text to the file of given path.
 *
 * If the input path is simply \c "-", the text will be written to the standard
 * output stream.
 *
 * \note If the file does not exist, it will be created, otherwise its content
 *       will be overwritten.
 *
 * \param[in] path The path to the destination file, or \c "-"
 * \param[in] text The value to write in the file
 *
 * \return \c 0 on success, a negative value on error
 */
int writeFile(const char *path, const char *text);


/**
 * \brief Determines whether the given path is a regular file or not.
 *
 * \param[in] path The path to check for existence
 *
 * \return \c true iff the path names an existing regular file
 */
bool isFile(const char *path);

#endif /* FILE_IO_H */
