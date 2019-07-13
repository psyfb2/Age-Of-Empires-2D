#pragma once

// For these purposes a sprite is assumed to be an object which can move itself and shows an image

#include "header.h"
#include "DisplayableObject.h"
#include "ImageManager.h"

class DragableImageObject : public DisplayableObject
{
public:
	// Constructors
	DragableImageObject(BaseEngine* pEngine, std::string strURL, bool useTopLeftFor00 = true, bool bVisible = true)
		: DisplayableObject(pEngine, 100, 100, useTopLeftFor00),
		image(ImageManager::get()->getImagebyURL(strURL, true/*Load*/, false/*don't keep it loaded*/)),
		// Note: the above is like: ImageManager::loadImage(strURL, false/*don't keep it loaded*/)
		m_bIsBeingDragged(false)
	{
		this->m_iDrawWidth = image.getWidth();
		this->m_iDrawHeight = image.getHeight();
		// Offset within the drawing area to draw at - need to reset since width and height changed:
		m_iStartDrawPosX = useTopLeftFor00 ? 0 : -m_iDrawWidth / 2;
		m_iStartDrawPosY = useTopLeftFor00 ? 0 : -m_iDrawHeight / 2;
		pEngine->notifyObjectsAboutMouse(true); // Tell BaseEngine to send us the mouse events because we use them
		setVisible(true);
	}

	DragableImageObject(int xStart, int yStart, BaseEngine* pEngine, std::string strURL, bool useTopLeftFor00 = true, bool bVisible = true)
		: DisplayableObject(xStart, yStart, pEngine, 100, 100, useTopLeftFor00),
		image(ImageManager::get()->getImagebyURL(strURL, true/*Load*/, false/*Keep it loaded*/)),
		m_bIsBeingDragged(false)
	{
		m_iDrawWidth = image.getWidth();
		m_iDrawHeight = image.getHeight();
		// Offset within the drawing area to draw at - need to reset since width and height changed:
		m_iStartDrawPosX = useTopLeftFor00 ? 0 : -m_iDrawWidth / 2;
		m_iStartDrawPosY = useTopLeftFor00 ? 0 : -m_iDrawHeight / 2;
		pEngine->notifyObjectsAboutMouse(true); // Tell BaseEngine to send us the mouse events because we use them
		setVisible(bVisible);
	}

	// Draw the object - override to implement the actual drawing of the correct object
	virtual void virtDraw()
	{
		if (isVisible())
		{
			image.renderImageWithMask(getEngine()->getForegroundSurface(), 0, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight);
		}
	}

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
	SimpleImage image;
};
