#pragma once

#ifndef MULTILINELABEL_H
#define MULTILINELABEL_H

#include "GLabel.h"

class GMultiLineLabel: public GLabel {
private:
	int wrap_length;
public:
	GMultiLineLabel(std::string ID, SDL_Renderer* renderer, SDL_Rect rect);

	virtual void SetText(std::string text);
	void SetWrapLength(int wrap);
};



#endif
