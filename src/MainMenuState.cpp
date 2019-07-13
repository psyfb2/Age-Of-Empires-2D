#include "header.h"
#include "MainMenuState.h"
#include "Unit.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include "MainGame.h"
#include "GameAI.h"

MainMenuState::MainMenuState(Psyfb2* engine) : BaseState(engine)
{
}

MainMenuState::~MainMenuState()
{
}

void MainMenuState::virtSetupBackgroundBuffer()
{
	SimpleImage image = ImageManager::loadImage("images\\mainMenuBackground.png", true).resizeTo(engine->getWindowWidth(), engine->getWindowHeight());
	image.renderImage(engine->getBackgroundSurface(), 0, 0, 0, 0, engine->getWindowWidth(), engine->getWindowHeight());
}

int MainMenuState::virtInitialiseObjects()
{
	return 0;
}

void MainMenuState::virtKeyDown(int keyCode) 
{
	switch (keyCode) {
		case SDLK_ESCAPE:
			engine->setExitWithCode(0);
			break;
		case 'p':
			// play the game
			engine->changeGameState(0);
			engine->virtInitialise();
			engine->redrawDisplay(); 
			break;
		case 'l':
			// saved file in the following format
			// first line save (leftbase?, basehp, baserecources) e.g. 1,200,30
			// second line save (leftbase?, basehp, base recources) 
			// then save each unit (unitType, direction, x, y, hp) on a seperate ling e.g. 0,0,50,700,20

			// so load this into the array and start the main game
			engine->deleteAllDisplayableObjects();
			int j = 0;

			std::ifstream in("savedGame.txt");

			int numLines = 0;
			std::string unused;
			while (std::getline(in, unused)) {
				++numLines;
			}
			numLines--;
			in.close();

			
			engine->drawableObjectsChanged();
			engine->createObjectArray(2);
			std::ifstream savedGame("savedGame.txt");

			MainGame* mainGame = engine->getMainGame();

			std::string leftBase;
			std::string baseHP;
			std::string recources;
			for (int i = 0; i < 2; i++) {
				getline(savedGame, leftBase, ',');
				getline(savedGame, baseHP, ',');
				getline(savedGame, recources, '\n');
				numLines--;

				Base* b;
				
				if (leftBase == "1") { 
					b = new Base(0, engine->getWindowHeight() - mainGame->getBaseHeight(), mainGame->getBaseWidth(), mainGame->getBaseHeight(), 150, "images\\player1StoneAge.jpg", mainGame, true);
					b->setHP(stoi(baseHP));
					mainGame->setPlayer1Recources(stoi(recources));
				}
				else {
					b = new Base(engine->getWindowWidth() - mainGame->getBaseWidth(), engine->getWindowHeight() - mainGame->getBaseHeight(), mainGame->getBaseWidth(), mainGame->getBaseHeight(), 150, "images\\player2StoneAge.jpg", mainGame, false); 
					b->setHP(stoi(baseHP)); 
					mainGame->setPlayer2Recources(stoi(recources));
				}
				engine->storeObjectInArray(j, b);
				j++;
			}

			std::string unitType;
			std::string direction;
			std::string x;
			std::string y;
			std::string hp;
			for(int i = 0; i <= numLines; i++)
			{
				getline(savedGame, unitType, ',');
				getline(savedGame, direction, ',');
				getline(savedGame, x, ',');
				getline(savedGame, y, ',');
				getline(savedGame, hp, '\n');

				int unitTypeInt = stoi(unitType);

				Unit * u;
				switch (unitTypeInt) {
				case 0:
					if (direction == "0") {
						// left
						u = new Unit(stoi(x), stoi(y), &UNITS[stoi(unitType)], mainGame, mainGame->getUnitFactory1()->getSpriteSheetFlipped(), mainGame->getUnitFactory1()->getSpriteSheetData(), static_cast<Direction>(stoi(direction)));
					}
					else {
						u = new Unit(stoi(x), stoi(y), &UNITS[stoi(unitType)], mainGame, mainGame->getUnitFactory1()->getSpriteSheet(), mainGame->getUnitFactory1()->getSpriteSheetData(), static_cast<Direction>(stoi(direction)));
					}
					break;
				case 1:
					if (direction == "0") {
						u = new Unit(stoi(x), stoi(y), &UNITS[stoi(unitType)], mainGame, mainGame->getUnitFactory2()->getSpriteSheetFlipped(), mainGame->getUnitFactory2()->getSpriteSheetData(), static_cast<Direction>(stoi(direction)));
					}
					else {
						u = new Unit(stoi(x), stoi(y), &UNITS[stoi(unitType)], mainGame, mainGame->getUnitFactory2()->getSpriteSheet(), mainGame->getUnitFactory2()->getSpriteSheetData(), static_cast<Direction>(stoi(direction)));
					}
					break;
				default:
					if (direction == "0") {
						u = new Unit(stoi(x), stoi(y), &UNITS[stoi(unitType)], mainGame, mainGame->getUnitFactory3()->getSpriteSheetFlipped(), mainGame->getUnitFactory3()->getSpriteSheetData(), static_cast<Direction>(stoi(direction)));
					}
					else {
						u = new Unit(stoi(x), stoi(y), &UNITS[stoi(unitType)], mainGame, mainGame->getUnitFactory3()->getSpriteSheet(), mainGame->getUnitFactory3()->getSpriteSheetData(), static_cast<Direction>(stoi(direction)));
					}
					break;
				}
				mainGame->getEngine()->drawableObjectsChanged();
				mainGame->getEngine()->appendObjectToArray(u);
				mainGame->getEngine()->setAllObjectsVisible(true);
				std::cout << unitType << "," << direction << "," << x << "," << y << "," << hp << "\n";
				j++;
			}

			// add the AI
			engine->appendObjectToArray(new GameAI(mainGame, engine->getWindowWidth() - mainGame->getBaseWidth(), engine->getWindowHeight() - 20, mainGame->getUnitButtonArr()));

			// done call virt init objects
			engine->setAllObjectsVisible(true);
			engine->setScore(0);
			engine->changeGameState(0);
			engine->virtSetupBackgroundBuffer();
			engine->redrawDisplay();
			engine->unpause();
			break;
	}
}

void MainMenuState::virtDrawStringsOnTop()
{
}

void MainMenuState::virtMouseDown(int button, int x, int y)
{
}


