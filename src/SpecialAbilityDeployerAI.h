#pragma once
#include "SpecialAbilityDeployer.h"

enum Direction {Left, Right};
class SpecialAbilityDeployerAI :
	public SpecialAbilityDeployer
{
public:
	SpecialAbilityDeployerAI(BaseEngine* engine, int centreX, int centreY, int radius, unsigned int color, const char* text, int textOffset, int leftBarrier, int rightBarrier);
	~SpecialAbilityDeployerAI();
	void virtDoUpdate(int currentTime) override;

protected:
	Direction autoMoveDirection;
};

