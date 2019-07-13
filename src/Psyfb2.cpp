#include "header.h"
#include "Psyfb2.h"
#include "MainGame.h"
#include "BaseState.h"
#include "PauseState.h"
#include "GameOverState.h"
#include "MainMenuState.h"
#include "DisplayableObject.h"

const int NUMBER_OF_STATES = 4;

Psyfb2::Psyfb2()
{
	gameStateArr = new BaseState*[NUMBER_OF_STATES];
	gameStateArr[0] = new MainGame(this);
	gameStateArr[1] = new PauseState(this);
	gameStateArr[2] = new GameOverState(this);
	gameStateArr[3] = new MainMenuState(this);

	firstChangeToPlay = true;
	gameState = gameStateArr[3]; 
	score = 0;
}

Psyfb2::~Psyfb2()
{
	deleteAllDisplayableObjects();

	for (int i = 0; i < NUMBER_OF_STATES; i++) {
		delete gameStateArr[i];
	}
	delete[] gameStateArr;
}

MainGame* Psyfb2::getMainGame() 
{
	return (MainGame *) gameStateArr[0];
}

void Psyfb2::virtSetupBackgroundBuffer() {
	gameState->virtSetupBackgroundBuffer();
}

int Psyfb2::virtInitialiseObjects()
{
	return gameState->virtInitialiseObjects();
}

void Psyfb2::virtKeyDown(int keyCode)
{
	gameState->virtKeyDown(keyCode);
}

/* Called in MainLoop, draw strings on top of other objects */
void Psyfb2::virtDrawStringsOnTop()
{
	gameState->virtDrawStringsOnTop();
}

void Psyfb2::virtMouseDown(int button, int x, int y)
{
	gameState->virtMouseDown(button, x, y);
}

void Psyfb2::changeGameState(int stateIndex)
{
	gameState = gameStateArr[stateIndex];
}

void Psyfb2::deleteAllDisplayableObjects()
{
	destroyOldObjects(true);
}

