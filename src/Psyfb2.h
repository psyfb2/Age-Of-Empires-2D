#pragma once
#include "BaseEngine.h"

class MainGame;
class BaseState;

class Psyfb2 :
	public BaseEngine
{
public:
	Psyfb2();
	~Psyfb2();
	MainGame * getMainGame();
	virtual void virtSetupBackgroundBuffer() override;
	virtual int virtInitialiseObjects() override;
	virtual void virtKeyDown(int keyCode) override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtMouseDown(int button, int x, int y) override;

	void changeGameState(int stateIndex);
	void deleteAllDisplayableObjects();

	void setScore(int score) {
		this->score = score;
	}

	void increaseScore() {
		std::cout << "Score: score";
		score++;
	}

	void setPlayer1Won(bool player1Won) {
		this->player1Won = player1Won;
	}

	int getScore() {
		return score;
	}

	bool getPlayer1Won() {
		return player1Won;
	}

private: 
	BaseState** gameStateArr;
	BaseState* gameState;
	bool firstChangeToPlay;
	bool player1Won;
	int score;
};

