#pragma once

#include "DisplayableObject.h"
#include "BaseEngine.h"

class DragableObject : public DisplayableObject
{
public:
	// Constructors
	DragableObject(BaseEngine* pEngine, int iWidth = 0, int iHeight = 0, bool useTopLeftFor00 = true, bool bVisible = true )
		: DisplayableObject(pEngine, iWidth, iHeight, useTopLeftFor00), m_bIsBeingDragged(false)
	{
		pEngine->notifyObjectsAboutMouse(true); // Tell BaseEngine to send us the mouse events because we use them
		setVisible(bVisible);
	}

	DragableObject(int xStart, int yStart, BaseEngine* pEngine, int iWidth, int iHeight, bool useTopLeftFor00 = true, bool bVisible = true)
		: DisplayableObject(xStart, yStart, pEngine, iWidth, iHeight, useTopLeftFor00), m_bIsBeingDragged(false)
	{
		pEngine->notifyObjectsAboutMouse(true); // Tell BaseEngine to send us the mouse events because we use them
		setVisible(bVisible);
	}

	// Create a new object which is a clone of this one. Uses the Java approach to avoid issues of assignment operator
	DisplayableObject* cloneObject() { return cloneDragable(); }

	// Create a new object which is a clone of this one. Uses the Java approach to avoid issues of assignment operator
	DragableObject* cloneDragable() { return new DragableObject(*this); }

	/* Override if necessary - Mouse event - mouse button pressed. Must call notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called */
	virtual void virtMouseDown(int iButton, int iX, int iY)
	{
		//std::cout << "Mouse down " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		if (virtIsPositionWithinObject(iX, iY))
		{
			//printf("Drag started...");
			m_bIsBeingDragged = true;
			m_iInitialClickedDragX = iX;
			m_iInitialClickedDragY = iY;
			m_iStartOfDragX = m_iCurrentScreenX;
			m_iStartOfDragY = m_iCurrentScreenY;
		}
	}

	/* Override if necessary - Mouse event - mouse was moved. Must call notifyObjectsAboutMouse to tell base engine to notify objects (see constructor), or this method will not be called */
	virtual void virtMouseMoved(int iX, int iY)
	{
		//std::cout << "Mouse move " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		if (m_bIsBeingDragged)
		{
			m_iCurrentScreenX = m_iStartOfDragX + iX - m_iInitialClickedDragX;
			m_iCurrentScreenY = m_iStartOfDragY + iY - m_iInitialClickedDragY;
			redrawDisplay(); // It moved so do a redraw!
		}
	}

	/* Override if necessary - Mouse event - mouse button released. Must call notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called */
	virtual void virtMouseUp(int iButton, int iX, int iY)
	{
		//std::cout << "Mouse up " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		m_bIsBeingDragged = false;
	}

private:
	bool m_bIsBeingDragged;
	int m_iStartOfDragX, m_iStartOfDragY, m_iInitialClickedDragX, m_iInitialClickedDragY;
};
