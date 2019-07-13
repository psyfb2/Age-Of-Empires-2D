#pragma once

// For these purposes a sprite is assumed to be an object which can move itself and shows an image

#include "header.h"
#include "DisplayableObject.h"
#include "ImageManager.h"
#include "BaseEngine.h"

//
// This class is a basic DisplayableObject which draws an image to the screen instead of just a square
// See: 
// Constructor: initialises the SimpleImage so you can use it
// Draw: asks the SimpleImage to draw itself at the position of the object on the screen
//
class ImageObject : public DisplayableObject
{
public:
	// Constructors
	ImageObject(BaseEngine* pEngine, std::string strURL, bool useTopLeftFor00 = true, bool bVisible = true)
		: DisplayableObject(pEngine, 100, 100, useTopLeftFor00),
		image(pEngine->loadImage(strURL, false/*don't keep it loaded*/) )
	{
		this->m_iDrawWidth = image.getWidth();
		this->m_iDrawHeight = image.getHeight();
		// Offset within the drawing area to draw at - need to reset since width and height changed:
		m_iStartDrawPosX = useTopLeftFor00 ? 0 : -m_iDrawWidth / 2;
		m_iStartDrawPosY = useTopLeftFor00 ? 0 : -m_iDrawHeight / 2;
		setVisible(true);
	}

	ImageObject(int xStart, int yStart, BaseEngine* pEngine, std::string strURL, bool useTopLeftFor00 = true, bool bVisible = true)
		: DisplayableObject(xStart, yStart, pEngine, 100, 100, useTopLeftFor00),
		image(ImageManager::get()->getImagebyURL(strURL, true/*Load*/, false/*Keep it loaded*/))
	{
		m_iDrawWidth = image.getWidth();
		m_iDrawHeight = image.getHeight();
		// Offset within the drawing area to draw at - need to reset since width and height changed:
		m_iStartDrawPosX = useTopLeftFor00 ? 0 : -m_iDrawWidth / 2;
		m_iStartDrawPosY = useTopLeftFor00 ? 0 : -m_iDrawHeight / 2;
		setVisible(bVisible);
	}

	// Draw the object - override to implement the actual drawing of the correct object
	virtual void virtDraw() override
	{
		if (isVisible())
		{
			image.renderImage(getEngine()->getForegroundSurface(), 0, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight);
		}
	}

private:
	SimpleImage image;
};

