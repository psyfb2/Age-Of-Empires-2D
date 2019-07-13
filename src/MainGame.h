#pragma once
#include "BaseState.h"
#include "ClickableImage.h"

enum Age { StoneAge, CastleAge, ModernWarfare };
enum State { Play, Pause, GameOver };
enum UnitType {};

class MainGame :
	public BaseState
{
public:
	MainGame(Psyfb2* engine);
	~MainGame();

	virtual void virtSetupBackgroundBuffer() override;
	virtual int virtInitialiseObjects() override;
	virtual void virtKeyDown(int keyCode) override;
	virtual void virtDrawStringsOnTop() override;
	virtual void virtMouseDown(int button, int x, int y) override;

	int getPlayer1Recources() {
		return player1Recources;
	}

	int getPlayer2Recources() {
		return player2Recources;
	}

	void setPlayer1Recources(int r) {
		player1Recources = r;
		if (player1Recources > recourceCap) {
			player1Recources = recourceCap;
		}
	}

	void setPlayer2Recources(int r) {
		player2Recources = r;
		if (player2Recources > recourceCap) {
			player2Recources = recourceCap;
		} 
	}

	int getBaseWidth() {
		return baseWidth;
	}

	int getBaseHeight() {
		return baseHeight;
	}

	void increaseScore() {
		score++;
	}

	ClickableImage* getUnitFactory1() {
		return unitButtonArr[0];
	}

	ClickableImage* getUnitFactory2() {
		return unitButtonArr[1];
	}

	ClickableImage* getUnitFactory3() {
		return unitButtonArr[2];
	}

	ClickableImage** getUnitButtonArr() {
		return unitButtonArr;
	}

	void NotifyBaseDestroyed(bool leftBase);

	void moveToGameOver();

private:
	int line1Y;
	int line2Y;
	int player1Recources;
	int player2Recources;
	int baseWidth;
	int baseHeight;
	int recourceCap;
	int score;
	bool notified;
	ClickableImage** unitButtonArr;
};

