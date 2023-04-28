#pragma once

#ifndef FRAME_FUNCTORS_H
#define FRAME_FUNCTORS_H

#include "Functor.h"
class GFrame;
class Layer;

class FrameFunction : public Functor {
protected:
	GFrame* frame;
public:
	FrameFunction(GFrame* frame);
};

class ScrollFunction : public FrameFunction {
public:
	ScrollFunction(GFrame* frame);
};

class MoveFunction : public FrameFunction {
	MoveFunction(GFrame* frame);
};

#endif


