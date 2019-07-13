#pragma once
#pragma once

#include "BaseEngine.h"

#include "SimpleDemo.h"

#include "ImageManager.h"
#include "TileManager.h"
#include "DisplayableObject.h"
#include "BouncingObject.h"

/*
This draws and moves an image on the screen.
Look particularly at the lines labelled "// Added to support image!"
*/
class BouncingImageObject : public BouncingObject
{
public:
	// Constructor has to set up all of the position and size members
	BouncingImageObject(BaseEngine* pEngine, std::string strURL)
		: BouncingObject(pEngine)
		, image(pEngine->loadImage(strURL, false/*don't keep it loaded*/)) // Added to support image!
	{
		// The following line is entirely optional!
		image = image.shrinkBy(4); // This will divide the image size by 4.
		// Note: it actually creates a new SimpleImage of smaller size, and then copies details to your old SimpleImage object

		// Nothing to do since base class will do it all!
		// Note that the width and height are hard-coded in the base class - but you can fix that when you create your own class.
	}

	// Draw the shape - just draws a rectangle
	virtual void virtDraw() override
	{
		if (isVisible())
		{
			image.renderImageWithMask(getEngine()->getForegroundSurface(), 0, 0, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iDrawWidth, m_iDrawHeight);
		}
	}

private:
	SimpleImage image; // Added to support image!
};
