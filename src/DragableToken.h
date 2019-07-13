#pragma once

#include "header.h"

#include "DisplayableObject.h"
#include "BaseEngine.h"
#include "DragableObject.h"
#include "FontManager.h"

// This class represents a token which can appear on a screen
// A token can have the screen or a card as its parent
// If it is on the screen then it is added as a DisplayableObject for the BaseEngine
// If it is on a card then it is added as a DisplayableObject on the Card
class DragableToken : public DisplayableObject
{
public:
	// Constructors
	DragableToken(BaseEngine* pEngine, int x, int y, int colour, bool bVisible = true)
		: DisplayableObject(x, y, pEngine, 25, 25, true), m_bIsBeingDragged(false), m_iTokenCount(0)
	{
		pEngine->notifyObjectsAboutMouse(true); // Tell BaseEngine to send us the mouse events because we use them
		setDefaultColour(colour),
		setVisible(bVisible);
	}

	// Create a new object which is a clone of this one. Uses the Java approach to avoid issues of assignment operator
	DisplayableObject* cloneObject() { return new DragableToken(*this); }

	/* Override if necessary - Mouse event - mouse button pressed. Must call notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called */
	virtual void virtMouseDown(int iButton, int iX, int iY) override
	{
/*
		if (virtIsPositionWithinObject(iX, iY))
		{
			// Normal mouse down can change the token if a key is pressed
			if (getEngine()->isKeyPressed('o'))
				m_iTokenCount--;
			else if (getEngine()->isKeyPressed('p'))
				m_iTokenCount++;
			redrawDisplay();
		}
*/
	}

	// Start the draw - only get this if mouse is on the actual display rather than a card at the time
	// IMPORTANT: If token is being shown on top of a token container, it will instead get a notify function call, from the container, because this token is not then actually in the BaseEngine objects list.
	// This means that things like handling + and - for token count need to be considered in there too!
	virtual bool virtSingleItemMouseDown(int iButton, int iX, int iY) override
	{
		//std::cout << "Mouse down " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		if (virtIsPositionWithinObject(iX, iY))
		{
			virtRedrawRectangle();

			// Since this was the object which was clicked, move it to the top of the containers now
			getEngine()->moveToLast(this);

			//printf("Drag started...");
			m_bIsBeingDragged = true;
			m_iInitialClickedDragX = iX;
			m_iInitialClickedDragY = iY;
			m_iStartOfDragX = m_iCurrentScreenX;
			m_iStartOfDragY = m_iCurrentScreenY;

			handleTokenWasClicked();

			virtRedrawRectangle();

			return true; // We grabbed it
		}
		return false; // We didn't get it
	}

	// Called then it was clicked by BaseEngine's function call (virtSingleItemMouseDown) or when container told it it was clicked (via notify!)
	void handleTokenWasClicked()
	{
		// Normal mouse down can change the token if a key is pressed
		if (getEngine()->isKeyPressed('o'))
			m_iTokenCount--;
		else if (getEngine()->isKeyPressed('p'))
			m_iTokenCount++;
	}


	/* Override if necessary - Mouse event - mouse was moved. Must call notifyObjectsAboutMouse to tell base engine to notify objects (see constructor), or this method will not be called */
	virtual void virtMouseMoved(int iX, int iY) override
	{ 
		//std::cout << "Mouse move " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		if (m_bIsBeingDragged)
		{
			virtRedrawRectangle();
			m_iCurrentScreenX = m_iStartOfDragX + iX - m_iInitialClickedDragX;
			m_iCurrentScreenY = m_iStartOfDragY + iY - m_iInitialClickedDragY;
			virtRedrawRectangle();
			//redrawDisplay(); // It moved so do a redraw!
		}
	}

	/* Override if necessary - Mouse event - mouse button released. Must call notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called */
	virtual void virtMouseUp(int iButton, int iX, int iY) override
	{
		if (m_bIsBeingDragged)
		{
			virtMouseMoved(iX, iY); // Ensure it moves to the position before drag ends
			//std::cout << "Mouse up " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
			m_bIsBeingDragged = false;
			// Check for token appearing over a token container
			DisplayableObject* pObject = getEngine()->getTopObjectForNotifyValue(NOTIFY_FIND_TOKEN_CONTAINER_FOR_POSITION, NOTIFY_YES, iX, iY);
			if (pObject != nullptr)
			{ // We found a container which we should be given to
				DragableTokenContainer* pContainer = dynamic_cast<DragableTokenContainer*>(pObject);
				if (pContainer != nullptr)
				{
					//std::cout << "Token ended up on container - add to container now..." << std::endl;
					getEngine()->removeDisplayableObject(this);
					pContainer->appendObjectToArray(this);
					if (getDrawingRegionLeft() < pContainer->getDrawingRegionLeft())
						m_iCurrentScreenX += pContainer->getDrawingRegionLeft() - getDrawingRegionLeft();
					if (getDrawingRegionRight() > pContainer->getDrawingRegionRight())
						m_iCurrentScreenX -= getDrawingRegionRight() - pContainer->getDrawingRegionRight();
					if (getDrawingRegionTop() < pContainer->getDrawingRegionTop())
						m_iCurrentScreenY += pContainer->getDrawingRegionTop() - getDrawingRegionTop();
					if (getDrawingRegionBottom() > pContainer->getDrawingRegionBottom())
						m_iCurrentScreenY -= getDrawingRegionBottom() - pContainer->getDrawingRegionBottom();

					virtRedrawRectangle();
				}
			}
		}
	}

	/* Draw token as sphere. If there is a value > 0 then show that as well. */
	void virtDraw() override
	{
		// Don't draw if not visible
		if (!isVisible())
			return;

		// If not redrawing all screen, and this is not in the redraw region then skip it
		if (!m_pEngine->getRedrawAllScreen() && !virtIsObjectInRedrawArea())
			return;

		int iRadiusSquared = (m_iDrawWidth / 2) * (m_iDrawWidth / 2);
		int iCentreX = m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth / 2;
		int iCentreY = m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight / 2;
		for (int iX = m_iCurrentScreenX + m_iStartDrawPosX; iX < (m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth); iX++)
			for (int iY = m_iCurrentScreenY + m_iStartDrawPosY; iY < (m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight); iY++)
				if (((iX - iCentreX)*(iX - iCentreX) + (iY - iCentreY)*(iY - iCentreY)) <= iRadiusSquared)
				{
					if (m_bIsBeingDragged && ( ((iX - iCentreX)*(iX - iCentreX) + (iY - iCentreY)*(iY - iCentreY)) <= (iRadiusSquared/4) ) )
					{
						getEngine()->setForegroundPixel(iX, iY, 0x808080);
					}
					else
					{
						// 0xB0 is the range of values, 0xff is the brightest value.
						unsigned int uiColour = (0xB0 * ((iX - iCentreX)*(iX - iCentreX) + (iY - iCentreY)*(iY - iCentreY))) / iRadiusSquared;
						uiColour = 0xff - uiColour;
						getEngine()->setForegroundPixel(iX, iY, m_iColour * uiColour);
					}
				}
		if (m_iTokenCount != 0)
			getEngine()->drawForegroundString( m_iCurrentScreenX + m_iStartDrawPosX + 5, m_iCurrentScreenY + m_iStartDrawPosY + 5,
						std::to_string(m_iTokenCount).c_str(),0, getEngine()->getFont("Cornerstone Regular.ttf", 15) );
	}

	// Handle a notification by a caller. The SignalNumber will have an application-specific meaning, i.e. you can use whatever you wish. See the NotifyAllObjects... functions in BaseEngine.
	virtual long virtNotify(int iSignalNumber, int x, int y) override
	{
		switch (iSignalNumber)
		{
		case NOTIFY_TOKEN_CHECK_MOUSEDOWN:	// Ask components whether click is inside it
			if (virtIsPositionWithinObject(x, y))
			{
				handleTokenWasClicked();
				return NOTIFY_YES;
			}
			else
				return NOTIFY_NO; // Not me!
			break;
		case NOTIFY_TOKEN_START_DRAG:	// Used for tokens - tell them that the mouse down has happened
			//printf("Drag started - notification came from container we were in...");
			m_bIsBeingDragged = true;
			m_iInitialClickedDragX = x;
			m_iInitialClickedDragY = y;
			m_iStartOfDragX = m_iCurrentScreenX;
			m_iStartOfDragY = m_iCurrentScreenY;
			return NOTIFY_YES; // We grabbed it
		case NOTIFY_TOKEN_CONTAINER_MOVED:	// Used to tell a token that the card it was on has moved
			m_iCurrentScreenX += x;
			m_iCurrentScreenY += y;
			return NOTIFY_YES; // We grabbed it
		}
		return NOTIFY_NO;
	}

	// Objects which are not destroyable, e.g. one-offs, should return false
	virtual bool destroyable() { return true; }

private:
	bool m_bIsBeingDragged;
	int m_iStartOfDragX, m_iStartOfDragY, m_iInitialClickedDragX, m_iInitialClickedDragY;

	int m_iTokenCount; // Count to show on the token
};

