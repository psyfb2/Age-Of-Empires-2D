#include "header.h"
#include "HealthBar.h"



HealthBar::HealthBar(int width, int height,  BaseEngine* engine, bool decreaseBarFromRight) :
	width(width), height(height), engine(engine), decreaseBarFromRight(decreaseBarFromRight)
{
}


HealthBar::~HealthBar()
{
}


void HealthBar::drawHealthBar(int x, int y, double percentageFullness)
{
	if (percentageFullness > 1.0) {
		percentageFullness = 1.0;
	}
	else if (percentageFullness < 0.0) {
		percentageFullness = 0.0;
	}

	// green if health is high, slowly turn to red if health is getting lower
	unsigned int red = 255 * (1 - percentageFullness);
	unsigned int green = 255 * percentageFullness;
	unsigned int color = ((red & 0xff) << 16) + ((green & 0xff) << 8);

	// draw outline for the health bar
	engine->drawForegroundLine(x, y, x + width, y, 0x000000);
	engine->drawForegroundLine(x, y, x, y + height, 0x000000);
	engine->drawForegroundLine(x + width, y, x + width, y + height, 0x000000);
	engine->drawForegroundLine(x, y + height, x + width, y + height, 0x000000);

	if (decreaseBarFromRight) {
		engine->drawForegroundRectangle(x, y, x + (width * percentageFullness), y + height, color);
	}
	else {
		engine->drawForegroundRectangle(x + (width * (1 - percentageFullness)), y, x + width, y + height, color);
	}
	
}