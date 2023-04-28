#pragma once
#include"GFrame.h"

class GButton;

class GFeedFrame: public GFrame {
protected:

	bool expanding_feed;
	int default_spacing_x;
	int default_spacing_y;
	Direction dirn;

public:
	enum Position { PUSH_TOP = -1, PUSH_BOTTOM = -2 };

	GFeedFrame(std::string ID, SDL_Renderer* renderer, SDL_Rect texture_rect, Direction dirn);

	void SetSpacing(int space_x, int space_y, bool propagate);
	void SetExpanding(bool state);
	bool GetExpanding();
	void NotifyChildDimensionsChanged();

	virtual void AddElement(GElement* element) {
		this->AddElement(element, PUSH_TOP);
	}
	virtual void AddElement(GElement* element, int position);

	virtual void RemoveElement(GElement* element);
	virtual void RemoveElement(int position);
	void RemoveElement(std::list<GElement*>::iterator it);

	virtual bool CheckUpdated() {
		return true;
	}

};