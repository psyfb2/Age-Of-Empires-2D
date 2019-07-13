#include "header.h"
#include "Unit.h"
#include "ImageManager.h"
#include "MainGame.h"

UnitData UNITS[] = {
	//   speed, hp, rateOfHealing, attack, attackTimeMilliSeconds, cost, attackBonusVs,    this unit
		{1,		50,  0,				10,		1200,					 3,		StoneAgeUnit2, StoneAgeUnit1}, // 8.3 DPS
		{1,     90,  0,             10,     750,                     5,      StoneAgeUnit3, StoneAgeUnit2}, // 13.3 DPS
		{1,     100, 5,             27,     1350,                    10,     StoneAgeUnit1, StoneAgeUnit3}  // 20 DPS
};
const int NUMBER_OF_UNITS = 3;
const int UNITS_PER_AGE = 3;
const double ATTACK_BONUS = 2;
const double KILL_RETURN = 1.1;

UnitFactory Unit::unitBeingCountered = StoneAgeUnit1;

Unit::Unit(int xStart, int yStart, UnitData* unitData, MainGame* mainGame, SpriteSheet* spriteSheet, SpriteSheetData* animationData, Direction moveDirection)
	: DisplayableObject(xStart, yStart, mainGame->getEngine(), spriteSheet->getWidth(), spriteSheet->getHeight()),
	animationData(animationData), moveDirection(moveDirection), speed(unitData->speed), hp(unitData->hp), mainGame(mainGame), unitData(unitData),
	healthBar(HealthBar(spriteSheet->getWidth() - 5, 7, mainGame->getEngine()))
{
	// create the animation objects
	int frameDelay = 150;

	stand  = new SpriteSheetAnimation(mainGame->getEngine(), spriteSheet, animationData->topYStand, animationData->standNumOfFrames, frameDelay, moveDirection, true);
	run    = new SpriteSheetAnimation(mainGame->getEngine(), spriteSheet, animationData->topYRun, animationData->runNumOfFrames, frameDelay, moveDirection, true);
	attack = new SpriteSheetAnimation(mainGame->getEngine(), spriteSheet, animationData->topYAttack, animationData->attackNumOfFrames, frameDelay, moveDirection, true);

	// die is an optional animation (not all characters have a die animation)
	die = nullptr;
	if (animationData->topYDie > -1 && animationData->dieNumOfFrames > 0) {
		die = new SpriteSheetAnimation(mainGame->getEngine(), spriteSheet, animationData->topYDie, animationData->dieNumOfFrames, frameDelay, moveDirection, false);
	}

	timeOfLastAttack = 0;
	baseDestroyed = false;

	currentAction = Run;

	setVisible(true);
}

Unit::~Unit()
{
	delete stand;
	delete run;
	delete attack;
	if (die != nullptr) {
		delete die;
	}
}

void Unit::virtDoUpdate(int currentTime)
{
	if (baseDestroyed || getEngine()->isPaused() || !isVisible()) {
		return;
	}

	currentMilliSeconds = currentTime;
	bool collision = false;
	Base* b;

	if (hp <= 0) {
		if (die != nullptr) {
			if (currentAction != Die) {
				speed = 0;
				currentAction = Die;
				die->reset();
			}
			if (die->getDone()) {
				removeFromGame();
			}
		}
		else {
			removeFromGame();
		}
		return;
	}

	if (currentMilliSeconds - timeOfLastHeal >= 1000) {
		timeOfLastHeal = currentMilliSeconds;
		hp += unitData->rateOfHealing;
		if (hp > unitData->hp) {
			hp = unitData->hp;
		}
	}
	// check for collision with other units
	for (int i = 0; !baseDestroyed && i < mainGame->getEngine()->getContentCount() && !collision; i++) {
		Unit* u = dynamic_cast<Unit *> (mainGame->getEngine()->getDisplayableObject(i));

		if (u != nullptr) {
			// check for collision only for units in front
			if ((moveDirection == Right && m_iCurrentScreenX + m_iDrawWidth >= u->getX() && m_iCurrentScreenX < u->getX()) ||
				(moveDirection == Left && m_iCurrentScreenX <= u->getX() + u->getDrawWidth() && m_iCurrentScreenX > u->getX())) {

				collision = true;

				if (u->getDirection() == moveDirection) {
					// colliding with a friendly unit in front, so wait for that unit to move forward first
					if (currentAction != Stand) {
						speed = 0;
						currentAction = Stand;
						stand->reset();
					}
				}
				else {
					// colliding with an enemy unit in front, so start attacking
					if (currentAction != Attack) {
						speed = 0;
						currentAction = Attack;
						attack->reset();
					}
					if (timePassed(timeOfLastAttack)) {
						if (unitData->attackBonusVs == u->getUnitData()->thisUnit) {
							u->hurt(unitData->attack * ATTACK_BONUS);
						}
						else {
							u->hurt(unitData->attack);
						}
						
						timeOfLastAttack = currentMilliSeconds;
					}
										
				}
			}
		}
		else {
			Base* baseToAttack = dynamic_cast<Base *>(mainGame->getEngine()->getDisplayableObject(i));
			if (baseToAttack != nullptr) {
				if ((moveDirection == Right && m_iCurrentScreenX + m_iDrawWidth >= baseToAttack->getX() && !baseToAttack->isLeftBase()) ||
					(moveDirection == Left && m_iCurrentScreenX <= baseToAttack->getX() + baseToAttack->getWidth() && baseToAttack->isLeftBase())) {
					collision = true;
					if (currentAction != Attack) {
						speed = 0;
						currentAction = Attack;
						attack->reset();
					}
					// reduce the hp of the base by the attack of the unit
					if (timePassed(timeOfLastAttack)) {
						baseToAttack->setHP(baseToAttack->getHP() - unitData->attack);
						timeOfLastAttack = currentMilliSeconds;
					}
				
				}
				if (baseToAttack->getHP() <= 0) {
					baseDestroyed = true;
					b = baseToAttack;
				}
			}
		}
	}

	if (!collision && currentAction != Run) {
		currentAction = Run;
		run->reset();
		speed = unitData->speed;
	}
	if (moveDirection == Right) {
		m_iCurrentScreenX += speed;
	}
	else {
		m_iCurrentScreenX -= speed;
	}
	if (baseDestroyed) {
		mainGame->NotifyBaseDestroyed(b->isLeftBase());
	}
	this->redrawDisplay();
}

void Unit::virtDraw()
{
	// display the animation
	if (isVisible()) {
		switch (currentAction) {
			case Stand:
				stand->draw(currentMilliSeconds, m_iCurrentScreenX, m_iCurrentScreenY);
				break;
			case Run:
				run->draw(currentMilliSeconds, m_iCurrentScreenX, m_iCurrentScreenY);
				break;
			case Attack:
				attack->draw(currentMilliSeconds, m_iCurrentScreenX, m_iCurrentScreenY);
				break;
			case Die:
				if (die != nullptr) {
					die->draw(currentMilliSeconds, m_iCurrentScreenX, m_iCurrentScreenY);
				}
				break;
		}
		healthBar.drawHealthBar(m_iCurrentScreenX, m_iCurrentScreenY - 10, hp / unitData->hp);
	}
}

bool Unit::compareEffectiveness(UnitFactory u1, UnitFactory u2)
{
	double u1Effectiveness;
	double u2Effectiveness;

	if (UNITS[u1].attackBonusVs == unitBeingCountered) {
		u1Effectiveness = (UNITS[u1].attack * ATTACK_BONUS * (1000 * UNITS[u1].attackTimeMilliSeconds) + UNITS[u1].rateOfHealing) / UNITS[u1].cost;
	}
	else {
		u1Effectiveness = (UNITS[u1].attack * (1000 * UNITS[u1].attackTimeMilliSeconds) + UNITS[u1].rateOfHealing) / UNITS[u1].cost;
	}

	if (UNITS[u2].attackBonusVs == unitBeingCountered) {
		u2Effectiveness = (UNITS[u2].attack * ATTACK_BONUS * (1000 * UNITS[u2].attackTimeMilliSeconds) + UNITS[u2].rateOfHealing) / UNITS[u2].cost;
	}
	else {
		u2Effectiveness = (UNITS[u2].attack * (1000 * UNITS[u2].attackTimeMilliSeconds) + UNITS[u2].rateOfHealing) / UNITS[u2].cost;
	}

	return u1Effectiveness > u2Effectiveness;
}

bool Unit::compareEffectivenessNoCounters(UnitFactory u1, UnitFactory u2)
{
	double u1Effectiveness = (UNITS[u1].attack * (1000 * UNITS[u1].attackTimeMilliSeconds) + UNITS[u1].rateOfHealing) / UNITS[u1].cost;
	double u2Effectiveness = (UNITS[u2].attack * (1000 * UNITS[u2].attackTimeMilliSeconds) + UNITS[u2].rateOfHealing) / UNITS[u2].cost;

	return u1Effectiveness > u2Effectiveness;
}

bool Unit::timePassed(int lastAttackTime)
{
	if (currentMilliSeconds - lastAttackTime >= unitData->attackTimeMilliSeconds) {
		return true;
	}
	return false;
}

void Unit::removeFromGame()
{
	mainGame->getEngine()->removeDisplayableObject(this);
	if (moveDirection == Left) {
		mainGame->setPlayer1Recources(mainGame->getPlayer1Recources() + (unitData->cost * KILL_RETURN));
		mainGame->getEngine()->increaseScore();
	}
	else {
		mainGame->setPlayer2Recources(mainGame->getPlayer2Recources() + (unitData->cost * KILL_RETURN));
	}
	delete this;
}
