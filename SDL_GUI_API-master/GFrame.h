#pragma once

#ifndef FRAME_H
#define FRAME_H

#include "GElement.h"

class GBackgroundImage;
struct ELEMENT_ATTRIBUTES;

class GFrame :public GElement {
protected:

	int containing_scrollfactor_x;
	int containing_scrollfactor_y;

	bool containing_scrollable_x;
	bool containing_scrollable_y;

	int scrolled_amount_x;
	int scrolled_amount_y;

	int scroll_limit_x;
	int scroll_limit_y;

	GBackgroundImage* frame_bg;

	std::list<GElement*> frame_elements;
	std::vector<GElement*> frame_free_elements;

	virtual void OnMouseDown_left();
	virtual void OnMouseDown_right();
	virtual void OnMouseUp();
	virtual void OnMouseMotion_Hover();
	virtual void OnMouseMotion_Clicked_left();
	virtual void OnKeyPressed();
	virtual void OnMouseWheel();

	virtual bool CheckCursor();

public:

	GFrame(std::string ID, SDL_Renderer* renderer, SDL_Rect rect);

	virtual bool InitializeTextures_from_stylesheet();
	void NotifyChildDimensionsChanged();

	virtual bool CheckUpdated();

	void AttachStylesheet(GStylesheet* stylesheet);

	virtual void AddElement(GElement* element);	
	void ActivateElement(GElement* element);
	void DeactivateElement(GElement* element);

	GElement* GetElementByID(std::string ID);

	virtual void AddElement(GElement* element, int index);
	void AddBackground(GBackgroundImage* background);
	void RemoveElement(GElement* element);

	virtual void Scroll_Containing(Direction dirn, int amount);

	virtual bool OnFocusLostMotion();
	virtual bool OnFocusLostClick();
	virtual bool OnFocusGainedMotion();
	virtual bool OnFocusGainedClick();


	virtual bool isFocussable_click();

	GElement* GetFocussedElementMotion();
	GElement* GetFocussedElementClick();

	void SetFocussedElementClick(GElement* element);
	void SetFocussedElementMotion(GElement* element);

	virtual void DrawTexture();
	virtual void PrepareTexture();

	virtual bool CheckEvent(SDL_Event event);

	void SetContainingScrollable(bool state_x, bool state_y);
	void SetContainingScrollFactors(int factor_x, int factor_y);

	~GFrame();
};


#endif
