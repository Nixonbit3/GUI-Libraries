#include "GBackgroundImage.h"

GBackgroundImage::GBackgroundImage(std::string ID, SDL_Renderer* renderer, SDL_Rect rect) :
	GLabel(ID, renderer, rect) {
	type = "BG_IMAGE";
	scrollable_x = false;
	scrollable_y = false;
}
