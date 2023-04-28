#pragma once

#ifndef STYLESHEET_H
#define STYLESHEET_H

#include "includes.h"
#include "resources.h"
#include "ELEMENT_ATTRIBUTES.h"

struct GStylesheet {

	SDL_Color* foreground;
	SDL_Color* background;
	SDL_Color* neutral;
	SDL_Color* accent;
	SDL_Color* frame_background_accent;
	SDL_Color* frame_background_neutral;
	Shape* shape;
	font_data* font;

};

#endif