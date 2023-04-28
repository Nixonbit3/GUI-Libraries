#include "GMultiLineLabel.h"

GMultiLineLabel::GMultiLineLabel(std::string ID, SDL_Renderer * renderer, SDL_Rect rect): GLabel(ID, renderer, rect){
	wrap_length = text_rect.w;
}

void GMultiLineLabel::SetText(std::string text){

	this->text = text;

	if (!this->stylesheet) {
		return;
	}

	this->text = text;


	if (this->texture_repository[TEXT_IMAGE]) {
		SDL_DestroyTexture(this->texture_repository[TEXT_IMAGE]);
		texture_repository[TEXT_IMAGE] = NULL;
	}

	this->texture_repository[TEXT_IMAGE] = load_texture_from_multiline_text(renderer, this->stylesheet->font, text, wrap_length);
	int w, h;
	SDL_QueryTexture(texture_repository[TEXT_IMAGE], NULL, NULL, &w, &h);
	this->text_rect = rect_maker(((texture_rect.w - w) / 2)*(texture_rect.w > w), ((texture_rect.h - h) / 2)*(texture_rect.h > h), w, h);
	updated = true;

}

void GMultiLineLabel::SetWrapLength(int wrap){
	this->wrap_length = wrap;
}
