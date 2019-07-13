#pragma once
#include "DisplayableObject.h"
#include "SimpleImage.h"
#include "SpecialAbilityDeployerAI.h"
#include "SpriteSheet.h"
#include "SpriteSheetAnimation.h"
#include "HealthBar.h"
#include "Base.h"


enum UnitState {
	Stand, Run, Attack, Die
};

enum UnitFactory {
	StoneAgeUnit1 = 0, StoneAgeUnit2 = 1, StoneAgeUnit3 = 2
};

struct UnitData {
	double speed;
	double hp;
	double rateOfHealing; // healing per second
	double attack;
	double attackTimeMilliSeconds; // delay between attacks (sync with animation :D) 
	int cost;
	UnitFactory attackBonusVs; // does 1.5 * attack to this unit
	UnitFactory thisUnit;
};

extern UnitData UNITS[];
extern const int NUMBER_OF_UNITS;
extern const int UNITS_PER_AGE;
extern const double ATTACK_BONUS;

struct SpriteSheetData {
	int topYStand;
	int standNumOfFrames;

	int topYRun;
	int runNumOfFrames;

	int topYAttack;
	int attackNumOfFrames;

	// set to -1 if unit doesnt have a death animation
	int topYDie;
	int dieNumOfFrames;
};

class Unit :
	public DisplayableObject
{
public:
	/* Assuming that every unit has a sprite sheet for 3 animation: stand, run, attack and optionally death animation */
	Unit(int xStart, int yStart, UnitData* unitData, MainGame* mainGame, SpriteSheet* spriteSheet, SpriteSheetData* animationData, Direction moveDirection);
	~Unit();
	virtual void virtDoUpdate(int currentTime) override;
	virtual void virtDraw() override;

	void removeFromGame();

	double getX() {
		return m_iCurrentScreenX;
	}

	double getY() {
		return m_iCurrentScreenY;
	}

	Direction getDirection() {
		return moveDirection;
	}

	UnitData* getUnitData() {
		return unitData;
	}

	double getHP() {
		return hp;
	}

	void hurt(double damage) {
		hp -= damage;
		if (hp < 0) {
			hp = 0;
		}
	}

	/* Used to sort position of units descending */
	static bool comparePosition(Unit* u1, Unit* u2) {
		return u1->getX() > u2->getX();
	}

	/* Call before calling compareEffectiveness to set the unit (so that counters to this unit are considered) */
	static void setUnitBeingCountered(UnitFactory u) {
		unitBeingCountered = u;
	}

	/* Used to sort units by effectiveness against a unit set in the SetUnitBeingCountered method descending */
	static bool compareEffectiveness(UnitFactory u1, UnitFactory u2);

	/* Used to sort units by effectiveness of a unit (this will not take into account counter units) */
	static bool compareEffectivenessNoCounters(UnitFactory u1, UnitFactory u2);

private:
	static UnitFactory unitBeingCountered;
	bool timePassed(int lastAttackTime);

	MainGame* mainGame;

	Direction moveDirection;

	UnitState currentAction;
	UnitData* unitData;

	SpriteSheetData *animationData;
	SpriteSheetAnimation* stand;
	SpriteSheetAnimation* run;
	SpriteSheetAnimation* attack;
	SpriteSheetAnimation* die;

	HealthBar healthBar;

	double speed;
	double hp;
	int timeOfLastAttack; // used for timing purposes
	int timeOfLastHeal;
	int currentMilliSeconds;
	bool baseDestroyed;
};

