#pragma once

#ifndef ELEMENT_H
#define ELEMENT_H
#include "includes.h"

struct ELEMENT_ATTRIBUTES;
class GFrame;
class GWindow;

class GElement {
protected:
	std::string type;
	std::string ID;

	GWindow* parent_window;
	GFrame* parent_element;
	ELEMENT_ATTRIBUTES* element_attributes;
	GStylesheet* stylesheet;

	std::vector<SDL_Texture*> texture_repository;
	int active_index;

	GElement* focussed_element_motion = NULL;
	GElement* focussed_element_click = NULL;

	bool scrollable_x;
	bool scrollable_y;
	bool movable_x;
	bool movable_y;
	bool events_enabled;
	bool render_enabled;
	bool updated;
	bool frame_free_render;

	bool altered_stylesheet;
	bool altered_attributes;
	bool stylesheet_locked;

	bool focussed_motion;
	bool focussed_click;

	int scrollfactor_x;
	int scrollfactor_y;

	Functor* Function_click_left;
	Functor* Function_click_right;
	Functor* Function_key_enter;
	Functor* Function_motion;

	SDL_Event event;
	SDL_Event last_event;
	SDL_Renderer* renderer;
	SDL_Texture* element_texture;   //this is the image to be rendered. can be anything. uninitialized
	
	SDL_Rect events_rect;
	SDL_Rect texture_rect;
	SDL_Rect absolute_position_rect;

	virtual bool CheckCursor();   //Checks if cursor position as given in event satisfies 

	virtual void OnMouseDown_left();
	virtual void OnMouseDown_right();
	virtual void OnMouseUp();
	virtual void OnKeyPressed();
	virtual void OnKeyReleased();
	virtual void OnMouseMotion_Hover();
	virtual void OnMouseMotion_Clicked_left();
	virtual void OnMouseMotion_Clicked_right();
	virtual void OnMouseWheel();

public:

	enum {FRAME_FREE, FRAME_BOUND};

	GElement(std::string ID, SDL_Renderer* renderer, SDL_Rect texture_rect);

	void SetEventsEnabled(bool events_state);
	void SetEnabled(bool both_state);
	virtual void OnRenderEnabledChange(bool change) {
		;
	}
	virtual void OnEventsEnabledChange(bool change) {
		;
	}

	virtual bool isEventsEnabled() {
		return events_enabled;
	}
	virtual bool isEnabled(){
		return events_enabled && render_enabled;
	}

	int GetWidth();
	int GetHeight();
	int GetX();							// relative to frame or layer
	int GetY();
	int GetScrollFactor_x();
	int GetScrollFactor_y();

	std::string GetID();
	std::string GetType();
	virtual std::string GetInfo();		// a sort of description of the element
	SDL_Renderer* GetRenderer();

	const ELEMENT_ATTRIBUTES* GetAttributes();
	ELEMENT_ATTRIBUTES* AlterAttributes();
	void SetAttributes(ELEMENT_ATTRIBUTES* attributes);

	void SetParentElement(GFrame* parent);
	GFrame* GetParentElement();

	virtual void AttachStylesheet(GStylesheet* stylesheet);     /////virtual in case you want to render various elements based on the stylesheet
	const GStylesheet* GetStylesheet();
	GStylesheet* AlterStylesheet();
	void LockStylesheet();
	void UnlockStylesheet();

	virtual void DrawTexture();        // renders element
	virtual void PrepareTexture();
	void SetBlending(SDL_BlendMode blendmode);

	void SetImage(int index, std::string path);
	void SetImage(int index, SDL_Texture* texture);
	void SetImage(int index, SDL_Texture* (*texture_creator)( SDL_Renderer*, GElement*));

	virtual bool InitializeTextures_from_stylesheet(); //call after calling AlterStylesheet

	virtual GElement* GetElementByID(std::string ID);

	virtual bool CheckEvent(SDL_Event e);     //checks whether an event is satisfied or not
	virtual void HandleEvent();				  //runs handling code for the event already set with previous

	void SetParentWindow(GWindow* window);
	GWindow* GetParentWindow();

	void SetTextureRect(SDL_Rect rect);
	SDL_Rect GetTextureRect();
	SDL_Rect* AlterTextureRect();

	void ExpandElementTexture(int amt_x1, int amt_y1, int amt_x2, int amt_y2);

	void SetEventsRect(SDL_Rect rect);
	SDL_Rect GetEventsRect();
	SDL_Rect* AlterEventsRect();

	void SetAbsoluteRect(SDL_Rect rect);

	void SetRenderType(int render_type);
	bool isFrameFree();

	virtual bool CheckUpdated();
	void Activate();
	void Deactivate();

	virtual bool OnFocusLostMotion();   //if cursor moved out of it
	virtual bool OnFocusLostClick();    //if clicked out of it having already clicked once
	virtual bool isFocussable_click();

	virtual bool OnFocusGainedMotion();
	virtual bool OnFocusGainedClick();

	virtual void SetFunction_click_left(Functor* func);
	virtual void SetFunction_click_right(Functor* func);
	virtual void SetFunction_key_enter(Functor* func);
	virtual void SetFunction_motion(Functor* func);

	virtual void Scroll(Direction dirn, int amount);
	virtual void Move(int amt_x, int amt_y);

	void SetScrollable(bool state_x, bool state_y);
	void SetScrollFactor_x(int factor_x);
	void SetScrollFactor_y(int factor_y);

	void SetMovable(bool state_x, bool state_y);

	void isScrollable(bool *s_x, bool *s_y);
	void isMovable(bool *m_x, bool *m_y);

	virtual ~GElement();
};

#endif
