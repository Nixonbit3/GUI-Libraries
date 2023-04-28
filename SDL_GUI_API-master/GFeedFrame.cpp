#include "GFeedFrame.h"
#include "GButton.h"
#include "GWindow.h"
#include "GBackgroundImage.h"

GFeedFrame::GFeedFrame(std::string ID, SDL_Renderer * renderer, SDL_Rect texture_rect, Direction dirn): GFrame(ID, renderer, texture_rect){

	this->type = "FD_FRAME";

	this->default_spacing_x = 10;
	this->default_spacing_y = 10;
	expanding_feed = false;

	this->dirn = dirn;
}

void GFeedFrame::SetSpacing(int space_x, int space_y, bool propagate){

	if (frame_elements.size()) {
		return;
	}

	this->default_spacing_x = space_x;
	this->default_spacing_y = space_y;
	return;
	   
}

void GFeedFrame::SetExpanding(bool state){
	if (frame_elements.size()) {
		return;
	}

	this->expanding_feed = true;
}

bool GFeedFrame::GetExpanding(){
	return expanding_feed;
}

void GFeedFrame::NotifyChildDimensionsChanged(){

	scroll_limit_x = 0;
	scroll_limit_y = 0;

	if (expanding_feed) {

		int w, h;
		SDL_QueryTexture(element_texture, NULL, NULL, &w, &h);

		SDL_DestroyTexture(element_texture);
		dirn == DIRECTION_X ? this->texture_rect.w = 1 : this->texture_rect.h = 1;

		element_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dirn == DIRECTION_X ? 1 : w, dirn == DIRECTION_Y ? 1 : h);
	}
	this->InitializeTextures_from_stylesheet();

	std::vector<GElement*> elements_;
	elements_.resize(frame_elements.size() + 1);

	auto it = frame_elements.begin();

	while (it != frame_elements.end()) {
		elements_.push_back(*it);
		it = frame_elements.erase(it);
	}

	for (int i = 0; i < elements_.size(); i++) {
		this->AddElement(elements_[i], PUSH_BOTTOM);
	}

	this->InitializeTextures_from_stylesheet();
	this->Scroll_Containing(dirn, dirn == DIRECTION_X ? scrolled_amount_x : scrolled_amount_y);

	if (!expanding_feed) {
		GFrame::NotifyChildDimensionsChanged();
	}

	if (parent_element->GetType() == "FD_FRAME" && expanding_feed) {
		parent_element->NotifyChildDimensionsChanged();
	}
}


void GFeedFrame::AddElement(GElement* element, int position){

	if (!element) return;

	if (position < -2) 
		return;
	
	std::list<GElement*>::iterator it = frame_elements.begin();
	SDL_Rect tex_rect;
	GElement* _elem = nullptr;
	int height = element->GetHeight();
	int width = element->GetWidth();

	if (!frame_elements.size()) {
		element->SetTextureRect(rect_maker(default_spacing_x, default_spacing_y, element->GetWidth(), element->GetHeight()));
		frame_elements.push_back(element);
	}
	else {

		switch (position) {
		case PUSH_TOP:
			_elem = *(frame_elements.begin());
			tex_rect.x = _elem->GetX();
			tex_rect.y = _elem->GetY();
			tex_rect.h = height;
			tex_rect.w = width;
			element->SetTextureRect(tex_rect);

			for (GElement* elem : frame_elements) {
				elem->Move(dirn == DIRECTION_X ? width + default_spacing_x : 0, dirn == DIRECTION_Y ? height + default_spacing_y : 0);
			}
			frame_elements.push_front(element);
			break;

		case PUSH_BOTTOM:
			_elem = *(frame_elements.rbegin());
			tex_rect.x = _elem->GetX() + (dirn == DIRECTION_X ? _elem->GetWidth() + default_spacing_x : 0);
			tex_rect.y = _elem->GetY() + (dirn == DIRECTION_Y ? _elem->GetHeight() + default_spacing_y : 0);
			tex_rect.h = height;
			tex_rect.w = width;
			element->SetTextureRect(tex_rect);

			frame_elements.push_back(element);
			break;

		default:
			std::advance(it, position - 1);
			_elem = *it;

			tex_rect.x = _elem->GetX() + (dirn == DIRECTION_X ? _elem->GetWidth() + default_spacing_x : 0);
			tex_rect.y = _elem->GetY() + (dirn == DIRECTION_Y ? _elem->GetHeight() + default_spacing_y : 0);
			tex_rect.h = height;
			tex_rect.w = width;
			element->SetTextureRect(tex_rect);

			std::advance(it, 1);
			it = frame_elements.insert(it, element);
			std::advance(it, 1);

			for (auto _it = it; _it != frame_elements.end(); ++_it) {
				(*_it)->Move(dirn == DIRECTION_X ? width + default_spacing_x : 0, dirn == DIRECTION_Y ? height + default_spacing_y : 0);
			}
			break;
		}
	}

	if (!element->GetStylesheet() && this->stylesheet) {
		element->AttachStylesheet(this->stylesheet);
	}
	element->SetParentElement(this);
	element->SetParentWindow(this->parent_window);


	float distance_y = 0;
	float distance_x = 0;

	if (this->expanding_feed == true) {

		texture_rect.w += dirn == DIRECTION_X ? width + default_spacing_x : 0;
		texture_rect.h += dirn == DIRECTION_Y ? height + default_spacing_y : 0;

		events_rect.w += dirn == DIRECTION_X ? width + default_spacing_x : 0;
		events_rect.h += dirn == DIRECTION_Y ? height + default_spacing_y : 0;

		if (frame_free_render) {
			absolute_position_rect.w += dirn == DIRECTION_X ? width + default_spacing_x : 0;
			absolute_position_rect.h += dirn == DIRECTION_Y ? height + default_spacing_y : 0;
		}

		int w, h;
		SDL_QueryTexture(element_texture, NULL, NULL, &w, &h);
		SDL_DestroyTexture(element_texture);
		element_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dirn == DIRECTION_X ? w + width + default_spacing_x : w, dirn == DIRECTION_Y ? h + height + default_spacing_y : h);

	}

	distance_y = (((*frame_elements.rbegin())->GetY() + height - texture_rect.h) / scrollfactor_y);
	distance_x = (((*frame_elements.rbegin())->GetX() + width - texture_rect.w) / scrollfactor_x);

	if (distance_y > scroll_limit_y) scroll_limit_y = distance_y * (distance_y > 0);
	if (distance_x > scroll_limit_x) scroll_limit_x = distance_x * (distance_x > 0);

	this->InitializeTextures_from_stylesheet();
	return;
}

void GFeedFrame::RemoveElement(GElement * element){
	std::list<GElement*>::iterator it_;
	for (auto it = frame_elements.begin(); it != frame_elements.end(); ++it) {
		if (*it == element) {
			it_ = it;
			break;
		}
	}
	RemoveElement(it_);
}

void GFeedFrame::RemoveElement(int position) {
	std::list<GElement*>::iterator it_ = frame_elements.begin();
	std::advance(it_, position);
	RemoveElement(it_);
}

void GFeedFrame::RemoveElement(std::list<GElement*>::iterator it){

	GElement* _elem = nullptr;
	_elem = *it;

	it = frame_elements.erase(it);
	SDL_Rect elem_texture_rect = _elem->GetTextureRect();
	int width = -(elem_texture_rect.w + default_spacing_x);
	int height = -(elem_texture_rect.h + default_spacing_y);

	for (; it != frame_elements.end(); ++it) {
		(*it)->Move(dirn == DIRECTION_X ? width : 0, dirn == DIRECTION_Y ? height : 0);
	}
	
	if (this->expanding_feed) {

		texture_rect.w += dirn == DIRECTION_X ? width : 0;
		texture_rect.h += dirn == DIRECTION_Y ? height : 0;

		events_rect.w += dirn == DIRECTION_X ? width : 0;
		events_rect.h += dirn == DIRECTION_Y ? height : 0;

		if (frame_free_render) {
			absolute_position_rect.w += dirn == DIRECTION_X ? width : 0;
			absolute_position_rect.h += dirn == DIRECTION_Y ? height : 0;
		}

		int w, h;
		SDL_QueryTexture(element_texture, NULL, NULL, &w, &h);
		SDL_DestroyTexture(element_texture);
		element_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, dirn == DIRECTION_X ? w + width : w, dirn == DIRECTION_Y ? h + height : h);
		InitializeTextures_from_stylesheet();
	}
}


