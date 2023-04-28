#pragma once

#ifndef LABEL_H
#define LABEL_H

#include "GElement.h"

class GLabel : public GElement {
protected:
	enum { TEXT_IMAGE = 0 };
	SDL_Rect text_rect;
	std::string text;

public:

	enum { BG_IMAGE = 1 };
	GLabel(std::string ID, SDL_Renderer* renderer, SDL_Rect texture_rect);

	bool CheckEvent();
	virtual void PrepareTexture();

	void SetTextRect(SDL_Rect rect);

	virtual bool InitializeTextures_from_stylesheet();

	virtual void SetText(std::string text);
	std::string GetText();

};

#endif
