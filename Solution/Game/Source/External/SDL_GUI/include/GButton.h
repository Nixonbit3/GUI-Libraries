#pragma once

#ifndef BUTTON_H
#define BUTTON_H

#include "GElement.h"

class GButton: public GElement {
protected:
	enum { TEXT_IMAGE };
	SDL_Rect text_rect;
	std::string text;

	virtual void OnMouseDown_left();
	virtual void OnMouseUp();
	virtual void OnMouseMotion_Hover();

public:

	enum { CLICKED_IMAGE = 1, IDLE_IMAGE, HIGHLIGHT_IMAGE };
	GButton(std::string ID, SDL_Renderer* renderer, SDL_Rect rect);

	virtual bool InitializeTextures_from_stylesheet();

	void SetText(std::string text);
	std::string GetText();

	virtual bool OnFocusLostMotion();

	virtual void operator()();

	virtual bool CheckEvent(SDL_Event e);

	void PrepareTexture();

};

#endif

