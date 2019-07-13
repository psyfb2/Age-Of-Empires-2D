#include "header.h"
#include "Base.h"
#include "ImageManager.h"
#include "Psyfb2.h"
#include "MainGame.h"

Base::Base(int x, int y, int width, int height, double hp, std::string imageURL, MainGame* mainGame, bool leftBase)
	: DisplayableObject(x, y, mainGame->getEngine(), width, height), hp(hp), mainGame(mainGame), initialHP(hp), healthBar(HealthBar(width, 7, mainGame->getEngine(), leftBase)), leftBase(leftBase)
{
	base = ImageManager::loadImage(imageURL, false);
	base = base.resizeTo(width, height);

	explosionSpriteSheet = new SpriteSheet(480 / 5, 288 / 3, ImageManager::loadImage("images\\explosionSpriteSheet.png"));
	explosion1 = new SpriteSheetAnimation(mainGame->getEngine(), explosionSpriteSheet, 0, 5, 150, Right, false);
	explosion2 = new SpriteSheetAnimation(mainGame->getEngine(), explosionSpriteSheet, 288 / 3, 5, 150, Right, false);
	explosion3 = new SpriteSheetAnimation(mainGame->getEngine(), explosionSpriteSheet, (288 / 3) * 2, 5, 150, Right, false);

	currentMilliSeconds = 0;
	notified = false;
}

Base::~Base()
{
	delete explosionSpriteSheet;
	delete explosion1;
	delete explosion2;
	delete explosion3;
}

void Base::virtDraw()
{
	if (!isVisible()) {
		return;
	}

	base.renderImageWithMask(mainGame->getEngine()->getForegroundSurface(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, m_iDrawWidth, m_iDrawHeight);
	healthBar.drawHealthBar(m_iCurrentScreenX, m_iCurrentScreenY - 10, hp / initialHP);

	if (hp == 0) {
		// base is destroyed so display an explosion
		if (!explosion1->getDone()) {
			explosion1->draw(currentMilliSeconds, m_iCurrentScreenX, m_iCurrentScreenY - 30);
		}
		else if (!explosion2->getDone()) {
			explosion2->draw(currentMilliSeconds, m_iCurrentScreenX, m_iCurrentScreenY - 30);
		}
		else if(!explosion3->getDone()){
			explosion3->draw(currentMilliSeconds, m_iCurrentScreenX, m_iCurrentScreenY - 30);
		}
	}
}

void Base::virtDoUpdate(int milliSeconds) 
{
	if (mainGame->getEngine()->isPaused() || !isVisible()) {
		return;
	}

	currentMilliSeconds = milliSeconds;
}

void Base::setHP(int hp)
{
	if (hp > initialHP) {
		this->hp = hp;
		initialHP = hp;
	}
	else if (hp < 0) {
		this->hp = 0;
	}
	else {
		this->hp = hp;
	}
}
