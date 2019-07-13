#pragma once
#include "Psyfb2.h"

/* Acts as an interface for different game states */
class BaseState
{
public:
	BaseState(Psyfb2* engine);
	virtual ~BaseState();

	Psyfb2* getEngine() {
		return engine;
	}
	// pure virtual functions
	virtual void virtSetupBackgroundBuffer() = 0;
	virtual int virtInitialiseObjects() = 0;
	virtual void virtKeyDown(int keyCode) = 0;
	virtual void virtDrawStringsOnTop() = 0;
	virtual void virtMouseDown(int button, int x, int y) = 0;

protected:
	Psyfb2* engine;
};

