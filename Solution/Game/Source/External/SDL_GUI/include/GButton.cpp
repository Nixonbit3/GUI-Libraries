#include "GButton.h"
#include "Functor.h"

GButton::GButton(std::string ID, SDL_Renderer* renderer, SDL_Rect rect) : GElement(ID, renderer, rect) {
	type = "BUTTON";
	text = "";

	this->texture_repository.resize(4);

	active_index = IDLE_IMAGE;
}

bool GButton::InitializeTextures_from_stylesheet() {

	if (!GElement::InitializeTextures_from_stylesheet()) return false;

	CreateRectangle(
		renderer,
		texture_repository[CLICKED_IMAGE],
		0, 0, texture_rect.w, texture_rect.h,
		this->stylesheet->foreground->r,
		this->stylesheet->foreground->g,
		this->stylesheet->foreground->b,
		this->stylesheet->foreground->a,
		SDL_BLENDMODE_BLEND
	);

	CreateRectangle(
		renderer,
		texture_repository[IDLE_IMAGE],
		0, 0, texture_rect.w, texture_rect.h,
		this->stylesheet->background->r,
		this->stylesheet->background->g,
		this->stylesheet->background->b,
		this->stylesheet->background->a,
		SDL_BLENDMODE_BLEND
	);

	CreateRectangle(
		renderer,
		texture_repository[HIGHLIGHT_IMAGE],
		0, 0, texture_rect.w, texture_rect.h,
		this->stylesheet->accent->r,
		this->stylesheet->accent->g,
		this->stylesheet->accent->b,
		this->stylesheet->accent->a,
		SDL_BLENDMODE_BLEND
	);

	if (text != "") this->SetText(text);

	updated = true;

	return true;

}

void GButton::OnMouseDown_left() {
	if (last_event.type != SDL_MOUSEBUTTONDOWN) GElement::OnMouseDown_left();
	SDL_FlushEvent(SDL_MOUSEBUTTONDOWN);
	active_index = CLICKED_IMAGE;
	return;
}

void GButton::OnMouseUp() {
	GElement::OnMouseUp();
	SDL_FlushEvent(SDL_MOUSEBUTTONUP);
	active_index = HIGHLIGHT_IMAGE;
	return;
}

void GButton::OnMouseMotion_Hover() {
	GElement::OnMouseMotion_Hover();
	active_index = HIGHLIGHT_IMAGE;
	return;
}

bool GButton::OnFocusLostMotion() {
	if (!GElement::OnFocusLostMotion()) return false;
	active_index = IDLE_IMAGE;
	return true;
}

bool GButton::CheckEvent(SDL_Event event) {

	if (!GElement::CheckEvent(event)) return false;
	switch (event.type) {

		case SDL_MOUSEMOTION:
		case SDL_MOUSEBUTTONDOWN:
		case SDL_MOUSEBUTTONUP:
			if (CheckCursor()) {
				if (event.type == SDL_MOUSEMOTION) 
					this->focussed_motion = true;
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

void GButton::PrepareTexture() {
	
	if (!updated) return;
	SDL_Texture* prev_target = SDL_GetRenderTarget(renderer);

	SDL_SetRenderTarget(renderer, element_texture);
	SDL_RenderCopy(renderer, texture_repository[active_index], NULL, &rect_maker(0, 0, texture_rect.w, texture_rect.h));
	if (this->text != "")SDL_RenderCopy(renderer, texture_repository[TEXT_IMAGE], NULL, &text_rect);


	SDL_SetRenderTarget(renderer, prev_target);
}

void GButton::SetText(std::string text) {

	this->text = text;

	if (!this->stylesheet) {
		return;
	}

	this->text = text;


	if (this->texture_repository[TEXT_IMAGE]) {
		SDL_DestroyTexture(this->texture_repository[TEXT_IMAGE]);
		texture_repository[TEXT_IMAGE] = NULL;
	}

	this->texture_repository[TEXT_IMAGE] = load_texture_from_text(renderer, this->stylesheet->font, text);
	int w, h;
	SDL_QueryTexture(texture_repository[TEXT_IMAGE], NULL, NULL, &w, &h);
	this->text_rect = rect_maker((texture_rect.w - w) / 2, (texture_rect.h - h) / 2, w, h);
	updated = true;
}

std::string GButton::GetText() {
	return this->text;
}

void GButton::operator()() {
	(*Function_click_left)();
	return;
}