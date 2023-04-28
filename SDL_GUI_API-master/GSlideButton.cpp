#include "GSlideButton.h"
#include "GFrame.h"

GSlideButton::GSlideButton(std::string ID, SDL_Renderer* renderer, SDL_Rect rect): GButton(ID, renderer, rect) {

	activated = false;
	positive_slide_ratio_y = 0;
	positive_slide_ratio_x = 1;

	this->limit_y_up = 0;
	this->limit_y_down = 300;

	this->limit_x_down = 300;
	this->limit_x_up = 0;
}

bool GSlideButton::isFocussable_click() {
	return true;
}

void GSlideButton::SetParameters(int x_comp, int y_comp, int limit_x_down, int limit_y_down, int limit_x_up, int limit_y_up){
	this->positive_slide_ratio_x = x_comp;
	this->positive_slide_ratio_y = y_comp;

	this->limit_x_down = limit_x_down;
	this->limit_x_up = limit_x_up;

	this->limit_y_down = limit_y_down;
	this->limit_y_up = limit_y_up;

}


bool GSlideButton::CheckEvent(SDL_Event event) {
	if (!GElement::CheckEvent(event)) return false;
	switch (event.type) {

	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
		if (activated) {
			return true;
		}
		if (CheckCursor()) {
			if (event.type == SDL_MOUSEMOTION)
				this->focussed_motion = true;
			else {
				this->focussed_click = true;
				this->activated = true;
			}
			return true;
		}
		break;
	case SDL_MOUSEBUTTONUP:
		if (activated) {
			activated = false;
			return true;
		}
		break;
	case SDL_MOUSEWHEEL:
		return false;
		break;
	default:
		return false;
		break;
	}
	return false;
}

void GSlideButton::OnMouseMotion_Clicked_left(){
	this->CoordinateUpdatingFunction();
}

void GSlideButton::CoordinateUpdatingFunction(){

	float x1 = event.motion.x - last_event.motion.x;
	float y1 = event.motion.y - last_event.motion.y;

	if (positive_slide_ratio_y == 0 && texture_rect.x + x1 >= limit_x_up && texture_rect.w + texture_rect.x + x1 <= limit_x_down) {
		Move(x1, 0);
	}
	else if (positive_slide_ratio_x == 0 && texture_rect.y + y1 >= limit_y_up && texture_rect.h + texture_rect.y + y1 <= limit_y_down) {
		Move(0, y1);
	}
	else if (positive_slide_ratio_x == positive_slide_ratio_y) {
		if (texture_rect.y + y1 >= limit_y_up && texture_rect.h + texture_rect.y + y1 <= limit_y_down &&
			texture_rect.x + x1 >= limit_x_up && texture_rect.w + texture_rect.x + x1 <= limit_x_down)
			Move(x1 > y1 ? y1 : x1, x1 > y1 ? y1 : x1);
	}
	else {

		float ratio = positive_slide_ratio_x / positive_slide_ratio_y;

		int dx, dy;

		if (x1 / y1 > ratio) {
			dx = y1 * ratio;
			dy = y1;
		}
		else if (x1 / y1 < ratio) {
			dy = x1 / ratio;
			dx = x1;
		}
		else {		
			dx = x1;
			dy = y1;
		}
		if (texture_rect.y + dy >= limit_y_up && texture_rect.h + texture_rect.y + dy <= limit_y_down &&
			texture_rect.x + dx >= limit_x_up && texture_rect.w + texture_rect.x + dx <= limit_x_down)
			this->Move(dx, dy);
	}
}
