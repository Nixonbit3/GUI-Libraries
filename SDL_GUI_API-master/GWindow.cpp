#include "GWindow.h"
#include "containers.h"

GWindow::GWindow(SDL_Renderer* renderer, SDL_Window* window)   {
	SDL_Init(SDL_INIT_EVERYTHING);
	this->active = false;
	this->renderer = renderer;
	this->window = window;
	this->stylesheet = &DEFAULT_STYLESHEET;
	this->render_timeout = 2000;
}

GWindow::GWindow( std::string title, bool centered_x, bool centered_y, int w, int h, Uint32 flags) {

	this->active = false;
	this->active_layer = NULL;
	int x = centered_x ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED;
	int y = centered_y ? SDL_WINDOWPOS_CENTERED : SDL_WINDOWPOS_UNDEFINED;
	SDL_MouseWheelDirection(SDL_MOUSEWHEEL_NORMAL);
	this->window = SDL_CreateWindow(title.c_str(), x, y, w, h, NULL);
	this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE);
	SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	this->stylesheet = NULL;
	this->render_timeout = 50;

}

void GWindow::AddLayer(GFrame* layer) {

	this->layers.push_back(layer);
	if (!layer->GetStylesheet() && this->stylesheet) {
		layer->AttachStylesheet(this->stylesheet);
	}
	layer->SetParentWindow(this);
}

void GWindow::AttachStylesheet(GStylesheet* stylesheet) {

	if (!stylesheet) return;
	this->stylesheet = stylesheet;
	for (GFrame* layer : layers) {
		layer->AttachStylesheet(stylesheet);
	}
}

bool GWindow::GetState() {
	return active;
}

void GWindow::SetState(bool state) {
	this->active = state;
	if (state)
		if (!active_layer) active_layer = *(layers.begin());
		this->Start();
}

void GWindow::SetActiveLayer(GFrame* layer) {
	if (layer->GetParentWindow() != this) return;

	if (this->active_layer) {
		//active_layer->CleanupElement();   //remove all textures and such
	}
	this->active_layer = layer;
	SDL_SetRenderTarget(renderer, NULL);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

SDL_Window* GWindow::GetWindow() {
	return this->window;
}

GFrame* GWindow::GetActiveLayer() {
	if (active_layer) return this->active_layer;
	return NULL;
}

SDL_Renderer* GWindow::GetRenderer() {
	return this->renderer;
}

void GWindow::AdditionalStartRoutine() {
	return;
}

void GWindow::AdditionalLoopRoutine() {
	return;
}

void GWindow::AdditionalExitRoutine() {
	return;
}

GElement* GWindow::GetElementByID(std::string ID) {

	GElement* res;
	for (GFrame* frame : layers) {
		res = frame->GetElementByID(ID);
		if (res) {
			return res;
		}
	}
	return NULL;
}


void GWindow::Start() {
	bool quit = false;
	std::chrono::high_resolution_clock::time_point start;

	active_layer->PrepareTexture();
	active_layer->DrawTexture();
	SDL_RenderPresent(renderer);

	start = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration;

	while (!quit) {

		duration = (std::chrono::high_resolution_clock::now() - start);

		if (duration.count() >= (this->render_timeout)) {

			SDL_SetRenderTarget(renderer, NULL);
			active_layer->PrepareTexture();
			active_layer->DrawTexture();
			SDL_RenderPresent(renderer);
			start = std::chrono::high_resolution_clock::now();
		}

		while (SDL_PollEvent(&event)) {

			if (event.type == SDL_QUIT) {
				quit = true;
			}
			else if (active_layer->CheckEvent(event)) {
				active_layer->HandleEvent();
			}
		}

		this->AdditionalLoopRoutine();
	}
	SDL_Quit();
	delete  this;
}


GWindow::~GWindow() {

	SDL_DestroyWindow(this->window);
	for (GFrame* layer : layers) {
		delete layer;
	}
	active_layer = NULL;
}