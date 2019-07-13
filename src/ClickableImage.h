#pragma once
#include "ImageObject.h"
#include "Unit.h"
#include "SpriteSheet.h"

class MainGame;

/* Factory for units */
class ClickableImage
{
public:
	ClickableImage(int x, int y, int width, int height, MainGame* mainGame, std::string strURL, UnitFactory unitToProduce);
	~ClickableImage();
	void draw();

	SpriteSheet* getSpriteSheet() {
		return spriteSheet;
	}

	SpriteSheetData* getSpriteSheetData() {
		return spriteSheetData;
	}

	SpriteSheet* getSpriteSheetFlipped() {
		return spriteSheetFlipped;
	}

	/* Produces a unit dynamically */
	void clicked(int button, int x, int y, int unitX, int unitBottomY, Direction direction = Right, bool player1 = true);

	void loadNewImage(std::string strURL);

	/* Call from the AI to produce a unit */
	void produceUnitForAI(int unitX, int bottomY);
private:
	SimpleImage image;
	int x;
	int y;
	int width;
	int height;
	int unitHeight;
	UnitFactory unitToProduce;
	MainGame* mainGame;

	SpriteSheet* spriteSheet;
	SpriteSheet* spriteSheetFlipped;
	SpriteSheetData* spriteSheetData;

	UnitData* unitData;
};

