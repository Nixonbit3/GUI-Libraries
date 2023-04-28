#pragma once

#ifndef SLIDE_BUTTON_H
#define SLIDE_BUTTON_H

#include "Includes.h"
#include "GButton.h"
class GFrame;

class GSlideButton : public GButton {
protected:
	float positive_slide_ratio_x;
	float positive_slide_ratio_y;
	int limit_y_up;
	int limit_y_down;
	int limit_x_up;
	int limit_x_down;
	bool activated;

	virtual void OnMouseMotion_Clicked_left();
	virtual void CoordinateUpdatingFunction();

public:
	GSlideButton(std::string ID, SDL_Renderer* renderer, SDL_Rect rect);

	bool isFocussable_click();

	void SetParameters(int x_comp, int y_comp, int limit_x_down, int limit_y_down, int limit_x_up, int limit_y_up);
	bool CheckEvent(SDL_Event event);
	int GetSlideRatio(int* x, int* y);

};




#endif
