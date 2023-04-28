#pragma once

#ifndef TEXTFIELD_H
#define TEXTFIELD_H
#include "GElement.h"

#include "GFrame.h"

class GTextField :public GElement {
protected:
	bool active;
	int text_width;
	int text_height;

	int texture_padding;

	SDL_Rect text_rect;

	SDL_Rect offset_rect;
	std::string typed_text;

	virtual void OnKeyPressed();
	virtual void OnMouseDown_left();
	virtual void OnMouseMotion_Hover();
	virtual void OnMouseUp();

public:

	enum { CLICKED_IMAGE, IDLE_IMAGE, HIGHLIGHT_IMAGE, TEXT_IMAGE };
	GTextField(std::string ID, SDL_Renderer* renderer, SDL_Rect rect);

	void SetTextRect(SDL_Rect rect);

	virtual bool InitializeTextures_from_stylesheet();

	bool OnFocusLostClick();
	bool OnFocusLostMotion();
	bool OnFocusGainedClick();

	bool isFocussable_click();

	virtual void PrepareTexture();
	virtual void AttachStylesheet(GStylesheet* stylesheet);

	bool CheckEvent(SDL_Event event);

	void ScrollOffset(int direction);
	virtual void SetText(std::string text);
	virtual void SetText();
	std::string GetText();

	void SetTextPadding(int padding);
	void SetTexturePadding(int padding);

};

#endif