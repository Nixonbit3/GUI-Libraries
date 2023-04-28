#include "resources.h"
#include "GStylesheet.h"

SDL_BlendMode PRESERVE_DST_ALPHA =
SDL_ComposeCustomBlendMode(
	SDL_BLENDFACTOR_SRC_ALPHA,
	SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
	SDL_BLENDOPERATION_ADD,
	SDL_BLENDFACTOR_ZERO,
	SDL_BLENDFACTOR_ONE,
	SDL_BLENDOPERATION_ADD
);

SDL_BlendMode PRESERVE_SRC_ALPHA =
SDL_ComposeCustomBlendMode(
	SDL_BLENDFACTOR_SRC_ALPHA,
	SDL_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
	SDL_BLENDOPERATION_ADD,
	SDL_BLENDFACTOR_ONE,
	SDL_BLENDFACTOR_ZERO,
	SDL_BLENDOPERATION_ADD
);

SDL_Rect rect_maker(int x, int y, int w, int h) {
	return SDL_Rect{ x,y,w,h };
}

SDL_Texture* load_image_from_path(SDL_Renderer* renderer, std::string path) {
	SDL_Surface* loaded_surface = IMG_Load(path.c_str());
	SDL_Texture* loaded_image = SDL_CreateTextureFromSurface(renderer, loaded_surface);
	SDL_FreeSurface(loaded_surface);
	return loaded_image;
}

SDL_Texture* load_texture_from_text(SDL_Renderer* renderer, font_data* fontdata, std::string text) {

	SDL_Surface* text_temp;
	SDL_Texture* text_texture;

	text_temp = TTF_RenderUTF8_Solid(fontdata->face, text.c_str(), *(fontdata->color));

	text_texture = SDL_CreateTextureFromSurface(renderer, text_temp);
	SDL_FreeSurface(text_temp);

	return text_texture;
}

SDL_Texture * load_texture_from_multiline_text(SDL_Renderer * renderer, font_data * fontdata, std::string text, int wrap_x){

	SDL_Surface* text_temp;
	SDL_Texture* text_texture;

	text_temp = TTF_RenderText_Blended_Wrapped(fontdata->face, text.c_str(), *(fontdata->color), wrap_x);

	text_texture = SDL_CreateTextureFromSurface(renderer, text_temp);
	SDL_FreeSurface(text_temp);

	return text_texture;
}

SDL_Rect get_rect_from_texture(SDL_Texture* texture) {
	int w;
	int h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	return rect_maker(0, 0, w, h);
}

void fill_texture_with_color(SDL_Renderer* renderer, SDL_Texture* texture, Uint8 r, Uint8 g, Uint8 b, Uint8 a){

	SDL_Texture* prev_target = SDL_GetRenderTarget(renderer);
	SDL_SetRenderDrawColor(renderer, r, g, b, a);
	SDL_SetRenderTarget(renderer, texture);
	SDL_RenderClear(renderer);
	SDL_SetRenderTarget(renderer, prev_target);

}

void expand_rect(SDL_Rect* rect, int x_, int y_, int width_, int height_) {
	rect->x += x_;
	rect->y += y_;
	rect->w += width_;
	rect->h += height_;
}

void expand_texture(SDL_Renderer* renderer, SDL_Texture** tex, int dx1, int dy1, int dx2, int dy2) {

	std::cout << "expanding texture";
	int width, height;

	SDL_QueryTexture(*tex, NULL, NULL, &width, &height);

	SDL_Texture* temp_tex = *tex;
	*tex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width + dx1 + dx2, height + dy1 + dy2);

	SDL_Texture* prev_target = SDL_GetRenderTarget(renderer);
	SDL_SetRenderTarget(renderer, *tex);

	SDL_RenderCopy(renderer, temp_tex, NULL, &rect_maker(dx1, dy1, width, height));

	SDL_SetRenderTarget(renderer, prev_target);
	SDL_DestroyTexture(temp_tex);
}

enum Event_ID { MOUSEDOWN, MOUSEMOTION, MOUSEMOTION_CLICK_LEFT, MOUSEMOTION_CLICK_RIGHT, KEYPRESS };

GStylesheet DEFAULT_STYLESHEET;

SDL_Color red = { 255, 0, 0, 255 };
SDL_Color green = { 0, 255, 0, 255 };
SDL_Color blue = { 0, 0, 255, 255 };
SDL_Color black = { 0, 0, 0, 255 };
SDL_Color white = { 255, 255, 255, 255 };