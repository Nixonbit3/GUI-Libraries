#include "GDrawingArea.h"

void GDrawingArea::OnMouseDown_left(){

	GElement::OnMouseDown_left();

	if (!drawing) drawing = true;
}

void GDrawingArea::OnMouseMotion_Clicked_left() {

	GElement::OnMouseMotion_Clicked_left();

	SDL_Texture* prev_target = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, texture_repository[ACTIVE_DRAWING]);

	SDL_RenderCopy(renderer, pen_brush, NULL, &rect_maker(event.motion.x, event.motion.y, 10, 10));

	SDL_SetRenderTarget(renderer, prev_target);
}

void GDrawingArea::OnMouseUp() {
	GElement::OnMouseUp();
	this->active_index = false;
}

void GDrawingArea::OnKeyPressed(){

	GElement::OnKeyPressed();
	switch (event.key.keysym.sym) {
	case SDLK_LEFT:
		this->offset_rect.x -= 5;
		break;
	case SDLK_2:
		std::cout << "changed color";
		this->SetPenColor(red);
		break;
	case SDLK_3:
		this->SetPenColor(green);
		break;
	default:
		break;
	}
}

GDrawingArea::GDrawingArea(std::string ID, SDL_Renderer * renderer, SDL_Rect rect): GElement(ID, renderer, rect){

	texture_repository.resize(1);
	this->pen_color = black;
	this->background_color = &white;
	drawing = false;

	this->pen_shape.ellipse.shape_type = SHAPE_ELLIPSE;
	this->pen_shape.ellipse.radx = 5;
	this->pen_shape.ellipse.rady = 5;
	this->pen_brush = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 10, 10);
	SDL_SetTextureBlendMode(pen_brush, SDL_BLENDMODE_BLEND);

	CreateEllipse(
		renderer,
		pen_brush,
		5,
		5,
		pen_shape.ellipse.radx,
		pen_shape.ellipse.rady,
		pen_color.r,
		pen_color.g,
		pen_color.b,
		pen_color.a,
		SDL_BLENDMODE_BLEND
	);

	this->offset_rect = SDL_Rect{ 0,0,texture_rect.w, texture_rect.h };

	this->texture_repository[ACTIVE_DRAWING] = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, this->texture_rect.w, this->texture_rect.h);

}

bool GDrawingArea::CheckEvent(SDL_Event event){

	if (!GElement::CheckEvent(event)) {
		return false;
	}

	switch (event.type) {
	case SDL_MOUSEMOTION:
	case SDL_MOUSEBUTTONDOWN:
		if (CheckCursor()) {
			return true;
		}
		break;
	case SDL_KEYDOWN:
		return true;
	default:
		return false;
		break;
	}
	return false;
}

void GDrawingArea::PrepareTexture(){
	if (!CheckUpdated()) return;
	SDL_Texture* prev_target = SDL_GetRenderTarget(renderer);

	SDL_SetRenderTarget(renderer, element_texture);
	SDL_RenderCopy(renderer, texture_repository[ACTIVE_DRAWING], &offset_rect, NULL);

	SDL_SetRenderTarget(renderer, prev_target);

}

void GDrawingArea::SetPenColor(SDL_Color color){
	
	this->pen_color = color;
	fill_texture_with_color(renderer, pen_brush, 0, 0, 0, 0);
	CreateEllipse(
		renderer,
		pen_brush,
		5,
		5,
		pen_shape.ellipse.radx,
		pen_shape.ellipse.rady,
		pen_color.r,
		pen_color.g,
		pen_color.b,
		pen_color.a,
		SDL_BLENDMODE_BLEND
	);

}

void GDrawingArea::SetPenShape(Shape shape){
	this->pen_shape = shape;
}

bool GDrawingArea::isFocussable_click(){
	return true;
}

