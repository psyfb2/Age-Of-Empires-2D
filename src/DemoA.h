#pragma once
#include "BaseEngine.h"
#include "MyTileManager.h"

class DemoA :
	public BaseEngine
{
public:
	DemoA();
	~DemoA();
	// Override parent method to draw background
	void virtSetupBackgroundBuffer();
	// Identify mouse clicks
	void virtMouseDown(int button, int x, int y);
	// Called after a keypress
	void virtKeyDown(int keyCode);

protected:
	MyTileManager tm;
public:
	int virtInitialiseObjects();
};

