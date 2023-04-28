
#include "SDL.h"
#include "SDL2_gfxPrimitives.h"
#include "DrawingFunctions.h"

#include "resources.h"


void CreateRoundedBox(SDL_Renderer* ren, SDL_Texture* tex, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 rad,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_BlendMode blendmode) {

	SDL_SetTextureBlendMode(tex, blendmode);
	SDL_Texture* previous_target = SDL_GetRenderTarget(ren);
	SDL_SetRenderTarget(ren, tex);

	roundedBoxRGBA(ren, x1, y1, x2, y2, rad, r, g, b, a);

	SDL_SetRenderTarget(ren, previous_target);

}

void CreateRectangle(SDL_Renderer* ren, SDL_Texture* tex, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_BlendMode blendmode) {

	SDL_SetTextureBlendMode(tex, blendmode);
	SDL_Texture* previous_target = SDL_GetRenderTarget(ren);
	SDL_SetRenderTarget(ren, tex);

	boxRGBA(ren, x1, y1, x2, y2, r, g, b, a);

	SDL_SetRenderTarget(ren, previous_target);

}

void CreateLine(SDL_Renderer* ren, SDL_Texture* tex, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Uint8 width,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_BlendMode blendmode) {

	SDL_SetTextureBlendMode(tex, blendmode);
	SDL_Texture* previous_target = SDL_GetRenderTarget(ren);
	SDL_SetRenderTarget(ren, tex);

	thickLineRGBA(ren, x1, y1, x2, y2, width, r, g, b, a);

	SDL_SetRenderTarget(ren, previous_target);
}

void CreateEllipse(SDL_Renderer* ren, SDL_Texture* tex, Sint16 x, Sint16 y, Sint16 radx, Sint16 rady,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_BlendMode blendmode) {

	SDL_SetTextureBlendMode(tex, blendmode);
	SDL_Texture* previous_target = SDL_GetRenderTarget(ren);
	SDL_SetRenderTarget(ren, tex);

	filledEllipseRGBA(ren, x, y, radx, rady, r, g, b, a);

	SDL_SetRenderTarget(ren, previous_target);
}

void CreateTriangle(SDL_Renderer* ren, SDL_Texture* tex, Sint16 x1, Sint16 y1, Sint16 x2, Sint16 y2, Sint16 x3, Sint16 y3,
	Uint8 r, Uint8 g, Uint8 b, Uint8 a, SDL_BlendMode blendmode) {

	SDL_SetTextureBlendMode(tex, blendmode);
	SDL_Texture* previous_target = SDL_GetRenderTarget(ren);
	SDL_SetRenderTarget(ren, tex);

	filledTrigonRGBA(ren, x1, y1, x2, y2, x3, y3, r, g, b, a);

	SDL_SetRenderTarget(ren, previous_target);

}
