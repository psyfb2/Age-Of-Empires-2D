#pragma once
#include "BaseState.h"

class MainMenuState :
	public BaseState
{
public:
	MainMenuState(Psyfb2* engine);
	~MainMenuState();

	virtual void virtSetupBackgroundBuffer() override;
	virtual int virtInitialiseObjects() override;
	virtual void virtKeyDown(int keyCode) override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtMouseDown(int button, int x, int y) override;

};

