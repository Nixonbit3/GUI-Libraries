#pragma once

#ifndef RESOURCES_H
#define RESOURCES_H

#include "SDL/include/SDL.h"
#include "SDL_ttf/include/SDL_ttf.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "SDL_mixer/include/SDL_mixer.h"
#include "SDL_image/include/SDL_Image.h"
#include "SDL_GUI/include/SDL2_gfxPrimitives.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <string>
#include <map>
#include <list>
#include "Functor.h"
#include "DrawingFunctions.h"


enum ShapeType {
	SHAPE_ELLIPSE,
	SHAPE_RECTANGLE,
	SHAPE_ROUND_RECTANGLE
};

struct ShapeEllipse {
	ShapeType shape_type;
	Sint16 radx;
	Sint16 rady;
};

struct ShapeRoundRectangle {
	ShapeType shape_type;
	Sint16 w;
	Sint16 h;
	Sint16 rad;
};

struct ShapeRectangle {
	ShapeType shape_type;
	Sint16 w;
	Sint16 h;
};

union Shape {
	ShapeType shape_type;
	ShapeEllipse ellipse;
	ShapeRectangle rectangle;
	ShapeRoundRectangle round_rectangle;
};

struct font_data {
	TTF_Font* face;
	SDL_Color* color;
};

#include "GStylesheet.h"


SDL_Rect rect_maker(int x, int y, int w, int h);

SDL_Texture* load_image_from_path(SDL_Renderer* renderer, std::string path);

SDL_Texture* load_texture_from_text(SDL_Renderer* renderer, font_data* fontdata, std::string text);

SDL_Texture* load_texture_from_multiline_text(SDL_Renderer* renderer, font_data* fontdata, std::string text, int wrap_x);

SDL_Rect get_rect_from_texture(SDL_Texture* texture);

void fill_texture_with_color(SDL_Renderer*, SDL_Texture*, Uint8, Uint8, Uint8, Uint8);

void  expand_rect(SDL_Rect* rect, int x, int y, int w, int h);

void expand_texture(SDL_Renderer* renderer, SDL_Texture** tex, int x1, int y1, int x2, int y2);

enum Direction {
	DIRECTION_X,
	DIRECTION_Y
};


extern SDL_BlendMode PRESERVE_DST_ALPHA;
extern SDL_BlendMode PRESERVE_SRC_ALPHA;

extern GStylesheet DEFAULT_STYLESHEET;

extern SDL_Color red;
extern SDL_Color blue;
extern SDL_Color green;
extern SDL_Color black;
extern SDL_Color white;

#endif