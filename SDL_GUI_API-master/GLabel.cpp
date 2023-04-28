#include "GLabel.h"


GLabel::GLabel(std::string ID, SDL_Renderer* renderer, SDL_Rect texture_rect)
	:GElement(ID, renderer, texture_rect) {

	type = "LABEL";
	text = "";

	this->text_rect = SDL_Rect{ 0,0,texture_rect.w, texture_rect.h };
	this->active_index = 0;
	texture_repository.resize(2);

}

bool GLabel::CheckEvent() {
	return false;
}

void GLabel::PrepareTexture() {

	if (!updated) return;

	SDL_Texture* prev_target = SDL_GetRenderTarget(renderer);

	SDL_SetRenderTarget(renderer, element_texture);
	SDL_RenderCopy(renderer, texture_repository[BG_IMAGE], NULL, &rect_maker(0, 0, texture_rect.w, texture_rect.h));
	if(this->text!="") SDL_RenderCopy(renderer, texture_repository[TEXT_IMAGE], NULL, &text_rect);

	SDL_SetRenderTarget(renderer, prev_target);
}

void GLabel::SetTextRect(SDL_Rect rect){
	this->text_rect = rect;
}

bool GLabel::InitializeTextures_from_stylesheet(){

	if (!GElement::InitializeTextures_from_stylesheet())
		return false;

	CreateRoundedBox(
		renderer,
		texture_repository[BG_IMAGE],
		0, 0, texture_rect.w, texture_rect.h,20,
		this->stylesheet->background->r,
		this->stylesheet->background->g,
		this->stylesheet->background->b,
		this->stylesheet->background->a,
		SDL_BLENDMODE_BLEND
	);


	if (this->text != "") {

		if (texture_repository[TEXT_IMAGE])SDL_DestroyTexture(this->texture_repository[TEXT_IMAGE]);
		texture_repository[TEXT_IMAGE] = load_texture_from_text(renderer, stylesheet->font, this->text);
		int w, h;
		SDL_QueryTexture(texture_repository[TEXT_IMAGE], NULL, NULL, &w, &h);
		this->text_rect = rect_maker((texture_rect.w - w) / 2, (texture_rect.h - h) / 2, w, h);

	}

	updated = true;
	return true;
}

void GLabel::SetText(std::string text) {
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

std::string GLabel::GetText() {
	return this->text;
}