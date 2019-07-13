#pragma once
#include "BaseState.h"

class PauseState :
	public BaseState
{
public:
	PauseState(Psyfb2* engine);
	~PauseState();

	virtual void virtSetupBackgroundBuffer() override;
	virtual int virtInitialiseObjects() override;
	virtual void virtKeyDown(int keyCode) override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtMouseDown(int button, int x, int y) override;
};

