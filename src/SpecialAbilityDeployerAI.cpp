#include "header.h"
#include "SpecialAbilityDeployerAI.h"
#include "BaseEngine.h"


SpecialAbilityDeployerAI::SpecialAbilityDeployerAI(BaseEngine* engine, int centreX, int centreY, int radius, unsigned int color, const char* text, int textOffset, int leftBarrier, int rightBarrier)
	: SpecialAbilityDeployer(engine, centreX, centreY, radius, color, text, textOffset, leftBarrier, rightBarrier), autoMoveDirection(Right)
{
}


SpecialAbilityDeployerAI::~SpecialAbilityDeployerAI()
{
}

void SpecialAbilityDeployerAI::virtDoUpdate(int currentTime)
{
	if (getEngine()->isPaused() || !isVisible()) {
		return;
	}
	// shoot special ability onto enemy units
	// for now just move this left to right (for CW3)
	if (autoMoveDirection == Right) {
		m_iCurrentScreenX += speed;
		if (!isInBorders()) {
			m_iCurrentScreenX -= speed;
			autoMoveDirection = Left;
		} 
	}
	else {
		m_iCurrentScreenX -= speed;
		if (!isInBorders()) {
			m_iCurrentScreenX += speed;
			autoMoveDirection = Right;
		}
	}

}
