#include "GElement.h"
#include "GFrame.h"
#include "ELEMENT_ATTRIBUTES.h"
#include "PRESET_ELEMENT_ATTRIBUTES.h"

GElement::GElement(std::string ID, SDL_Renderer* renderer, SDL_Rect texture_rect ) {
	this->ID = ID;

	scrollable_x = true;
	scrollable_y = true;
	movable_x = true;
	movable_y = true;
	events_enabled = true;
	render_enabled = true;
	updated = true;
	stylesheet_locked = false;

	frame_free_render = false;
	altered_stylesheet = false;
	altered_attributes = false;

	focussed_click = false;
	focussed_motion = false;

	this->parent_element = NULL;
	this->parent_window = NULL;
	this->stylesheet = NULL;

	scrollfactor_x = 1;
	scrollfactor_y = 1;

	this->Function_click_left = NULL;
	this->Function_click_right = NULL;
	this->Function_key_enter = NULL;
	this->Function_motion = NULL;

	this->texture_rect = texture_rect;
	this->events_rect = rect_maker(0, 0, texture_rect.w, texture_rect.h);
	this->absolute_position_rect = rect_maker(0, 0, 0, 0);

	this->renderer = renderer;

	this->element_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, texture_rect.w, texture_rect.h);
	SDL_SetTextureBlendMode(element_texture, SDL_BLENDMODE_BLEND);
	this->element_attributes = &static_element_attributes;
}

bool GElement::CheckEvent(SDL_Event event) {
	if (!events_enabled) return false;

	if (event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP) {

		event.button.x -= texture_rect.x - events_rect.x;
		event.button.y -= texture_rect.y - events_rect.y;
	}
	else if (event.type == SDL_MOUSEMOTION) {

		event.motion.x -= texture_rect.x - events_rect.x;
		event.motion.y -= texture_rect.y - events_rect.y;
	}

	this->last_event = this->event;
	this->event = event;
	return true;
}

void GElement::HandleEvent() {

	this->updated = true;
	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
		if (event.button.button == SDL_BUTTON_LEFT) {
			this->OnFocusGainedClick();
			this->OnMouseDown_left();
		}
		else if (event.button.button == SDL_BUTTON_RIGHT) {
			this->OnMouseDown_right();
		}
		break;

	case SDL_MOUSEBUTTONUP:
		this->OnMouseUp();
		break;

	case SDL_MOUSEMOTION:
		this->OnFocusGainedMotion();
		if (event.motion.state & SDL_BUTTON_LMASK)
			this->OnMouseMotion_Clicked_left();
		else if (event.motion.state & SDL_BUTTON_RMASK)
			this->OnMouseMotion_Clicked_right();
		else
			this->OnMouseMotion_Hover();

		break;

	case SDL_KEYDOWN:
		this->OnKeyPressed();
		break;

	case SDL_KEYUP:
		this->OnKeyReleased();
		break;

	case SDL_MOUSEWHEEL:
		this->OnMouseWheel();
		break;
	}

}

void GElement::SetParentWindow(GWindow * window){
	this->parent_window = window;
}

GWindow * GElement::GetParentWindow()
{
	return this->parent_window;
}

bool GElement::CheckCursor() {

	switch (event.type) {
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		if ((events_rect.x < event.button.x && event.button.x < events_rect.w + events_rect.x) &&
			(events_rect.y < event.button.y && event.button.y < events_rect.h + events_rect.y)) {
			return true;
		}
		break;
	case SDL_MOUSEMOTION:
		if ((events_rect.x < event.motion.x && event.motion.x < events_rect.x + events_rect.w) &&
			(events_rect.y < event.motion.y && event.motion.y < events_rect.y + events_rect.h)) {
			return true;
		}
		break;
	}

	return false;
}


void GElement::SetTextureRect(SDL_Rect rect) {
	this->texture_rect = rect;
}

SDL_Rect GElement::GetTextureRect() {
	return this->texture_rect;
}

SDL_Rect * GElement::AlterTextureRect(){
	return &this->texture_rect;
}

void GElement::ExpandElementTexture(int amt_x1, int amt_y1, int amt_x2, int amt_y2){
	int w, h;
	SDL_QueryTexture(this->element_texture, NULL, NULL, &w, &h);

	SDL_Texture* temp_tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, w + amt_x1 + amt_x2, h + amt_y1 + amt_y2);
	SDL_Texture* prev_target = SDL_GetRenderTarget(renderer);

	SDL_SetRenderTarget(renderer, temp_tex);
	SDL_RenderCopy(renderer, element_texture, NULL, &rect_maker(amt_x1, amt_y1, w, h));
	SDL_DestroyTexture(element_texture);

	SDL_SetRenderTarget(renderer, prev_target);

	element_texture = temp_tex;
}

void GElement::SetEventsRect(SDL_Rect rect){
	this->events_rect = rect;
}

SDL_Rect GElement::GetEventsRect(){
	return events_rect;
}

//I realise this breaks encapsulation but this is easily the most efficient solution
SDL_Rect* GElement::AlterEventsRect(){
	return &(this->events_rect);
}

void GElement::SetAbsoluteRect(SDL_Rect rect){
	this->absolute_position_rect = rect;
}

void GElement::SetRenderType(int render_type){
	switch (render_type) {
	case FRAME_FREE:
		this->frame_free_render = true;
	case FRAME_BOUND:
		this->frame_free_render = false;
	}
}

bool GElement::isFrameFree(){

	return this->frame_free_render;
}

void GElement::SetEnabled(bool both_state) {

	bool changed_check_render = both_state != render_enabled;
	bool changed_check_events = both_state != events_enabled;

	this->SetEventsEnabled(both_state);
	this->render_enabled = both_state;

	if(changed_check_events) this->OnEventsEnabledChange(both_state);
	if(changed_check_render) this->OnRenderEnabledChange(both_state);
}

void GElement::SetEventsEnabled(bool events_state) {
	events_enabled = events_state;
	this->OnEventsEnabledChange(events_state);
}

void GElement::OnMouseDown_left() {
	std::cout << ID << "mouse down left" << std::endl;
	if(this->Function_click_left) (*Function_click_left)();
}
void GElement::OnMouseDown_right() {
	if(this->Function_click_right) (*Function_click_right)();
}
void GElement::OnMouseUp() {
	;
}
void GElement::OnKeyPressed() {
	if (this->Function_key_enter && (event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_RETURN)) {
		(*Function_key_enter)();

	}
}
void GElement::OnKeyReleased() {
	;
}
void GElement::OnMouseMotion_Hover() {
	if(this->Function_motion) (*Function_motion)();
}
void GElement::OnMouseMotion_Clicked_left() {
	;
}
void GElement::OnMouseMotion_Clicked_right() {
	;
}
void GElement::OnMouseWheel() {
	;
}

bool GElement::isFocussable_click() {
	return false;
}

bool GElement::OnFocusGainedMotion(){
	if (focussed_motion) return false;
	this->focussed_motion = true;
	if (parent_element && parent_element->GetFocussedElementClick() != this) {
		parent_element->SetFocussedElementMotion(this);
	}
	updated = true;
	return true;
}

bool GElement::OnFocusGainedClick(){
	if (this->focussed_click) return false;
	if (!this->isFocussable_click()) return false;

	this->focussed_click = true;
	if (parent_element && parent_element->GetFocussedElementClick() != this) {
		std::cout << "setting fec"<< std::endl;
		parent_element->SetFocussedElementClick(this);
	}
	updated = true;
	return true;
}

bool GElement::OnFocusLostMotion() {
	if (!focussed_motion) return false;
	focussed_motion = false;
	if (parent_element && parent_element->GetFocussedElementMotion() == this) {
		parent_element->SetFocussedElementMotion(NULL);
	}
	updated = true;
	return true;
}

bool GElement::OnFocusLostClick() {
	if (!focussed_click) return false;
	focussed_click = false;

	if (parent_element && parent_element->GetFocussedElementClick() == this) {
		parent_element->SetFocussedElementClick(NULL);
	}
	updated = true;
	return true;
}

void GElement::DrawTexture() {
	if (!render_enabled) return;

	SDL_Texture* prev_target = NULL;
	if (frame_free_render) {
		prev_target = SDL_GetRenderTarget(renderer);
		SDL_SetRenderTarget(renderer, NULL);
	}

	SDL_RenderSetClipRect(renderer, &texture_rect);
	SDL_RenderCopy(renderer, element_texture, NULL, !frame_free_render ? &texture_rect : &absolute_position_rect);
	updated = false;

	if (frame_free_render) {
		SDL_SetRenderTarget(renderer, prev_target);
	}
}

void GElement::PrepareTexture() {
	return;
}

bool GElement::CheckUpdated() {
	return updated;
}

void GElement::Activate(){
	if(parent_element) parent_element->ActivateElement(this);
	else this->SetEnabled(true);
}

void GElement::Deactivate(){
	if(parent_element) parent_element->DeactivateElement(this);
	else {
		if(this->GetAttributes()->HIDE_ON_DEACTIVATE) this->SetEnabled(false);
		else this->SetEventsEnabled(false);
	}
}

void GElement::SetBlending(SDL_BlendMode blendmode) {
	SDL_SetTextureBlendMode(element_texture, blendmode);
}

void GElement::SetImage(int index, std::string path){
	if (index > texture_repository.size() - 1) {
		return;
	}
	if (texture_repository[index]) {
		SDL_DestroyTexture(texture_repository[index]);
		texture_repository[index] = NULL;
	}
	texture_repository[index] = load_image_from_path(renderer, path);
	this->LockStylesheet();
	updated = true;
}

void GElement::SetImage(int index, SDL_Texture * texture){
	if (index > texture_repository.size() - 1) {
		return;
	}
	if (texture_repository[index]) {
		SDL_DestroyTexture(texture_repository[index]);
		texture_repository[index] = NULL;
	}
	texture_repository[index] = texture;
	this->LockStylesheet();
	updated = true;
}

void GElement::SetImage(int index, SDL_Texture* (*texture_creator)(SDL_Renderer*, GElement* ) ){
	if (index > texture_repository.size() - 1) {
		return;
	}
	if (texture_repository[index]) {
		SDL_DestroyTexture(texture_repository[index]);
		texture_repository[index] = NULL;
	}
	texture_repository[index] = texture_creator(renderer, this);
	this->LockStylesheet();
	updated = true;
}

bool GElement::InitializeTextures_from_stylesheet(){
	if (!stylesheet) return false;

	for (auto it = texture_repository.begin(); it != texture_repository.end(); ++it) {
		if (*it) SDL_DestroyTexture(*it);
		*it = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, texture_rect.w, texture_rect.h);
		SDL_SetTextureBlendMode(*it, SDL_BLENDMODE_BLEND);
		fill_texture_with_color(renderer, *it, 0, 0, 0, 0);
	}
	return true;
}

GElement* GElement::GetElementByID(std::string ID) {
	if (this->GetID() == ID) return this;
	return NULL;
}


int GElement::GetWidth() {
	return this->texture_rect.w;
}

int GElement::GetHeight() {
	return this->texture_rect.h;
}

int GElement::GetX() {
	return this->texture_rect.x;
}

int GElement::GetY() {
	return this->texture_rect.y;
}

std::string GElement::GetID() {
	return this->ID;
}

std::string GElement::GetType(){
	return this->type;
}

std::string GElement::GetInfo() {
	return "Base element class";
}

SDL_Renderer* GElement::GetRenderer() {
	return this->renderer;
}

const ELEMENT_ATTRIBUTES* GElement::GetAttributes() {
	return this->element_attributes;
}

ELEMENT_ATTRIBUTES* GElement::AlterAttributes() {

	if (this->altered_attributes) delete element_attributes;
	this->element_attributes = new ELEMENT_ATTRIBUTES(*(this->element_attributes));
	this->altered_attributes = true;
	return this->element_attributes;
}

void GElement::SetAttributes(ELEMENT_ATTRIBUTES* attributes) {
	this->element_attributes = attributes;
}

GFrame* GElement::GetParentElement() {
	return this->parent_element;
}

void GElement::AttachStylesheet(GStylesheet* stylesheet){

	if (!stylesheet) return;
	if (stylesheet_locked) return;

	this->stylesheet = stylesheet;
	this->InitializeTextures_from_stylesheet();
}

const GStylesheet * GElement::GetStylesheet(){
	return this->stylesheet;
}

GStylesheet * GElement::AlterStylesheet() {

	if (stylesheet_locked) return false;

	if (altered_stylesheet) delete this->stylesheet;
	this->stylesheet = new GStylesheet(*(this->stylesheet));
	altered_stylesheet = true;
	return this->stylesheet;
}

void GElement::LockStylesheet(){
	stylesheet_locked = true;
}

void GElement::UnlockStylesheet(){
	stylesheet_locked = false;
}

void GElement::SetParentElement(GFrame* parent) {
	this->parent_element = parent;
}

void GElement::Scroll(Direction dirn, int amount) {
	if (!scrollable_x && !scrollable_y) return;
	this->updated = true;
	switch (dirn) {
	case DIRECTION_X:
		if (scrollable_x) {
			this->texture_rect.x -= amount * scrollfactor_x;
			if (frame_free_render) this->absolute_position_rect.x += amount * scrollfactor_x;
		}
		break;
	case DIRECTION_Y:
		if (scrollable_y) {
			this->texture_rect.y -= amount * scrollfactor_y;
			if (frame_free_render) this->absolute_position_rect.y += amount * scrollfactor_y;
		}
		break;
	}
}

void GElement::Move(int x_, int y_) {
	if (!movable_x && !movable_y) return;
	this->updated = true;
	if (movable_x) {
		this->texture_rect.x += x_;
		if(frame_free_render) this->absolute_position_rect.x += x_;
	}
	if (movable_y) {
		this->texture_rect.y += y_;
		if (frame_free_render) this->absolute_position_rect.y += y_;
	}
}

void GElement::SetScrollable(bool state_x, bool state_y) {
	this->scrollable_x = state_x;
	this->scrollable_y = state_y;
}

void GElement::SetMovable(bool state_x, bool state_y) {
	this->movable_x = state_x;
	this->movable_y = state_y;
}

void GElement::isScrollable(bool *s_x, bool *s_y) {
	if (s_x) *s_x = scrollable_x;
	if (s_y) *s_y = scrollable_y;
}

void GElement::isMovable(bool *m_x, bool *m_y) {
	if (m_x) *m_x = movable_x;
	if (m_y) *m_y = movable_y;
}

void GElement::SetFunction_click_left(Functor* func) {
	this->Function_click_left = func;
}

void GElement::SetFunction_motion(Functor* func) {
	this->Function_motion = func;
}

void GElement::SetFunction_click_right(Functor* func) {
	this->Function_click_right = func;
}

void GElement::SetFunction_key_enter(Functor* func) {
	this->Function_key_enter = func;
}

void GElement::SetScrollFactor_x(int factor_x) {
	this->scrollfactor_x = factor_x;
}

void GElement::SetScrollFactor_y(int factor_y) {
	this->scrollfactor_y = factor_y;
}

int GElement::GetScrollFactor_x() {
	return scrollfactor_x;
}

int GElement::GetScrollFactor_y() {
	return scrollfactor_y;
}

GElement::~GElement() {

	SDL_Texture* tex;
	for (auto it = texture_repository.begin(); it != texture_repository.end(); ++it) {
		tex = *it;
		if (tex) SDL_DestroyTexture(tex);
	}
	if (this->altered_stylesheet) {
		delete this->stylesheet;
	}
	if (this->altered_attributes) {
		delete this->element_attributes;
	}
}


