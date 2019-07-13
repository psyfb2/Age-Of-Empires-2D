#pragma once
#include "BaseState.h"
class GameOverState :
	public BaseState
{
public:
	GameOverState(Psyfb2* engine);
	~GameOverState();

	virtual void virtSetupBackgroundBuffer() override;
	virtual int virtInitialiseObjects() override;
	virtual void virtKeyDown(int keyCode) override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtMouseDown(int button, int x, int y) override;

	void saveGame();
	void moveToMainMenu();
private:
	int score;
	char userText[256];
	int lastChar;
	bool player1Won;
};

