#pragma once
#include "BaseEngine.h"

class HealthBar
{
public:
	HealthBar(int width, int height, BaseEngine* engine, bool decreaseBarFromRight = true);
	~HealthBar();

	void drawHealthBar(int x, int y, double percentageFullness);
private:
	int width;
	int height;
	bool decreaseBarFromRight;
	BaseEngine* engine;
};

