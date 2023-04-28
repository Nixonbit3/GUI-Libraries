#pragma once

#ifndef WINDOW_H
#define WINDOW_H
#include "includes.h"

class GFrame;
struct GStylesheet;
class GElement;

class GWindow {
protected:
	bool active;

	SDL_Renderer* renderer;
	SDL_Window* window;
	SDL_Event event;

	SDL_Rect test_rect;

	double render_timeout;

	GStylesheet* stylesheet;
	GFrame* active_layer;
	std::list<GFrame*> layers;
	std::unordered_map<std::string, GElement*> id_map;

	virtual void Start();
	virtual void AdditionalStartRoutine();
	virtual void AdditionalLoopRoutine();
	virtual void AdditionalExitRoutine();

public:
	GWindow(SDL_Renderer* renderer, SDL_Window* window);
	GWindow(std::string title, bool centered_x, bool centered_y, int w, int h, Uint32 flags);

	void AddLayer(GFrame* layer);
	void AttachStylesheet(GStylesheet * stylesheet);
	void SetActiveLayer(GFrame* layer);
	GFrame* GetActiveLayer();

	GElement* GetElementByID(std::string ID);
	bool GetState();
	void SetState(bool state);

	SDL_Window* GetWindow();
	SDL_Renderer* GetRenderer();

	~GWindow();


};

#endif`