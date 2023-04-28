#pragma once

#ifndef BGLABEL_H
#define BGLABEL_H

#include "GLabel.h"

class GBackgroundImage : public GLabel {
public:

	GBackgroundImage(std::string ID, SDL_Renderer* renderer, SDL_Rect rect);

	bool InitializeTextures_from_stylesheet() {

		if (!GElement::InitializeTextures_from_stylesheet())
			return false;

		CreateRectangle(
			renderer,
			texture_repository[BG_IMAGE],
			0, 0, texture_rect.w, texture_rect.h, 
			this->stylesheet->frame_background_neutral->r,
			this->stylesheet->frame_background_neutral->g,
			this->stylesheet->frame_background_neutral->b,
			this->stylesheet->frame_background_neutral->a,
			SDL_BLENDMODE_BLEND
		);

		updated = true;
		return true;
	}
};

#endif
