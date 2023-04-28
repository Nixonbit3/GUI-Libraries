#include "GScrollBar.h"
#include "GSlideButton.h"

void GScrollBar::ChangeContext(GFrame * linked_frame){

	float ratio_x = (float)linked_frame->GetWidth() / (float)this->GetWidth();
	float ratio_y = (float)linked_frame->GetHeight() / (float)this->GetHeight();

	if (this->slide_button) {
		delete this->slide_button;
	}
}
