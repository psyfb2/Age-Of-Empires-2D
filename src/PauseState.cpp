#include "header.h"
#include "PauseState.h"

PauseState::PauseState(Psyfb2* engine) :
	BaseState(engine)
{
}

PauseState::~PauseState()
{
	std::cout << "Deleted\n";
}

void PauseState::virtSetupBackgroundBuffer()
{
}

int PauseState::virtInitialiseObjects()
{
	return 0;
}

void PauseState::virtKeyDown(int keyCode)
{
	switch (keyCode) {
	case SDLK_ESCAPE:
		engine->setExitWithCode(0);
		break;
	case SDLK_SPACE:
		// go to play state
		engine->changeGameState(0);
		engine->unpause();
		break;
	}
}

void PauseState::virtDrawStringsOnTop()
{
	engine->drawForegroundString((engine->getWindowWidth() / 2) - 100, (engine->getWindowHeight() / 2) - 100, "***GAME PAUSED***", 0x000000);
}

void PauseState::virtMouseDown(int button, int x, int y)
{
}
