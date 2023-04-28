#pragma once

#ifndef ELEMENT_ATTRIBUTES_H
#define ELEMENT_ATTRIBUTES_H

struct ELEMENT_ATTRIBUTES {

	bool FIXED_POSITION = 0;              //related to determining frame order
	bool DISABLE_FRAMES_AT_LESSER_LEVEL = 0;   //related to determing whether to continue checking event 

	bool HIDE_ON_CURSOR_OUT = 0;               //related to determining frame order
	bool HIDE_ON_CLICKED_OUT = 0;              //related to determing frame order
	bool PUSH_TO_TOP_ON_CURSOR_IN = 0;         //related to determining frame order
	bool PUSH_TO_TOP_ON_CLICKED_IN = 0;        //related to determining frame order

	bool PUSH_TO_TOP_ON_ACTIVATE = 0;          //related to determining frame order 
	bool HIDE_ON_DEACTIVATE = 0;

};

#endif