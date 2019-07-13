#if defined(_MSC_VER)
// Only include in windows, when in debug:
#ifdef _DEBUG
#include <crtdbg.h>
#endif

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_surface.h"
#include "SDL_image.h"


#elif defined(__linux)

#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_surface.h"
#include "SDL_image.h"

#else

#ifndef _SDL_H
#include "SDL.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#endif


#endif




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include <iostream>
#include <string>

// These are used for backward compatibility with SDL 1.2 to get the keycodes to work
//#define SDL12_SDLK_FIRST 0
#define SDL12_SDLK_NORMAL 127
#define SDL12_SDLK_LAST 255
