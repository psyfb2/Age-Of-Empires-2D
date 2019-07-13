#include "header.h"
#include "SpecialAbilityDeployer.h"
#include "BaseEngine.h"

// will not deviate from xStart
SpecialAbilityDeployer::SpecialAbilityDeployer(BaseEngine* engine, int centreX, int centreY, int radius, unsigned int color, const char* text, int textOffset, int leftBarrier, int rightBarrier)
	: DisplayableObject(centreX, centreY, engine, radius * 2, radius * 2, false),
	color(color), text(text), textOffset(textOffset), leftBarrier(leftBarrier), rightBarrier(rightBarrier), speed(3)
{
	if (!isInBorders()) {
		m_iCurrentScreenX = leftBarrier - m_iStartDrawPosX;
	}
	setVisible(true);
	engine->notifyObjectsAboutMouse(this);
}

SpecialAbilityDeployer::~SpecialAbilityDeployer()
{
}


// Tell the framework how to draw this object
void SpecialAbilityDeployer::virtDraw()
{
	if (!isVisible()) {
		return;
	}

	unsigned int textColor = 0xff * (0x010101 - color); 

	// m_iStartDrawPos is the offest (negative radius / 2)
	int topLeftX = m_iCurrentScreenX + m_iStartDrawPosX;
	int topLeftY = m_iCurrentScreenY + m_iStartDrawPosY;
	getEngine()->drawForegroundOval(topLeftX, topLeftY , topLeftX + m_iDrawWidth, topLeftY + m_iDrawHeight, color);

	getEngine()->drawForegroundString(m_iCurrentScreenX + textOffset, m_iCurrentScreenY + textOffset, text, textColor);
}


void SpecialAbilityDeployer::virtDoUpdate(int currentTime) {
	if (getEngine()->isPaused() || !isVisible()) {
		return;
	}

	// change position if player presses a key
	if (getEngine()->isKeyPressed(SDLK_LEFT)) {
		m_iCurrentScreenX -= speed;
		if (!isInBorders()) {
			m_iCurrentScreenX += speed;
		}
	}
	if (getEngine()->isKeyPressed(SDLK_RIGHT)) {
		m_iCurrentScreenX += speed;
		if (!isInBorders()) {
			m_iCurrentScreenX -= speed;
		}
	}

	redrawDisplay();
}

/* Make it so when the circle is clicked */
