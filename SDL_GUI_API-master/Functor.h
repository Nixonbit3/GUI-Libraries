#pragma once

#ifndef FUNCTOR_H
#define FUNCTOR_H

class GWindow;

class Functor {
protected:
	GWindow* window;
public:
	Functor(GWindow* window) {
		this->window = window;;
	}
	virtual void operator()() {
		return;
	}
};

#endif
