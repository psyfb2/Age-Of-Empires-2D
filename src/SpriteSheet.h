#pragma once
#include "SimpleImage.h"

/* Keeps track of the width and height of an individual image in a sprite sheet */
class SpriteSheet
{
public:
	SpriteSheet(int singleImageWidth, int singleImageHeight, SimpleImage spriteSheet);
	~SpriteSheet();

	int getWidth() {
		return width;
	}

	int getHeight() {
		return height;
	}

	SimpleImage getImage() {
		return spriteSheet;
	}
private:
	int width;
	int height;
	SimpleImage spriteSheet;
};

