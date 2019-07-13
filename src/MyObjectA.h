#pragma once
#include "DisplayableObject.h"
class MyObjectA :
	public DisplayableObject
{
public:
	MyObjectA(BaseEngine* pEngine);
	~MyObjectA();
	void virtDraw();
	void virtDoUpdate(int currentTime);
};

