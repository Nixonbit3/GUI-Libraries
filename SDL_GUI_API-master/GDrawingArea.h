#pragma once

#ifndef DRAWING_AREA_H
#define DRAWING_AREA_H

#include "GElement.h"

class GDrawingArea: public GElement {
protected:
	bool drawing;

	SDL_Color* background_color;
	SDL_Color pen_color;
	Shape pen_shape;
	SDL_Texture* pen_brush;
	SDL_Rect offset_rect;

	virtual void OnMouseDown_left();
	virtual void OnMouseMotion_Clicked_left();
	virtual void OnMouseMotion_Clicked_right() {
		;
	}
	virtual void OnKeyPressed();
	virtual void OnMouseWheel() {
		;
	}
	virtual void OnMouseUp();

public:
	enum {ACTIVE_DRAWING};
	enum Corner {TOP_LEFT, BOTTOM_RIGHT};
	GDrawingArea(std::string ID, SDL_Renderer* renderer, SDL_Rect rect);

	bool CheckEvent(SDL_Event event);

	virtual void PrepareTexture();
	void ExpandTexture(int w_inc, int h_inc, Corner corner);
	void SetPenColor(SDL_Color color);
	void SetPenShape(Shape shape);

	bool isFocussable_click();

};



#endif
