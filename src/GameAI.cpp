#include "header.h"
#include "GameAI.h"
#include "MainGame.h"
#include <cmath>

GameAI::GameAI(MainGame *mainGame, int xPositionOfBase, int unitBottomYStart, ClickableImage** unitButtonArr, int callRate) :
	DisplayableObject(mainGame->getEngine(), 0, 0), mainGame(mainGame), xPositionOfBase(xPositionOfBase), callRate(callRate), unitButtonArr(unitButtonArr), unitBottomYStart(unitBottomYStart)
{
	state = Counter;
	timeOfLastAttack = 0;
	timeOfLastCall = 0;
}

GameAI::~GameAI()
{
}

void GameAI::virtDoUpdate(int currentMilliSeconds)
{
	if (currentMilliSeconds - timeOfLastCall < callRate) {
		return;
	}

	timeOfLastCall = currentMilliSeconds;

	if (mainGame->getPlayer2Recources() < getCostOfCheapestUnit()) {
		return; 
	}

	
	scanAllUnits();
	switch (state) {
		case Counter:
			if (allFriendlyUnits.size() > 0 || getPositionOfClosestEnemyUnit() < xPositionOfBase - 600) {
				return; 
			}
			// the enemy is coming, I need to produce some counter units to defend my base
			produceOptimalUnits();
			if (currentMilliSeconds - timeOfLastAttack > 30000 || mainGame->getPlayer2Recources() > mainGame->getPlayer1Recources() * 1.5) {
				state = Offense;
			}
			break;
		case Offense:
			// before we were dynamically reacting to the enemy, now the enemy will dynamically react to me
			produceOptimalUnits();
			produceHighestDPSArmy();
			state = Counter;
			timeOfLastAttack = currentMilliSeconds;
			break;
	}
}

void GameAI::scanAllUnits()
{
	allEnemyUnits.clear();
	allFriendlyUnits.clear();

	for (int i = 0; i < mainGame->getEngine()->getContentCount(); i++) {
		Unit* u = dynamic_cast<Unit *> (mainGame->getEngine()->getDisplayableObject(i));
		if (u != nullptr) {
			if (u->getDirection() == Right) {
				allEnemyUnits.push_back(u);
			}
			else {
				allFriendlyUnits.push_back(u);
			}
		}
	}
}

double GameAI::getPositionOfClosestEnemyUnit()
{
	double furthestPosition = 0;
	for (Unit* u : allEnemyUnits) {
		if (u->getX() > furthestPosition) {
			furthestPosition = u->getX();
		}
	}
	return furthestPosition;
}

void GameAI::produceHighestDPSArmy()
{
	/*
	problem - produce optimal army (maximising collective DPS)
	constraints - use up all possible recources for this army

	run a constructive heuristic to obtain the optimal solution
	*/
	int recources = mainGame->getPlayer2Recources();

	std::vector<UnitFactory> unitsByEffectiveness(UNITS_PER_AGE);
	for (int i = 0; i < UNITS_PER_AGE; i++) {
		unitsByEffectiveness.push_back(static_cast<UnitFactory>(i));
	}
	std::sort(unitsByEffectiveness.begin(), unitsByEffectiveness.end(), Unit::compareEffectivenessNoCounters);

	for (int i = 0; i < unitsByEffectiveness.size(); i++) {
		UnitFactory unitToProduce = unitsByEffectiveness[i];

		double createThisManyUnits = floor(recources / UNITS[unitToProduce].cost);
		while (createThisManyUnits >= 1.0) {
			unitButtonArr[unitToProduce]->produceUnitForAI(xPositionOfBase, unitBottomYStart);
			createThisManyUnits--;
		}
		recources = mainGame->getPlayer2Recources();
	}
}

void GameAI::produceOptimalUnits()
{
	/*
	problem - produce units which is garunteed to kill all enemy units meanwhile minimising cost of units produced
	constraints - must stay below available recources, kill all enemy units

	run a constructive heuristic to obtain the optimal solution
	*/
	int recources = mainGame->getPlayer2Recources();

	// first sort units by position descending
	std::sort(allEnemyUnits.begin(), allEnemyUnits.end(), Unit::comparePosition);

	for (Unit* u : allEnemyUnits) {
		std::vector<UnitFactory> unitsByEffectiveness = getCounterUnits(u->getUnitData()->thisUnit);
		std::cout << unitsByEffectiveness[0] << "\n";
		int i = 0;
		double enemyRemainingHP = u->getHP();

		
		do {
			UnitFactory unitToProduce = unitsByEffectiveness[i];
			int createThisManyUnits = runSimulation(unitToProduce, u, &enemyRemainingHP, recources);
			unitButtonArr[unitToProduce]->produceUnitForAI(xPositionOfBase, unitBottomYStart);
			recources = recources - (UNITS[unitToProduce].cost * createThisManyUnits);
			i++;
		} while (enemyRemainingHP > 0 && i < unitsByEffectiveness.size());
	}
}

int GameAI::runSimulation(UnitFactory unitToProduce, Unit* unitToFight, double* simulationEnemyHP, int recources)
{
	if (recources < UNITS[unitToProduce].cost) {
		return 0;
	}

	double enemyDPS;
	if (unitToFight->getUnitData()->attackBonusVs == unitToProduce) {
		enemyDPS = unitToFight->getUnitData()->attack * ATTACK_BONUS * (1000 / unitToFight->getUnitData()->attackTimeMilliSeconds);
	}
	else {
		enemyDPS = unitToFight->getUnitData()->attack * (1000 / unitToFight->getUnitData()->attackTimeMilliSeconds);
	}

	double friendlyHP = UNITS[unitToProduce].hp;
	double friendlyDPS;
	if (UNITS[unitToProduce].attackBonusVs == unitToFight->getUnitData()->thisUnit) {
		friendlyDPS = UNITS[unitToProduce].attack * ATTACK_BONUS * (1000 / UNITS[unitToProduce].attackTimeMilliSeconds);
	}
	else {
		friendlyDPS = UNITS[unitToProduce].attack * (1000.0 / UNITS[unitToProduce].attackTimeMilliSeconds);
	}

	int unitsRequired = 1;
	recources = recources - UNITS[unitToProduce].cost;
	while (*simulationEnemyHP > 0) {
		friendlyHP -= enemyDPS;
		*simulationEnemyHP -= friendlyDPS;

		friendlyHP += UNITS[unitToProduce].rateOfHealing;
		*simulationEnemyHP += unitToFight->getUnitData()->rateOfHealing;

		if (*simulationEnemyHP > unitToFight->getUnitData()->hp) {
			*simulationEnemyHP = unitToFight->getUnitData()->hp;
		}
		if (friendlyHP > UNITS[unitToProduce].hp) {
			friendlyHP = UNITS[unitToProduce].hp;
		}
		if (friendlyHP <= 0) {
			if (recources < UNITS[unitToProduce].cost) {
				break;
			}
			unitsRequired++;
			recources = recources - UNITS[unitToProduce].cost;
		}
	}
	return unitsRequired;
}

std::vector<UnitFactory> GameAI::getCounterUnits(UnitFactory unitToCounter)
{
	std::vector<UnitFactory> unitsByEffectiveness(0);
	for (int i = 0; i < UNITS_PER_AGE; i++) {
		unitsByEffectiveness.push_back(static_cast<UnitFactory>(i));
	}

	Unit::setUnitBeingCountered(unitToCounter);
	std::sort(unitsByEffectiveness.begin(), unitsByEffectiveness.end(), Unit::compareEffectiveness);
	return unitsByEffectiveness;
}
