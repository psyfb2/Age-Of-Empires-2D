#include "header.h"
#include "GameOverState.h"
#include <fstream>

GameOverState::GameOverState(Psyfb2* engine) :
	BaseState(engine)
{
	lastChar = 0;
	userText[lastChar] = '\0'; // null terminate

}

GameOverState::~GameOverState()
{
}

void GameOverState::virtSetupBackgroundBuffer()
{
	score = engine->getScore();
	player1Won = engine->getPlayer1Won();

	SimpleImage image;
	if (player1Won) {
		image = ImageManager::loadImage("images\\youWin.jpg", true).resizeTo(engine->getWindowWidth(), engine->getWindowHeight());
	}
	else {
		image = ImageManager::loadImage("images\\gameOver.png", true).resizeTo(engine->getWindowWidth(), engine->getWindowHeight());
	}
	image.renderImage(engine->getBackgroundSurface(), 0, 0, 0, 0, engine->getWindowWidth(), engine->getWindowHeight());

	char bufScore[1028];
	sprintf(bufScore, "Units Killed: %d", score);
	engine->drawBackgroundString(100, 450, bufScore, 0xffffff);

	std::ifstream highScoreFile("highScore.txt");
	char name[1028];
	int score = 0;
	highScoreFile >> score >> name;
	sprintf(bufScore, "Current Kill Record Held By '%s' With %d Kills", name + 1, score);
	engine->drawBackgroundString(450, 500, bufScore, 0xffffff);

	engine->drawBackgroundString(0, 0, "Press Enter to Sumbit Your Score and Go Back To The Menu", 0x000000);
}

int GameOverState::virtInitialiseObjects()
{
	return 0;
}

void GameOverState::virtKeyDown(int keyCode)
{
	switch (keyCode) {
		case SDLK_ESCAPE:
			engine->setExitWithCode(0);
			break;
		case SDLK_RETURN:
			// save the game and go to the main menu
			saveGame();
			moveToMainMenu();
			break;
		case SDLK_BACKSPACE:
			if (lastChar <= 0) {
				break;
			}
			userText[--lastChar] = '\0';
			break;
		case SDLK_SPACE:
			if (lastChar < 512) {
				userText[lastChar++] = keyCode;
				userText[lastChar] = '\0';
			}
		default:
			if (keyCode >= 'a' && keyCode <= 'z' && lastChar < 512) {
				userText[lastChar++] = keyCode;
				userText[lastChar] = '\0';
			}
	}
}

void GameOverState::virtDrawStringsOnTop()
{
	char bufText[1028];
	sprintf(bufText, "Your Name: %s", userText);
	engine->drawForegroundString(100, 550, bufText, 0xffffff);
}

void GameOverState::virtMouseDown(int button, int x, int y)
{
}

void GameOverState::saveGame()
{
	int currentHighScore = 0;
	std::ifstream highScoreFile("highScore.txt");

	if (highScoreFile >> currentHighScore) {
		if (score < currentHighScore) {
			return;
		}
	}

	highScoreFile.close();

	std::ofstream highScoreFileOut("highScore.txt");

	highScoreFileOut << score << "," << userText << std::endl;

	highScoreFileOut.close();
}

void GameOverState::moveToMainMenu()
{
	engine->changeGameState(3);
	engine->virtSetupBackgroundBuffer();
	engine->destroyOldObjects(true);
	engine->redrawDisplay();
}