#include "header.h"
#include "MyObjectA.h"
#include "BaseEngine.h"



MyObjectA::MyObjectA(BaseEngine* pEngine)
	: DisplayableObject(100, 200, pEngine, 100, 200, true)
{
}


MyObjectA::~MyObjectA()
{
}


// tell framework how to draw this object
void MyObjectA::virtDraw()
{
	getEngine()->drawForegroundRectangle(
		m_iCurrentScreenX, m_iCurrentScreenY,
		m_iCurrentScreenX + m_iDrawWidth - 1,
		m_iCurrentScreenY + m_iDrawHeight - 1,
		0x00ff00);

}

// called at every frame
void MyObjectA::virtDoUpdate(int currentTime)
{
	// change position if player presses a key
	if (getEngine()->isKeyPressed(SDLK_UP)) {
		m_iCurrentScreenY -= 2;
	}
	if (getEngine()->isKeyPressed(SDLK_DOWN)) {
		m_iCurrentScreenY += 2;
	}
	if (getEngine()->isKeyPressed(SDLK_LEFT)) {
		m_iCurrentScreenX -= 2;
	}
	if (getEngine()->isKeyPressed(SDLK_RIGHT)) {
		m_iCurrentScreenX += 2;
	}

	redrawDisplay();
}
