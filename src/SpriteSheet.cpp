#include "header.h"
#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(int singleImageWidth, int singleImageHeight, SimpleImage spriteSheet) 
	: spriteSheet(spriteSheet), width(singleImageWidth), height(singleImageHeight)
{
}



SpriteSheet::~SpriteSheet()
{
}
