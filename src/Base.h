#pragma once
#include "DisplayableObject.h"
#include "SimpleImage.h"
#include "HealthBar.h"
#include "SpriteSheet.h"
#include "SpriteSheetAnimation.h"

class MainGame; // forward declare to stop circular include

class Base :
	public DisplayableObject
{
public:
	Base(int x, int y, int width, int height, double hp, std::string imageURL, MainGame* mainGame, bool leftBase);
	~Base();

	void virtDraw() override;
	void virtDoUpdate(int milliSeconds) override;

	int getHP() {
		return hp;
	}

	void setHP(int hp);

	int getX() {
		return m_iCurrentScreenX;
	}

	int getWidth() {
		return m_iDrawWidth;
	}

	bool isLeftBase() {
		return leftBase;
	}

private:
	HealthBar healthBar;
	MainGame* mainGame;
	SimpleImage base;
	SpriteSheetAnimation* explosion1;
	SpriteSheetAnimation* explosion2;
	SpriteSheetAnimation* explosion3;
	SpriteSheet* explosionSpriteSheet;
	bool leftBase;
	double hp;
	double initialHP;
	int currentMilliSeconds;
	bool notified;
};

