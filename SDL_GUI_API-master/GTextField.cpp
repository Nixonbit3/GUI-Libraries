#include "GTextField.h"

GTextField::GTextField(std::string ID, SDL_Renderer* renderer, SDL_Rect rect): GElement(ID, renderer, rect){

	type = "TXT_FIELD";

	this->texture_repository.resize(4);
	this->offset_rect = rect_maker(0, 0, rect.w, rect.h);
	text_width = 0;
	this->typed_text.push_back('\0');

	active_index = IDLE_IMAGE;

	this->texture_padding = 5;
	this->text_rect = { 10, 10, rect.w - 20, rect.h - 20 };
	this->offset_rect = rect_maker(0, 0, text_rect.w, text_rect.h);
}

void GTextField::SetTextRect(SDL_Rect rect) {
	this->text_rect = rect;
	this->offset_rect = rect_maker(0, 0, text_rect.w, text_rect.h);
	SetText();
}

bool GTextField::InitializeTextures_from_stylesheet(){
	
	if(!GElement::InitializeTextures_from_stylesheet()) return false;

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
		texture_repository[CLICKED_IMAGE],
		texture_padding, texture_padding, texture_rect.w - texture_padding, texture_rect.h - texture_padding,
		this->stylesheet->neutral->r,
		this->stylesheet->neutral->g,
		this->stylesheet->neutral->b,
		this->stylesheet->neutral->a,
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
		texture_repository[IDLE_IMAGE],
		texture_padding, texture_padding, texture_rect.w - texture_padding, texture_rect.h - texture_padding,
		this->stylesheet->neutral->r,
		this->stylesheet->neutral->g,
		this->stylesheet->neutral->b,
		this->stylesheet->neutral->a,
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
	CreateRectangle(
		renderer,
		texture_repository[HIGHLIGHT_IMAGE],
		texture_padding, texture_padding, texture_rect.w - texture_padding, texture_rect.h - texture_padding,
		this->stylesheet->neutral->r,
		this->stylesheet->neutral->g,
		this->stylesheet->neutral->b,
		this->stylesheet->neutral->a,
		SDL_BLENDMODE_BLEND
	);
	if (typed_text != "")SetText();
	this->text_height = TTF_FontHeight(stylesheet->font->face);
	return true;
}

void GTextField::PrepareTexture() {

	if (!CheckUpdated()) return;
	SDL_Texture* prev_target = SDL_GetRenderTarget(renderer);

	SDL_SetRenderTarget(renderer, element_texture);
	SDL_RenderCopy(renderer, texture_repository[active_index], NULL, &rect_maker(0, 0, texture_rect.w, texture_rect.h));
	if (this->typed_text != "")SDL_RenderCopy(renderer, texture_repository[TEXT_IMAGE], &offset_rect, &rect_maker(text_rect.x, text_rect.y, text_rect.w > text_width ? text_width : text_rect.w, text_height));

	SDL_SetRenderTarget(renderer, prev_target);

}

void GTextField::AttachStylesheet(GStylesheet* stylesheet){
	GElement::AttachStylesheet(stylesheet);
}

void GTextField::ScrollOffset(int direction) {

	if (offset_rect.x <= 0 && direction == -1 || offset_rect.x + offset_rect.w >= text_width && direction == 1) {
		return;
	}

	if (offset_rect.x >= 1 && direction == -1) {
		offset_rect.x -= 1;
	}
	else if ((offset_rect.x + offset_rect.w) <= (text_width - 1) && direction == 1) {
		offset_rect.x += 1;
	}
}

void GTextField::SetText(std::string text) {

	this->typed_text = text;
	this->SetText();
}

void GTextField::SetText() {

	if (!this->stylesheet) return;

	if (texture_repository[TEXT_IMAGE]) SDL_DestroyTexture(texture_repository[TEXT_IMAGE]);
	texture_repository[TEXT_IMAGE] = load_texture_from_text(renderer, stylesheet->font, typed_text);
	SDL_QueryTexture(texture_repository[TEXT_IMAGE], NULL, NULL, &text_width, NULL);

	if (text_width > text_rect.w) {
		offset_rect.x = text_width - text_rect.w;
	}
	else {
		offset_rect.x = 0;
	}
	if (offset_rect.x + offset_rect.w >= text_width && text_width > text_rect.w) {
		offset_rect.x = text_width - offset_rect.w;
	}
	updated = true;
}

std::string GTextField::GetText() {
	return this->typed_text;
}

void GTextField::SetTextPadding(int padding){
	this->SetTextRect(rect_maker(padding, padding, texture_rect.w - 2 * padding, texture_rect.h - 2 * padding));
	SetText();
}

void GTextField::SetTexturePadding(int padding){
	this->texture_padding = padding;
	this->InitializeTextures_from_stylesheet();
}


bool GTextField::CheckEvent(SDL_Event event) {
	if (!GElement::CheckEvent(event)) return false;
	switch (event.type) {
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
	case SDL_MOUSEBUTTONUP:
		if (CheckCursor()) {
			if (event.type == SDL_MOUSEMOTION) { 
				this->focussed_motion = true; 
			}
			else if (event.type == SDL_MOUSEBUTTONDOWN && isFocussable_click()) this->focussed_click = true;
			return true;
		}
		break;
	case SDL_KEYDOWN:
		if (active) return true;
		break;
	default: 
		return false;
		break;
	}
	return false;
}

void GTextField::OnMouseDown_left() {
	GElement::OnMouseDown_left();
	active = true;
	active_index = CLICKED_IMAGE;
}

void GTextField::OnMouseMotion_Hover() {
	GElement::OnMouseMotion_Hover();
	if (!active) active_index = HIGHLIGHT_IMAGE;
}

void GTextField::OnMouseUp() {
	;
}

bool GTextField::OnFocusLostClick(){
	if(!GElement::OnFocusLostClick()) return false;
	active = false;
	active_index = IDLE_IMAGE;
	return true;
}

bool GTextField::OnFocusLostMotion() {
	if(!GElement:: OnFocusLostMotion()) return false;
	if (!active) {
		active_index = IDLE_IMAGE;
	}
	return true;
}

bool GTextField::OnFocusGainedClick(){
	if (!GElement::OnFocusGainedClick()) {
		return false;
	}
	this->active = true;
	active_index = CLICKED_IMAGE;
	return true;
}

bool GTextField::isFocussable_click(){
	return true;
}

void GTextField::OnKeyPressed() {
	GElement::OnKeyPressed();
	char typed_letter = event.key.keysym.sym;
	switch (typed_letter) {
	case SDLK_RETURN:
	case SDLK_RETURN2:
		if (!Function_key_enter) break;
	case SDLK_ESCAPE:
		this->OnFocusLostClick();
		active_index = IDLE_IMAGE;
		if (GetParentElement()->GetFocussedElementClick() == (GElement*)this) GetParentElement()->SetFocussedElementClick(NULL);
		break;
	case 80:
		ScrollOffset(1);
		break;
	case 79:
		ScrollOffset(-1);
		break;
	case SDLK_BACKSPACE:
		if (typed_text != "") typed_text.pop_back();
		if (typed_text != "") typed_text.pop_back();
		typed_text.push_back('\0');
		SetText();
		break;
	default:
		if (typed_text != "") typed_text.pop_back();
		typed_text.push_back(typed_letter);
		typed_text.push_back('\0');
		SetText();
		break;
	}
}
