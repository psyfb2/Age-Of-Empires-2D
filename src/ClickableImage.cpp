#include "header.h"
#include "ClickableImage.h"
#include "MainGame.h"

const int stoneAgeUnit1Width = 48;
const int stoneAgeUnit1Height = 48;

const int stoneAgeUnit2Width = 64;
const int stoneAgeUnit2Height = 64;

const int stoneAgeUnit3Width = 96;
const int stoneAgeUnit3Height = 96;

ClickableImage::ClickableImage(int x, int y, int width, int height, MainGame* mainGame, std::string strURL, UnitFactory unitToProduce)
	: x(x), y(y), width(width), height(height), mainGame(mainGame), unitToProduce(unitToProduce)
{
	loadNewImage(strURL);
	unitData = &UNITS[unitToProduce];
	switch (unitToProduce) {
		case StoneAgeUnit1:
			spriteSheet = new SpriteSheet(stoneAgeUnit1Width, stoneAgeUnit1Height, ImageManager::loadImage("images\\StoneAgeUnit1SpriteSheet.png"));
			spriteSheetFlipped = new SpriteSheet(stoneAgeUnit1Width, stoneAgeUnit1Height, ImageManager::loadImage("images\\StoneAgeUnit1SpriteSheetFlipped.png")); // for AI
			spriteSheetData = new SpriteSheetData;
			
			spriteSheetData->topYStand = 0;
			spriteSheetData->standNumOfFrames = 4;

			spriteSheetData->topYRun = stoneAgeUnit1Height;
			spriteSheetData->runNumOfFrames = 8;

			spriteSheetData->topYAttack = stoneAgeUnit1Height * 2;
			spriteSheetData->attackNumOfFrames = 8;

			spriteSheetData->topYDie = stoneAgeUnit1Height * 3;
			spriteSheetData->dieNumOfFrames = 8;

			unitHeight = stoneAgeUnit1Height;

			break;
		case StoneAgeUnit2:
			spriteSheet = new SpriteSheet(stoneAgeUnit2Width, stoneAgeUnit2Height, ImageManager::loadImage("images\\StoneAgeUnit2SpriteSheet.png"));
			spriteSheetFlipped = new SpriteSheet(stoneAgeUnit2Width, stoneAgeUnit2Height, ImageManager::loadImage("images\\StoneAgeUnit2SpriteSheetFlipped.png"));
			spriteSheetData = new SpriteSheetData;

			spriteSheetData->topYStand = stoneAgeUnit2Height;
			spriteSheetData->standNumOfFrames = 6;

			spriteSheetData->topYRun = stoneAgeUnit2Height * 2;
			spriteSheetData->runNumOfFrames = 8;

			spriteSheetData->topYAttack = stoneAgeUnit2Height * 3;
			spriteSheetData->attackNumOfFrames = 5;

			spriteSheetData->topYDie = 0;
			spriteSheetData->dieNumOfFrames = 8;

			unitHeight = stoneAgeUnit2Height - 15;

			break;
		case StoneAgeUnit3:
			spriteSheet = new SpriteSheet(stoneAgeUnit3Width, stoneAgeUnit3Height, ImageManager::loadImage("images\\StoneAgeUnit3SpriteSheet.png")); 
			spriteSheetFlipped = new SpriteSheet(stoneAgeUnit3Width, stoneAgeUnit3Height, ImageManager::loadImage("images\\StoneAgeUnit3SpriteSheetFlipped.png"));
			spriteSheetData = new SpriteSheetData;

			spriteSheetData->topYStand = 5 * stoneAgeUnit3Height;
			spriteSheetData->standNumOfFrames = 6;

			spriteSheetData->topYRun = stoneAgeUnit3Height;
			spriteSheetData->runNumOfFrames = 8;

			spriteSheetData->topYAttack = 3 * stoneAgeUnit3Height;
			spriteSheetData->attackNumOfFrames = 9;

			spriteSheetData->topYDie = 9 * stoneAgeUnit3Height;
			spriteSheetData->dieNumOfFrames = 6;

			unitHeight = stoneAgeUnit3Height - 30;
			break;
		
	}
}

ClickableImage::~ClickableImage()
{
	delete spriteSheet;
	delete spriteSheetFlipped;
	delete spriteSheetData;
}

void ClickableImage::draw()
{
	image.renderImageWithMask(mainGame->getEngine()->getBackgroundSurface(), 0, 0, x, y, width, height);
}

void ClickableImage::clicked(int button, int clickX, int clickY, int unitX, int unitBottomY, Direction direction, bool player1)
{
	if ((button == SDL_BUTTON_LEFT && clickX >= x && clickX <= x + width && clickY >= y && clickY <= y + height && !mainGame->getEngine()->isPaused()) || !player1) {
		// produce unit dynamically using factory pattern
		Unit *u = nullptr;
		int recources;
		SpriteSheet* spriteSheetToUse;
		if (player1) {
			recources = mainGame->getPlayer1Recources();
			spriteSheetToUse = spriteSheet;
		}
		else {
			recources = mainGame->getPlayer2Recources();
			spriteSheetToUse = spriteSheetFlipped;
		}
		if (recources >= unitData->cost) {
			u = new Unit(unitX, unitBottomY - unitHeight, unitData, mainGame, spriteSheetToUse, spriteSheetData, direction);
			if (player1) {
				mainGame->setPlayer1Recources(recources - unitData->cost);
			}
			else {
				mainGame->setPlayer2Recources(recources - unitData->cost);
			}
					
		}
		else {
			return;
		}
		if (u != nullptr) {
			mainGame->getEngine()->drawableObjectsChanged();
			mainGame->getEngine()->appendObjectToArray(u);
			mainGame->getEngine()->setAllObjectsVisible(true);
		}
	}
}

void ClickableImage::produceUnitForAI(int unitX, int unitBottomY)
{
	clicked(0, 0, 0, unitX, unitBottomY, Left, false);
}

void ClickableImage::loadNewImage(std::string strURL) 
{
	image = ImageManager::loadImage(strURL, true);
	image = image.resizeTo(width, height);
}