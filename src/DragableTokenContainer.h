#pragma once

#include "DisplayableObject.h"
#include "BaseEngine.h"
#include "Notify.h"

/*
This is a card which can be dragged
*/
class DragableTokenContainer : public DisplayableObject, public DisplayableObjectContainer
{
public:
	// Constructors
	DragableTokenContainer(BaseEngine* pEngine, int iWidth = 0, int iHeight = 0, bool useTopLeftFor00 = true, bool bVisible = true)
		: DisplayableObject(pEngine, iWidth, iHeight, useTopLeftFor00), m_bIsBeingDragged(false)
	{
		pEngine->notifyObjectsAboutMouse(true); // Tell BaseEngine to send us the mouse events because we use them
		setVisible(bVisible);
		pEngine->notifyTopObjectAboutMouseClick(true); // So that we get the virtSingleItemMouseDown() notification when we are the top object
	}

	DragableTokenContainer(int xStart, int yStart, BaseEngine* pEngine, int iWidth, int iHeight, bool useTopLeftFor00 = true, bool bVisible = true)
		: DisplayableObject(xStart, yStart, pEngine, iWidth, iHeight, useTopLeftFor00), m_bIsBeingDragged(false)
	{
		pEngine->notifyObjectsAboutMouse(true); // Tell BaseEngine to send us the mouse events because we use them
		setVisible(bVisible);
		pEngine->notifyTopObjectAboutMouseClick(true); // So that we get the virtSingleItemMouseDown() notification when we are the top object
	}

	// Create a new object which is a clone of this one. Uses the Java approach to avoid issues of assignment operator
	DisplayableObject* cloneObject() 
	{ 
		DragableTokenContainer* pClone = new DragableTokenContainer(*this);
		pClone->clearContents(); // Don't copy the contents of array - give it an empty one
	}

	// Called to draw the tokens which are contained.
	void drawTokensOnItem()
	{
		for (auto it = m_vecDisplayableObjects.begin(); it != m_vecDisplayableObjects.end(); it++)
			if (*it)
				(*it)->virtDraw();

	}

	// Bring this item to the front - virtual so subclass can turn off the facility
	virtual void bringToFront()
	{
		getEngine()->moveToLast(this);
	}


	// Draw the object - override to implement the actual drawing of the correct object
	virtual void virtDraw()
	{
		// Don't draw if not visible
		if (!isVisible())
			return;

		// If not redrawing all screen, and this is not in the redraw region then skip it
		if (!m_pEngine->getRedrawAllScreen() && !virtIsObjectInRedrawArea())
			return;

		// Ensure that this entire draw region is redrawn - so its tokens appear too.
		virtRedrawRectangle();

		// If not redrawing all screen, and this is not in the redraw region then skip it
		if (!m_pEngine->getRedrawAllScreen() && !virtIsObjectInRedrawArea())
			return;

		// Draw the object - using the default colour
		// Use an image?
		for (int iX = m_iCurrentScreenX + m_iStartDrawPosX; iX < (m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth); iX++)
			for (int iY = m_iCurrentScreenY + m_iStartDrawPosY; iY < (m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight); iY++)
				m_pEngine->setForegroundPixel(iX, iY, 0xffff00);

		getEngine()->drawForegroundString(m_iCurrentScreenX + m_iStartDrawPosX + 5, m_iCurrentScreenY + m_iStartDrawPosY + 5,
			"Token", 0, getEngine()->getFont("Cornerstone Regular.ttf", 24));
		getEngine()->drawForegroundString(m_iCurrentScreenX + m_iStartDrawPosX + 5, m_iCurrentScreenY + m_iStartDrawPosY + 35,
			"Cntnr", 0, getEngine()->getFont("Cornerstone Regular.ttf", 24));

		// Now draw all tokens which are contained in the card
		drawTokensOnItem();
	}

	// Check for a click being in the place clicked - return true if token grabs the click
	bool checkForAndHandleTokenClick(int iX, int iY)
	{
		DisplayableObject* pTop = getTopObjectForNotifyValue(NOTIFY_TOKEN_CHECK_MOUSEDOWN, NOTIFY_YES, iX, iY);
		if (pTop != nullptr)
		{ // Token is being dragged
			//std::cout << "DragableTokenContainer: Removing token from card and adding to engine." << std::endl;
			removeDisplayableObject(pTop); // Remove from card
			getEngine()->appendObjectToArray(pTop); // Add back to the main display
			//std::cout << "Notify top object of begin drag " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
			pTop->virtNotify(NOTIFY_TOKEN_START_DRAG, iX, iY); // Tell it that it is being dragged now
			return true;
		}
		return false;
	}

	virtual bool virtSingleItemMouseDown(int iButton, int iX, int iY) override
	{
		//std::cout << "virtSingleItemMouseDown " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		if (virtIsPositionWithinObject(iX, iY))
		{
			// Since this was the container which was clicked, move it to the top of the containers now
			bringToFront();

			// Check whether we are actually clicking on a token
			if (checkForAndHandleTokenClick(iX, iY))
			{
				return true;
			}
			else
			{
				// if not then drag this item 
				startDrag( iX, iY);
				return true;
			}
		}
		return false;
	}

	// Start a drag operation
	void startDrag(int iX, int iY)
	{
		//std::cout << "Drag started on container..." << std::endl;
		m_bIsBeingDragged = true;
		m_iInitialClickedDragX = iX;
		m_iInitialClickedDragY = iY;
		m_iStartOfDragX = m_iCurrentScreenX;
		m_iStartOfDragY = m_iCurrentScreenY;
		virtRedrawRectangle();
	}


	/* Override if necessary - Mouse event - mouse button pressed. Must call notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called */
	virtual void virtMouseDown(int iButton, int iX, int iY)
	{
		if (virtIsPositionWithinObject(iX, iY))
		{	// We need to pass on the mouse down event to all of the tokens on us - in case they have a special handler
			std::for_each(m_vecDisplayableObjects.begin(), m_vecDisplayableObjects.end(), [=](DisplayableObject* pdo) 
			{ if (pdo == nullptr) return; pdo->virtMouseDown(iButton, iX, iY); });
		}
	}

	/* Override if necessary - Mouse event - mouse was moved. Must call notifyObjectsAboutMouse to tell base engine to notify objects (see constructor), or this method will not be called */
	virtual void virtMouseMoved(int iX, int iY)
	{
		//std::cout << "Mouse move " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		if (m_bIsBeingDragged)
		{
			virtRedrawRectangle();
			int iOldX = m_iCurrentScreenX;
			int iOldY = m_iCurrentScreenY;
			m_iCurrentScreenX = m_iStartOfDragX + iX - m_iInitialClickedDragX;
			m_iCurrentScreenY = m_iStartOfDragY + iY - m_iInitialClickedDragY;

			// Drag all tokens as well
			notifyAllObjects(NOTIFY_TOKEN_CONTAINER_MOVED, m_iCurrentScreenX - iOldX, m_iCurrentScreenY - iOldY);
			
			virtRedrawRectangle(); // Redraw just the bit we need to draw
			//redrawDisplay(); // It moved so do a redraw!
		}
	}

	/* Override if necessary - Mouse event - mouse button released. Must call notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called */
	virtual void virtMouseUp(int iButton, int iX, int iY)
	{
		if (m_bIsBeingDragged)
		{
			//std::cout << "Mouse up " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
			m_bIsBeingDragged = false;
		}
	}

	// Handle a notification by a caller. The SignalNumber will have an application-specific meaning, i.e. you can use whatever you wish. See the NotifyAllObjects... functions in BaseEngine.
	virtual long virtNotify(int iSignalNumber, int x, int y) override
	{
		switch (iSignalNumber)
		{
		case NOTIFY_FIND_TOKEN_CONTAINER_FOR_POSITION:	// Ask whether this container is at a drop position for a token
			if (virtIsPositionWithinObject(x, y))
			{
				//std::cout << "Token release point is within container... " << std::to_string(x) << " " << std::to_string(y) << std::endl;
				return NOTIFY_YES;
			}
			else
				return NOTIFY_NO;
			break;
		}
		return NOTIFY_NO;
	}


protected:
	bool m_bIsBeingDragged;
	int m_iStartOfDragX, m_iStartOfDragY, m_iInitialClickedDragX, m_iInitialClickedDragY;
};
