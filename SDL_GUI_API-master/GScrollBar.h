#pragma once

#ifndef SCROLLBAR_H
#define SCROLLBAR_H

#include "GFrame.h"
#include "Includes.h"

class GSlideButton;

class GScrollBar : public GFrame {
protected:
	GFrame* linked_frame;
	GSlideButton* slide_button;

public:

	GScrollBar(std::string id, SDL_Renderer* renderer, SDL_Rect rect);
	void ChangeContext(GFrame* linked_frame);
	void AddElement(GElement* element);
};



#endif