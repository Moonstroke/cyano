#ifndef CLOP_H
#define CLOP_H

#define OPTSTRING "c:h:w:"


void setvars(unsigned int *board_width, unsigned int *board_height, unsigned int *cell_pixels);

int getvals(const int argc, const char *const argv[], const char *optstr);

#endif // CLOP_H