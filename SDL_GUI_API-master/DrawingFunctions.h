#pragma once


#ifndef DRAWING_FUNCTIONS_H
#define DRAWING_FUNCTIONS_H

#include "SDL.h"

void CreateRoundedBox(SDL_Renderer* renderer, SDL_Texture* texture, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 rad,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_BlendMode blendmode);

void CreateRectangle(SDL_Renderer* renderer, SDL_Texture* texture, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_BlendMode blendmode);

void CreateLine(SDL_Renderer* renderer, SDL_Texture* texture, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 width,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_BlendMode blendmode);

void CreateEllipse(SDL_Renderer* renderer, SDL_Texture* texture, Sint16 x, Sint16 y, Sint16 radx, Sint16 rady,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_BlendMode blendmode);

void CreateTriangle(SDL_Renderer* renderer, SDL_Texture* texture, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_BlendMode blendmode);

#endif