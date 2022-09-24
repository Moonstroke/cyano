#include "gridwindow.h"

#include <stdint.h> /* for uint8_t */
#include <string.h> /* for strncpy */
#include <SDL2/SDL_mouse.h> /* for SDL_GetMouseState */



#ifdef USE_VSYNC
# define RENDERER_FLAGS SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
#else
# define RENDERER_FLAGS SDL_RENDERER_ACCELERATED
#endif


static const uint16_t icon_data[] = {
#if ICONSIZE == 16
	/* 1 image row = two lines */
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x000f, 0x000f,
	0x000f, 0x000f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x000f, 0x000f,
	0x000f, 0x000f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x000f, 0x000f,
	0x000f, 0x000f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x000f, 0x000f,
	0x000f, 0x000f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0x000f, 0x000f, 0x000f, 0x000f, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0x000f, 0x000f, 0x000f, 0x000f, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0x000f, 0x000f, 0x000f, 0x000f, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0x000f, 0x000f, 0x000f, 0x000f, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0x000f, 0x000f, 0x000f, 0x000f, 0xfff0, 0x000f, 0x000f,
	0x000f, 0x000f, 0xfff0, 0x000f, 0x000f, 0x000f, 0x000f, 0xfff0,
	0xfff0, 0x000f, 0x000f, 0x000f, 0x000f, 0xfff0, 0x000f, 0x000f,
	0x000f, 0x000f, 0xfff0, 0x000f, 0x000f, 0x000f, 0x000f, 0xfff0,
	0xfff0, 0x000f, 0x000f, 0x000f, 0x000f, 0xfff0, 0x000f, 0x000f,
	0x000f, 0x000f, 0xfff0, 0x000f, 0x000f, 0x000f, 0x000f, 0xfff0,
	0xfff0, 0x000f, 0x000f, 0x000f, 0x000f, 0xfff0, 0x000f, 0x000f,
	0x000f, 0x000f, 0xfff0, 0x000f, 0x000f, 0x000f, 0x000f, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0
#elif ICONSIZE == 64
	/* each 8-lines block <=> 1 row in the icon image */
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x777f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x777f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f, 0x000f,
	0x000f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f, 0x777f,
	0x777f, 0x777f, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,

	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0,
	0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0, 0xfff0
#endif
};


int init_grid_window(struct grid_window *gw, struct grid *grid,
                     unsigned int cell_pixels, unsigned int border_width,
                     const char *title) {
	gw->grid = grid;
	gw->cell_pixels = cell_pixels;
	gw->border_width = border_width;
	unsigned int win_width = GRID_SIZE_TO_WIN_SIZE(gw, grid->w);
	unsigned int win_height = GRID_SIZE_TO_WIN_SIZE(gw, grid->h);

	Uint32 win_flags = SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_FOCUS
	                                    | SDL_WINDOW_INPUT_FOCUS;
	gw->win = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
	                           SDL_WINDOWPOS_CENTERED, win_width, win_height,
	                           win_flags);
	if (gw->win == NULL) {
		strncpy(gw->error_msg, SDL_GetError(), sizeof gw->error_msg);
		return -__LINE__;
	}
	gw->ren = SDL_CreateRenderer(gw->win, -1, RENDERER_FLAGS);
	if (gw->ren == NULL) {
		strncpy(gw->error_msg, SDL_GetError(), sizeof gw->error_msg);
		return -__LINE__;
	}
	gw->sel_x = gw->sel_y = -1;
	gw->error_msg[0] = '\0';

	SDL_Surface *icon = SDL_CreateRGBSurfaceFrom((uint16_t*) icon_data,
	                                             ICONSIZE, ICONSIZE, 16,
	                                             ICONSIZE * sizeof icon_data[0],
	                                             0xf000, 0x0f00, 0x00f0,
	                                             0x000f);
	if (icon != NULL) {
		SDL_SetWindowIcon(gw->win, icon);
		SDL_FreeSurface(icon);
	}

	return 0;
}


void free_grid_window(struct grid_window *gw) {
	SDL_DestroyRenderer(gw->ren);
	SDL_DestroyWindow(gw->win);
}


static inline void _draw_cell(SDL_Renderer *ren, SDL_Rect *rect, unsigned int i,
                             unsigned int j, unsigned int c,
                             unsigned int border, uint8_t r, uint8_t g,
                             uint8_t b, uint8_t a) {
	SDL_SetRenderDrawColor(ren, r, g, b, a);
	rect->x = (c + border) * i + border;
	rect->y = (c + border) * j + border;
	SDL_RenderFillRect(ren, rect);
}

void render_grid_window(const struct grid_window *gw) {
	unsigned int w = gw->grid->w,
	             h = gw->grid->h,
	             c = gw->cell_pixels,
	             b = gw->border_width;

	unsigned int i, j;
	SDL_Rect r;

	SDL_SetRenderDrawColor(gw->ren, 127, 127, 127, 255);
	SDL_RenderClear(gw->ren);
	r.w = c;
	r.h = c;
	uint8_t cell_color[] = {
		[DEAD] = 255,
		[ALIVE] = 0
	};
	for (j = 0; j < h; ++j) {
		for (i = 0; i < w; ++i) {
			uint8_t ch = cell_color[get_grid_cell(gw->grid, i, j)];
			_draw_cell(gw->ren, &r, i, j, c, b, ch, ch, ch, 255);
		}
	}
	SDL_SetRenderDrawBlendMode(gw->ren, SDL_BLENDMODE_BLEND);
	if (gw->sel_x >= 0 && gw->sel_y >= 0) {
		_draw_cell(gw->ren, &r, gw->sel_x, gw->sel_y, c, b, 127, 127, 127, 127);
	}
	SDL_RenderPresent(gw->ren);
}


void get_cell_loc(const struct grid_window *gw, int x, int y, int *i, int *j) {
	/* Remove offset for up and left border */
	x -= gw->border_width;
	y -= gw->border_width;
	unsigned int step = gw->cell_pixels + gw->border_width;
	if (x % step < gw->border_width) {
		/* Hovering a vertical border */
		*i = -1;
	} else {
		*i = x / step;
	}
	if (y % step < gw->border_width) {
		/* Hovering a horizontal border */
		*j = -1;
	} else {
		*j = y / step;
	}
}

void get_hovered_cell_loc(const struct grid_window *gw, int *i, int *j) {
	int x, y;
	SDL_GetMouseState(&x, &y);
	get_cell_loc(gw, x, y, i, j);
}
