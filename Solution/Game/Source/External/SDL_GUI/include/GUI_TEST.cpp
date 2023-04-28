#include "SDL_GUI.h"

class Somefunc : public Functor {
public:
	GWindow* win;
	GFeedFrame* frame;
	Somefunc(GWindow* win, GFeedFrame* frame) : Functor(win) {
		this->win = win;
		this->frame = frame;
	}
	void operator()() {
		frame->AddElement(new GButton("newbtn", win->GetRenderer(), rect_maker(0, 0, 180, 40)));
		if (frame->GetParentElement()) {
			((GFeedFrame*)(frame->GetParentElement()))->NotifyChildDimensionsChanged();
			frame->GetParentElement()->GetParentElement()->NotifyChildDimensionsChanged();
		}
	}
};


class Somefunc2 : public Functor {
public:
	GWindow* win;
	GFeedFrame* frame;
	Somefunc2(GWindow* win, GFeedFrame* frame) : Functor(win) {
		this->win = win;
		this->frame = frame;
	}
	void operator()() {
		frame->RemoveElement(0);
	}
};

int main(int argc, char** argv) {

	GStylesheet default_;

	default_.accent = &red;
	default_.background = &white;
	default_.foreground = &black;
	default_.neutral = &green;
	default_.frame_background_neutral = &blue;

	TTF_Init();
	TTF_Font* font_face = TTF_OpenFont("C:/ASMAN.ttf", 20);

	font_data font_data_;
	font_data_.color = &blue;
	font_data_.face = font_face;

	default_.font = &font_data_;
	

	GWindow* win = new GWindow("window1", false, false, 800, 600, NULL);
	GFeedFrame* frame1 = new GFeedFrame("frame1", win->GetRenderer(), rect_maker(0, 0, 220, 1), DIRECTION_Y);
	frame1->SetExpanding(true);

	GFeedFrame* frame2 = new GFeedFrame("frame3", win->GetRenderer(), rect_maker(0, 0, 200, 1), DIRECTION_Y);
	frame2->SetExpanding(true);
	frame2->AddElement(new GButton("newbtn", win->GetRenderer(), rect_maker(0, 0, 180, 40)));
	
	GButton* btn1 = new GButton("creator_button", win->GetRenderer(), rect_maker(0, 0, 200, 80));
	frame1->AddElement(btn1, GFeedFrame::PUSH_BOTTOM);
	GButton* btn2 = new GButton("destructor_button", win->GetRenderer(), rect_maker(0, 0, 200, 80));
	frame1->AddElement(btn2, GFeedFrame::PUSH_BOTTOM);

	frame1->AddElement(frame2, GFeedFrame::PUSH_TOP);

	Somefunc test_func(win, frame2);
	btn1->SetFunction_click_left(&test_func);

	Somefunc2 test_func2(win, frame1);
	btn2->SetFunction_click_left(&test_func2);

	GFrame* frame3 = new GFrame("frame1", win->GetRenderer(), rect_maker(0, 0, 800, 600));
	frame3->AddElement(frame1);
	frame3->SetContainingScrollable(false, true);
	win->AddLayer(frame3);

	win->AttachStylesheet(&default_);

	frame1->AlterStylesheet()->frame_background_neutral = &black;
	frame1->InitializeTextures_from_stylesheet();
	frame2->AlterStylesheet()->frame_background_neutral = &green;
	frame2->InitializeTextures_from_stylesheet();
	win->SetActiveLayer(frame3);
	win->SetState(true); 

	return 0;
}