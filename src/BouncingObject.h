#pragma once

#include "BaseEngine.h"

#include "SimpleDemo.h"

#include "ImageManager.h"

#include "TileManager.h"

#include "DisplayableObject.h"

/*
This draws and moves the simple rectangle on the screen.
The main feature of this class is that it maintains a speed in the x and y direction, as well as a double accuracy position.
Before drawing etc it sets the normal integer position variables to be the values of the doubles rounded off.
Note: The entirity of this class is defined inside the class definition here.
*/
class BouncingObject : public DisplayableObject
{
private:
	double m_dSX;
	double m_dSY;
	double m_dX;
	double m_dY;

public:
	// Constructor has to set up all of the position and size members
	BouncingObject(BaseEngine* pEngine)
		: DisplayableObject(pEngine, 100, 50, false)
	{
		// Just put it somewhere initially
		m_dX = m_iCurrentScreenX = m_iDrawWidth;
		m_dY = m_iCurrentScreenY = m_iDrawHeight;
		// Speed
		m_dSX = 1.5;
		m_dSY = 2.5;
		// And make it visible
		setVisible(true);
	}

	// Draw the shape - just draws a rectangle
	void virtDraw() override
	{
		getEngine()->drawForegroundRectangle(
			m_iCurrentScreenX - m_iDrawWidth / 2 + 1,
			m_iCurrentScreenY - m_iDrawHeight / 2 + 1,
			m_iCurrentScreenX + m_iDrawWidth / 2 - 1,
			m_iCurrentScreenY + m_iDrawHeight / 2 - 1,
			0xffff00);
	}

	// Called frequently, this should move the item
	// In this case we also accept cursor key presses to change the speed
	// Space will set the speed to zero
	void virtDoUpdate(int iCurrentTime)
	{
		if (getEngine()->isPaused())
			return; // Do nothing if paused

					// Change speed if player presses a key
		if (getEngine()->isKeyPressed(SDLK_UP))
			m_dSY -= 0.005;
		if (getEngine()->isKeyPressed(SDLK_DOWN))
			m_dSY += 0.005;
		if (getEngine()->isKeyPressed(SDLK_LEFT))
			m_dSX -= 0.005;
		if (getEngine()->isKeyPressed(SDLK_RIGHT))
			m_dSX += 0.005;

		// Alter position for speed
		m_dX += m_dSX;
		m_dY += m_dSY;

		// Check for bounce off the edge
		if ((m_dX + m_iStartDrawPosX) < 0)
		{
			m_dX = -m_iStartDrawPosX;
			if (m_dSX < 0)
				m_dSX = -m_dSX;
		}
		if ((m_dX + m_iStartDrawPosX + m_iDrawWidth) >(getEngine()->getWindowWidth() - 1))
		{
			m_dX = getEngine()->getWindowWidth() - 1 - m_iStartDrawPosX - m_iDrawWidth;
			if (m_dSX > 0)
				m_dSX = -m_dSX;
		}
		if ((m_dY + m_iStartDrawPosY) < 0)
		{
			m_dY = -m_iStartDrawPosY;
			if (m_dSY < 0)
				m_dSY = -m_dSY;
		}
		if ((m_dY + m_iStartDrawPosY + m_iDrawHeight) >(getEngine()->getWindowHeight() - 1))
		{
			m_dY = getEngine()->getWindowHeight() - 1 - m_iStartDrawPosY - m_iDrawHeight;
			if (m_dSY > 0)
				m_dSY = -m_dSY;
		}

		// Set current position - you NEED to set the current positions
		m_iCurrentScreenX = (int)(m_dX + 0.5);
		m_iCurrentScreenY = (int)(m_dY + 0.5);

		//printf("Position %f, %f\n", m_dX, m_dY );

		// Ensure that the object gets redrawn on the display, if something changed
		redrawDisplay();
	}
};
