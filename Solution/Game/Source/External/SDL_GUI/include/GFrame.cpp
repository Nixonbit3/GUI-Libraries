//new
#include "Includes.h"
#include "GFrame.h"
#include "Containers.h"
#include "ELEMENT_ATTRIBUTES.h"
#include "GWindow.h"

GFrame::GFrame(std::string ID, SDL_Renderer* renderer, SDL_Rect rect) : GElement(ID, renderer, rect) {

	type = "FRAME";

	this->containing_scrollable_x = true;
	this->containing_scrollable_y = true;

	frame_bg = NULL;

	this->stylesheet = NULL;
	focussed_element_click = NULL;
	focussed_element_motion = NULL;

	scrolled_amount_x = 0;
	scrolled_amount_y = 0;
	scroll_limit_x = 0;
	scroll_limit_y = 0;
}

bool GFrame::InitializeTextures_from_stylesheet() {

	if (!this->stylesheet) return false;

	if (this->frame_bg) {
		delete frame_bg;
	}

	GBackgroundImage* bg_img = new GBackgroundImage(this->GetID() + "_bg", renderer, rect_maker(0, 0, 
		scroll_limit_x > texture_rect.w ? scroll_limit_x : texture_rect.w, scroll_limit_y > texture_rect.h ? scroll_limit_y : texture_rect.h));
	bg_img->AttachStylesheet(stylesheet);
	this->AddBackground(bg_img);

	updated = true;
	return true;
}

void GFrame::NotifyChildDimensionsChanged() {

	this->Scroll_Containing(DIRECTION_X, -scrolled_amount_x);
	this->Scroll_Containing(DIRECTION_Y, -scrolled_amount_y);

	float distance_y = 0;
	float distance_x = 0;

	for (GElement* element : frame_elements) {
		distance_y = (element->GetY() + element->GetHeight() - texture_rect.h) / element->GetScrollFactor_y();
		distance_x = (element->GetX() + element->GetWidth() - texture_rect.w) / element->GetScrollFactor_x();

		if (distance_y > scroll_limit_y) scroll_limit_y = distance_y * (distance_y > 0);
		if (distance_x > scroll_limit_x) scroll_limit_x = distance_x * (distance_x > 0);
	}

	this->Scroll_Containing(DIRECTION_X, scrolled_amount_x);
	this->Scroll_Containing(DIRECTION_Y, scrolled_amount_y);

}

bool GFrame::CheckUpdated(){
	if (GElement::CheckUpdated()) {
		return true;
	}
	for (GElement* elem : frame_elements) {
		if (elem->CheckUpdated()) {
			this->updated = true;
			return true;
		}
	}
	return false;
}

bool GFrame::CheckEvent(SDL_Event event) {

	if (!GElement::CheckEvent(event)) return false;

	switch (event.type) {
	case SDL_MOUSEWHEEL:
		if (focussed_motion || focussed_click || scrollable_x ||scrollable_y) return true;
		break;
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		if (CheckCursor()) {
			if (event.type == SDL_MOUSEMOTION) 
				this->focussed_motion = true;
			else if (event.type == SDL_MOUSEBUTTONDOWN && isFocussable_click()) 
				this->focussed_click = true;
			return true;
		}
		return false;
		break;
	case SDL_KEYDOWN:
	case SDL_KEYUP:
		return true;
		break;
	default:
		return false;
	}
	return false;
}

void GFrame::OnMouseDown_left(){

	GElement* prev_fec = focussed_element_click;
	focussed_element_click = NULL;

	if (prev_fec && prev_fec != focussed_element_motion) {
		prev_fec->OnFocusLostClick();
		if (prev_fec->GetAttributes()->HIDE_ON_CLICKED_OUT)
			this->DeactivateElement(prev_fec);
	}
	
	if (focussed_element_motion) {

		if (focussed_element_motion->CheckEvent(event)) {
			focussed_element_motion->HandleEvent();

			if (focussed_element_motion->isFocussable_click()) 
				focussed_element_click = focussed_element_motion;
			else {
				focussed_element_click = NULL;
				return;
			}

			if (focussed_element_click->GetAttributes()->PUSH_TO_TOP_ON_CLICKED_IN && !focussed_element_click->GetAttributes()->FIXED_POSITION) {
				bool inserted = false;
				bool removed = false;
				for (auto it = frame_elements.begin(); it != frame_elements.end(); ++it) {

					if ((*it) == focussed_element_click && it == frame_elements.begin())
						break;
					else if (*it == focussed_element_click) {
						if (!inserted) break;
						it = frame_elements.erase(it);
						removed = true;
					}
					else if ((*it) != focussed_element_click && !(*it)->GetAttributes()->FIXED_POSITION) {
						frame_elements.insert(it, focussed_element_click);
						inserted = true;
					}

					if (inserted && removed)
						break;
				}
			}
		}
	}
}

void GFrame::OnMouseDown_right(){
	;
}

void GFrame::OnMouseUp(){
	GElement::OnMouseUp();
	if (focussed_element_click && focussed_element_click->CheckEvent(event)) focussed_element_click->HandleEvent();
	else if (focussed_element_motion && focussed_element_motion->CheckEvent(event)) focussed_element_motion->HandleEvent();
	return;
}

void GFrame::OnMouseMotion_Hover(){

	GElement::OnMouseMotion_Hover();
	GElement* element_at_level;
	const ELEMENT_ATTRIBUTES* element_at_level_attr;
	GElement* prev_fem = focussed_element_motion;

	bool element_found = false;

	for (auto it = frame_elements.begin(); it != frame_elements.end(); it++) {
		element_at_level = *it;
		element_at_level_attr = element_at_level->GetAttributes();

		if (element_at_level->CheckEvent(event)) {

			focussed_element_motion = element_at_level;
			element_found = true;
			focussed_element_motion->HandleEvent();
			if (prev_fem && prev_fem != focussed_element_motion) {
				prev_fem->OnFocusLostMotion();
				if (prev_fem->GetAttributes()->HIDE_ON_CURSOR_OUT) {
					prev_fem->SetEnabled(false);
				}
			}

			if (element_at_level_attr->PUSH_TO_TOP_ON_CURSOR_IN && !element_at_level_attr->FIXED_POSITION) {
				bool inserted = false;
				bool removed = false;
				for (auto it = frame_elements.begin(); it != frame_elements.end(); ++it) {

					if ((*it) == focussed_element_click && it == frame_elements.begin())
						break;
					else if (*it == focussed_element_click) {
						if (!inserted) break;
						it = frame_elements.erase(it);
						removed = true;
					}
					else if ((*it) != focussed_element_click && !(*it)->GetAttributes()->FIXED_POSITION) {
						frame_elements.insert(it, focussed_element_click);
						inserted = true;
					}

					if (inserted && removed)
						break;
				}
			}
			break;
		}
		else if (element_at_level_attr->DISABLE_FRAMES_AT_LESSER_LEVEL) {
			break;
		}
	}

	if (!element_found) {
		this->focussed_element_motion = NULL;
		if (prev_fem) {
			prev_fem->OnFocusLostMotion();
		}
	}
}

void GFrame::OnMouseMotion_Clicked_left(){
	if (this->focussed_element_click) {
		if (focussed_element_click->CheckEvent(event)) {
			focussed_element_click->HandleEvent();
		}
	}
}

void GFrame::OnKeyPressed(){

	GElement::OnKeyPressed();
	if (focussed_element_click) {
		if (focussed_element_click->CheckEvent(event)) {
			focussed_element_click->HandleEvent();
			return;
		}
	}

	if (!(scrollable_x || scrollable_y)) return;
	switch (event.key.keysym.sym) {
	case SDLK_LEFT:
		if (containing_scrollable_x) Scroll_Containing(DIRECTION_X, 1);
		break;
	case SDLK_RIGHT:
		if (containing_scrollable_x) Scroll_Containing(DIRECTION_X, -1);
		break;
	case SDLK_DOWN:
		if (containing_scrollable_y) Scroll_Containing(DIRECTION_Y, -1);
		break;
	case SDLK_UP:
		if (containing_scrollable_y) Scroll_Containing(DIRECTION_Y, 1);			
		break;

	}

}

void GFrame::OnMouseWheel(){

	GElement::OnMouseWheel();
	if (focussed_element_motion) {
		if (focussed_element_motion->CheckEvent(event)) {
			focussed_element_motion->HandleEvent();
			return;
		}
	}

	if (containing_scrollable_x)
		Scroll_Containing(DIRECTION_X, event.wheel.x);
	if (containing_scrollable_y)
		Scroll_Containing(DIRECTION_Y, event.wheel.y);

}

bool GFrame::CheckCursor()
{
	if (GElement::CheckCursor()) {
		return true;
	}
	else {
		if (frame_free_elements.size()) {
			for (GElement* elem : frame_free_elements) {
				if (elem->CheckEvent(event)) {
					return true;
				}
			}
		}
	}
	return false;
}

void GFrame::AttachStylesheet(GStylesheet* stylesheet) {

	if (!stylesheet) return;
	if (stylesheet_locked) return;

	this->stylesheet = stylesheet;
	this->InitializeTextures_from_stylesheet();

	for (GElement* element: frame_elements) {
		element->AttachStylesheet(this->stylesheet); 
	}
}

void GFrame::ActivateElement(GElement* element) {

	element->SetEnabled(true);
	if (element->GetAttributes()->PUSH_TO_TOP_ON_ACTIVATE && !element->GetAttributes()->FIXED_POSITION) {
		bool inserted = false;
		bool removed = false;
		for (auto it = frame_elements.begin(); it != frame_elements.end(); ++it) {

			if ((*it) == element && it == frame_elements.begin())
				break;
			else if (*it == element) {
				if (!inserted) break;
				it = frame_elements.erase(it);
				removed = true;
			}
			else if ((*it) != element && !(*it)->GetAttributes()->FIXED_POSITION) {
				frame_elements.insert(it, element);
				inserted = true;
			}

			if (inserted && removed)
				break;
		}
	}
}

void GFrame::DeactivateElement(GElement* element) {

	if (this->focussed_element_click == element) focussed_element_click = NULL;
	if (this->focussed_element_motion == element) focussed_element_motion = NULL;

	if (element->GetAttributes()->HIDE_ON_DEACTIVATE) {

		element->SetEnabled(false);
	}
	else element->SetEventsEnabled(false);

}

void GFrame::AddElement(GElement* element) {

	if (element->GetParentElement()) return;

	if (element->GetAttributes()->FIXED_POSITION) {

		bool inserted = false;
		for (std::list<GElement*>::iterator it = frame_elements.begin(); it != frame_elements.end(); ++it) {
			if (!(*it)->GetAttributes()->FIXED_POSITION) {
				frame_elements.insert(it, (element));
				inserted = true;
			}
		}
		if (!inserted) {
			frame_elements.push_back(element);
		}
	}

	else {
		frame_elements.push_back(element);
	}

	if (element->isFrameFree()) {
		this->frame_free_elements.push_back(element);
	}

	if (!element->GetStylesheet() && this->stylesheet) {
		element->AttachStylesheet(this->stylesheet);
	}
	element->SetParentElement(this);
	element->SetParentWindow(this->parent_window);

	float distance_y = 0;
	float distance_x = 0;

	distance_y = (element->GetY() + element->GetHeight() - texture_rect.h) / element->GetScrollFactor_y();
	distance_x = (element->GetX() + element->GetWidth() - texture_rect.w) / element->GetScrollFactor_x();

	if (distance_y > scroll_limit_y) scroll_limit_y = distance_y * (distance_y > 0);
	if (distance_x > scroll_limit_x) scroll_limit_x = distance_x * (distance_x > 0);

}

void GFrame::AddElement(GElement* element, int index) {
	
	if (element->GetParentElement()) return;

	std::list<GElement*>::iterator it1;
	std::list<GElement*>::iterator it2;

	it1 = frame_elements.begin();
	it2 = frame_elements.begin();

	std::advance(it1, index - 1);
	std::advance(it2, index);

	if (index == 0) {
		if (element->GetAttributes()->FIXED_POSITION)
			frame_elements.push_front(element);
	}
	else if (index == frame_elements.size() - 1) {
		if (!(*frame_elements.rbegin())->GetAttributes()->FIXED_POSITION)
			frame_elements.push_back(element);
	}
	else if (((*it1)->GetAttributes()->FIXED_POSITION && element->GetAttributes()->FIXED_POSITION) ||
			(!(*it2)->GetAttributes()->FIXED_POSITION && !element->GetAttributes()->FIXED_POSITION)) {
		frame_elements.insert(it2, element);
	}
	else if (element->GetAttributes()->FIXED_POSITION) {
		frame_elements.push_front(element);
	}
	else {
		frame_elements.push_back(element);
	}

	element->SetParentElement(this);
	element->SetParentWindow(this->parent_window);

	if (!element->GetStylesheet() && this->stylesheet) {
		element->AttachStylesheet(this->stylesheet);
	}

	float distance_y = 0;
	float distance_x = 0;

	distance_y = (element->GetY() + element->GetHeight() - texture_rect.h) / scrollfactor_y;
	distance_x = (element->GetX() + element->GetWidth() - texture_rect.w) / scrollfactor_x;

	if (distance_y > scroll_limit_y) scroll_limit_y = distance_y * (distance_y > 0);
	if (distance_x > scroll_limit_x) scroll_limit_x = distance_x * (distance_x > 0);

}

void GFrame::AddBackground(GBackgroundImage* bg_img) {

	if (bg_img->GetParentElement()) return;

	this->frame_bg = bg_img;
	bg_img->SetParentElement(this);
	bg_img->SetParentWindow(this->parent_window);

	if (!bg_img->GetStylesheet() && this->stylesheet) {
		bg_img->AttachStylesheet(this->stylesheet);
	}


	int distance_y = 0;
	int distance_x = 0;

	distance_y = (frame_bg->GetY() + frame_bg->GetHeight() - texture_rect.h) / frame_bg->GetScrollFactor_y();
	distance_x = (frame_bg->GetX() + frame_bg->GetWidth() - texture_rect.w) / frame_bg->GetScrollFactor_x();

	if (distance_y > scroll_limit_y) scroll_limit_y = distance_y * (distance_y > 0);
	if (distance_x > scroll_limit_x) scroll_limit_x = distance_x * (distance_x > 0);

}

void GFrame::RemoveElement(GElement * element){
	this->frame_elements.remove(element);
	if (this->focussed_element_click == element) {
		this->focussed_element_click = NULL;
	}
	if (this->focussed_element_motion == element) {
		this->focussed_element_motion = NULL;
	}
}

bool GFrame::OnFocusLostMotion(){
	if (!GElement::OnFocusLostMotion()) return false;
	if (focussed_element_motion) {
		focussed_element_motion->OnFocusLostMotion();
	}
	return true;
}

bool GFrame::OnFocusLostClick(){
	if (!GElement::OnFocusLostClick()) return false;
	GElement::OnFocusLostClick();
	if (focussed_element_click)
		focussed_element_click->OnFocusLostClick();
	return true;
}

bool GFrame::OnFocusGainedMotion(){
	return false;
}

bool GFrame::OnFocusGainedClick(){
	return false;
}

bool GFrame::isFocussable_click() {
	return true;
}

GElement* GFrame::GetFocussedElementMotion() {
	return focussed_element_motion;
}
GElement* GFrame::GetFocussedElementClick() {
	return focussed_element_click;
}

void GFrame::SetFocussedElementMotion(GElement* element) {

	if (focussed_element_motion == element) return;

	if (focussed_element_motion && focussed_element_motion != element) focussed_element_motion->OnFocusLostMotion();
	this->focussed_element_motion = element;

	updated = true;
	if (!focussed_element_motion) return;
	focussed_element_motion->OnFocusGainedMotion();
	this->focussed_motion = true;

	element->SetEnabled(true);
	if (element->GetAttributes()->PUSH_TO_TOP_ON_CURSOR_IN && !element->GetAttributes()->FIXED_POSITION) {
		bool inserted = false;
		bool removed = false;
		for (auto it = frame_elements.begin(); it != frame_elements.end(); ++it) {

			if ((*it) == element && it == frame_elements.begin())
				break;
			else if (*it == element) {
				if (!inserted) break;
				it = frame_elements.erase(it);
				removed = true;
			}
			else if ((*it) != element && !(*it)->GetAttributes()->FIXED_POSITION) {
				frame_elements.insert(it, element);
				inserted = true;
			}

			if (inserted && removed)
				break;
		}
	}

	if (parent_element) parent_element->SetFocussedElementMotion(this);

}
void GFrame::SetFocussedElementClick(GElement* element) {

	if (focussed_element_click == element) return;
	if (focussed_element_click) focussed_element_click->OnFocusLostClick();
	this->focussed_element_click = element;

	updated = true;
	if (!focussed_element_click) return;
	focussed_element_click->OnFocusGainedClick();
	this->focussed_click = true;

	if (element->GetAttributes()->PUSH_TO_TOP_ON_CLICKED_IN && !element->GetAttributes()->FIXED_POSITION) {
		bool inserted = false;
		bool removed = false;
		for (auto it = frame_elements.begin(); it != frame_elements.end(); ++it) {

			if ((*it) == element && it == frame_elements.begin())
				break;
			else if (*it == element) {
				if (!inserted) break;
				it = frame_elements.erase(it);
				removed = true;
			}
			else if ((*it) != element && !(*it)->GetAttributes()->FIXED_POSITION) {
				frame_elements.insert(it, element);
				inserted = true;
			}

			if (inserted && removed)
				break;
		}
	}

	if(parent_element && focussed_element_click) parent_element->SetFocussedElementClick(this);

}

GElement* GFrame::GetElementByID(std::string ID) {

	if (GElement::GetElementByID(ID)) return this;
	GElement* res;

	for (GElement* elem : frame_elements) {
		res = elem->GetElementByID(ID);
		if (res) {
			return res;
		}
	}
	return NULL;
}

void GFrame::DrawTexture() {

	if (!render_enabled) return;

	if (CheckUpdated()) {

		SDL_Texture* previous_target = SDL_GetRenderTarget(renderer);
		SDL_SetRenderTarget(renderer, element_texture);
		SDL_RenderSetClipRect(renderer, &texture_rect);

		if (this->frame_bg)
			frame_bg->DrawTexture();

		for (std::list<GElement*>::reverse_iterator it = frame_elements.rbegin(); it != frame_elements.rend(); ++it) {
			(*it)->DrawTexture();
		}

		SDL_RenderSetClipRect(renderer, &texture_rect);
		SDL_SetRenderTarget(renderer, previous_target);

	}
	GElement::DrawTexture();

}

void GFrame::PrepareTexture() {
	if (!CheckUpdated()) return;

	if(frame_bg) frame_bg->PrepareTexture();
	for (GElement* element : frame_elements) {
		element->PrepareTexture();
	}
}

void GFrame::Scroll_Containing (Direction dirn, int amount) {

	bool scroll = false;
	
	switch (dirn) {
	case DIRECTION_X:
		scrolled_amount_x += amount;
		if (scrolled_amount_x >= 0 && scrolled_amount_x <= scroll_limit_x)
			scroll = true;
		break;
	case DIRECTION_Y:
		scrolled_amount_y += amount;
		if (scrolled_amount_y >= 0 && scrolled_amount_y <= scroll_limit_y)
			scroll = true;
		break;
	}

	if (scroll) {
		frame_bg->Scroll(dirn, amount);
		for (std::list<GElement*>::iterator it = frame_elements.begin(); it != frame_elements.end(); ++it) {
			(*it)->Scroll(dirn, amount);
		}
	}

	else {
		switch (dirn) {
		case DIRECTION_X:
			scrolled_amount_x -= amount;
			break;
		case DIRECTION_Y:
			scrolled_amount_y -= amount;
			break;
		}
	}	

}

void GFrame::SetContainingScrollable(bool state_x, bool state_y) {
	this->containing_scrollable_x = state_x; 
	this->containing_scrollable_y = state_y;
}

void GFrame::SetContainingScrollFactors(int factor_x, int factor_y){

	this->containing_scrollfactor_x = factor_x;
	this->containing_scrollfactor_y = factor_y;
}

GFrame::~GFrame(){

	GElement* elem;
	std::list<GElement*>::iterator it;
	it = frame_elements.begin();
	
	while (it != frame_elements.end()) {
		elem = *it;
		delete elem;
		++it;
	}
	if (frame_bg) {
		delete frame_bg;
	}
}


