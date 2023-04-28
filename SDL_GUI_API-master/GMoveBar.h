#pragma once

#ifndef MOVEBAR_H
#define MOVEBAR_H

#include "GButton.h"

class MoveBar : public GButton{
protected:
	virtual void OnMouseMotion_Clicked_Left();
public:
	MoveBar(std::string ID, SDL_Renderer* renderer, SDL_Texture* idle_image, SDL_Texture* highlight_image, SDL_Texture* clicked_image, SDL_Rect rect, Functor* move_func);
	bool CheckEvent(SDL_Event event);

	void OnFocusLostMotion();
	void OnFocusLostClick();

	bool isFocussable_motion();
	bool isFocussable_click();
};
#endif