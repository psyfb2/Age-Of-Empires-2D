#include "header.h"
#include "MainGame.h"
#include "ImageManager.h"
#include "Psyfb2.h"
#include "GameAI.h"
#include "Unit.h"
#include <fstream>

MainGame::MainGame(Psyfb2* engine) :
	BaseState(engine), line1Y(100), line2Y(200), player1Recources(20), player2Recources(20), baseWidth(1280 / 8), baseHeight(720 / 8), recourceCap(40)
{
	int w = 64;
	int h = 64;

	unitButtonArr = new ClickableImage*[UNITS_PER_AGE];
	unitButtonArr[0] = new ClickableImage(baseWidth, 0, w, h, this, "images\\StoneAgeUnit1.png", StoneAgeUnit1);
	unitButtonArr[1] = new ClickableImage(baseWidth + w + 40, 0, w, h, this, "images\\StoneAgeUnit2.png", StoneAgeUnit2);
	unitButtonArr[2] = new ClickableImage(baseWidth + (w * 2) + 40, 0, w, h, this, "images\\StoneAgeUnit3.png", StoneAgeUnit3);

	notified = false;
	score = 0;
}

MainGame::~MainGame()
{
	for (int i = 0; i < UNITS_PER_AGE; i++) {
		delete unitButtonArr[i];
	}
	delete[] unitButtonArr;
	engine->destroyOldObjects(true);
}

void MainGame::virtSetupBackgroundBuffer()
{
	SimpleImage image = ImageManager::loadImage("images\\background.png", false);
	image.renderImage(engine->getBackgroundSurface(), 0, 0, 0, 0, engine->getWindowWidth(), engine->getWindowHeight());

	// display unit creation buttons
	for (int i = 0; i < UNITS_PER_AGE; i++) {
		(unitButtonArr[i])->draw();
	}

	// draw lines which special ability deployer will move across
	engine->drawBackgroundLine(baseWidth, line1Y, engine->getWindowWidth() - baseWidth, line1Y, 0x000000);
	engine->drawBackgroundLine(baseWidth, line2Y, engine->getWindowWidth() - baseWidth, line2Y, 0x000000);

	// vertical barrier lines
	engine->drawBackgroundLine(baseWidth, line1Y + 100, baseWidth, 0, 0x000000);
	engine->drawBackgroundLine(engine->getWindowWidth() - baseWidth, line1Y + 100, engine->getWindowWidth() - baseWidth, 0, 0x000000);

	engine->drawBackgroundString(600, 5, "Press 's' to save the game", 0x000000);
}

int MainGame::virtInitialiseObjects()
{
	// tell framework to reload array of objects
	engine->createObjectArray(5);

	int radius = 30;

	// Special ability Deployers
	engine->storeObjectInArray(0,
		new SpecialAbilityDeployer(engine, (rand() % (engine->getWindowWidth() - 2 * baseWidth)) + radius + 2 * baseWidth, line1Y, radius, 0x0000ff, "P1", -radius / 2, baseWidth, engine->getWindowWidth() - baseWidth));

	engine->storeObjectInArray(1,
		new SpecialAbilityDeployerAI(engine, (rand() % (engine->getWindowWidth() - 2 * baseWidth)) + radius + 2 * baseWidth, line2Y, radius, 0xff0000, "P2", -radius / 2, baseWidth, engine->getWindowWidth() - baseWidth));

	engine->storeObjectInArray(2,
		new Base(0, engine->getWindowHeight() - baseHeight, baseWidth, baseHeight, 150, "images\\player1StoneAge.jpg", this, true));

	engine->storeObjectInArray(3,
		new Base(engine->getWindowWidth() - baseWidth, engine->getWindowHeight() - baseHeight, baseWidth, baseHeight, 150, "images\\player2StoneAge.jpg", this, false));

	engine->storeObjectInArray(4,
		new GameAI(this, engine->getWindowWidth() - baseWidth, engine->getWindowHeight() - 20, unitButtonArr));

	engine->setAllObjectsVisible(true);
	return 0;
}

void MainGame::virtKeyDown(int keyCode)
{
	switch (keyCode) {
	case SDLK_ESCAPE:
		engine->setExitWithCode(0);
		break;
	case SDLK_SPACE:
		// go to pause state
		engine->changeGameState(1);
		engine->pause();
		break;
	case 's':
		// first line save (leftbase?, basehp, baserecources) e.g. 1,200,30
		// second line save (leftbase?, basehp, base recources) 
		// then save each unit (unitType, direction, x, y, hp) on a seperate ling e.g. 0,0,50,700,20
		std::ofstream game("savedGame.txt");

		for (int i = 0; i < engine->getContentCount(); i++) {
			Base* b = dynamic_cast<Base *>(engine->getDisplayableObject(i));
			if (b != nullptr) {
				if (b->isLeftBase()) {
					game << "1," << b->getHP() << "," << player1Recources << std::endl;
				}
				else {
					game << "0," << b->getHP() << "," << player2Recources << std::endl;
				}
			}
		}

		for (int i = 0; i < engine->getContentCount(); i++) {
			Unit* u = dynamic_cast<Unit *>(engine->getDisplayableObject(i));
			if (u != nullptr) {
				game << u->getUnitData()->thisUnit << "," << u->getDirection() << "," << u->getX() << "," << u->getY() << "," << u->getHP() << std::endl;
			}
		}
		
		game.close();
		break;
	}
}

void MainGame::virtMouseDown(int button, int x, int y)
{
	for (int i = 0; i < UNITS_PER_AGE; i++) {
		(unitButtonArr[i])->clicked(button, x, y, baseWidth - 50, engine->getWindowHeight() - 20);
	}
}

void MainGame::virtDrawStringsOnTop()
{
	// display how much recources each player has
	char playerRecourcesText[256];
	sprintf(playerRecourcesText, "Recources: %d", player1Recources);
	engine->drawForegroundString(20, engine->getWindowHeight() - baseWidth - 50, playerRecourcesText, 0x00ff00);

	sprintf(playerRecourcesText, "Recources: %d", player2Recources);
	engine->drawForegroundString(engine->getWindowWidth() - baseWidth - 90, engine->getWindowHeight() - baseWidth - 50, playerRecourcesText, 0xff0000);
}

void MainGame::NotifyBaseDestroyed(bool leftBase)
{
	// go to gameover state
	notified = true;
	engine->setPlayer1Won(!leftBase);
	moveToGameOver();
}

void MainGame::moveToGameOver() 
{
	engine->changeGameState(2);
	engine->virtSetupBackgroundBuffer();
	engine->redrawDisplay();
}