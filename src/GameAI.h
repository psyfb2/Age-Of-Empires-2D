#pragma once
#include "DisplayableObject.h"
#include "Unit.h"
#include "ClickableImage.h"
#include <vector>

class MainGame;
/* 
An AI for Age of Empires 2D, uses 
	1. Markov State Machine
	2. Constructive heuristic to find optimal army composition
*/

enum AiState {
	/* 
	Counter state the AI is trying to counter all your units but is staying defensive
	Attack state the AI is trying to destroy your base
	*/
	Offense, Counter
};

class GameAI : public DisplayableObject
{
public:
	GameAI(MainGame* mainGame, int xPositionOfBase, int unitBottomYStart, ClickableImage** unitButtonArr, int callRate = 250);
	~GameAI();
	void virtDoUpdate(int currentMilliSeconds) override;
	void virtDraw() override {

	}

	/* Populates allEnemyUnits and allFriendlyUnits*/
	void scanAllUnits();

	double getPositionOfClosestEnemyUnit();

	/* 
	Performs a search to find optimal unit to produce depending on the enemy's army composition 
	Current recources is a constraint
	Then produces these units
	*/
	void produceOptimalUnits();

	/* Uses up all available recources to create the best possible army (assumes enemy will have no units on the field) */
	void produceHighestDPSArmy();

	/* run a quick simulation to see how many units are required to kill this unit while not exceeding recources */
	int runSimulation(UnitFactory unitToProduce, Unit* unitToFight, double* simulationEnemyHP, int recources);

	/* For now haven't implemented different ages to just return stone age unit 1 */
	int getCostOfCheapestUnit() {
		return UNITS[StoneAgeUnit1].cost;
	}

	/* returns the most effective units against a given unit from best to worst (takes into account the counter unit) */
	std::vector<UnitFactory> getCounterUnits(UnitFactory unitToCounter);

private:
	AiState state;
	std::vector<Unit *> allEnemyUnits;
	std::vector<Unit *> allFriendlyUnits;
	ClickableImage** unitButtonArr; // assume in the right order and of size UNITS_PER_AGE
	MainGame* mainGame;
	int timeOfLastAttack;
	int xPositionOfBase;
	int unitBottomYStart;
	int callRate; // period of time in milliseconds for which AI will be called once
	int timeOfLastCall;
};

