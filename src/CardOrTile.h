#pragma once

#include "header.h"
#include "DisplayableObject.h"
#include "DragableObject.h"
#include "DragableTokenContainer.h"
#include "CardContainer.h"
#include "BaseEngine.h"
#include "ImageManager.h"

#include <memory>

// A dragable token container which can contain image for front and back
// Cards can be dragged into card containers

class CardOrTile : public DragableTokenContainer
{
	// Front and back image URLs
	std::string m_strImageFrontURL;
	int m_iFrontImageX;
	int m_iFrontImageY;
	int m_iFrontImageWidth;
	int m_iFrontImageHeight;

	std::string m_strImageBackURL;
	int m_iBackImageX;
	int m_iBackImageY;
	int m_iBackImageWidth;
	int m_iBackImageHeight;

	// Image renderer to use
	SimpleImage m_image;

	// true if currently showing front image
	bool m_bShowingFront; 
	
	// true if should be showing front image
	bool m_bShouldShowFront; 

	// Flag specifying that this is a tile - leave at the back and need M to move it
	bool m_bIsTile;

	int m_iCurrentScalingLevel;
	int m_iNormalScalingLevel;

	int m_iTransparencyX;
	int m_iTransparencyY;
	int m_iDefaultTransparencyColour; // Used if x and y are -1

public:
	// Constructor: card that can be flipped and moved easily
	CardOrTile(
			BaseEngine* pEngine, 
			int x, int y, 
			std::string strURLFront, int xf, int yf, int wf, int hf,
			std::string strURLBack, int xb, int yb, int wb, int hb,
			int iWidth, int iHeight, 
			int iScale = 4,
			bool useTopLeftFor00 = true, bool bVisible = true)
		: DragableTokenContainer(x, y, pEngine, iWidth/ iScale, iHeight/ iScale, useTopLeftFor00),
		m_strImageFrontURL(strURLFront),
		m_iFrontImageX(xf), m_iFrontImageY(yf), m_iFrontImageWidth(wf), m_iFrontImageHeight(hf),
		m_strImageBackURL(strURLBack),
		m_iBackImageX(xb), m_iBackImageY(yb), m_iBackImageWidth(wb), m_iBackImageHeight(hb),
		m_image(),
		m_bShowingFront(false),
		m_bShouldShowFront(true),
		m_bIsTile(false),
		m_iCurrentScalingLevel(iScale),
		m_iNormalScalingLevel(iScale)
	{
		//pEngine->notifyObjectsAboutMouse(true); // Tell BaseEngine to send us the mouse events because we use them
		setVisible(true);
	}

	// Constructor: tile - use M to move, and don't bring to front when moved
	CardOrTile(
		BaseEngine* pEngine,
		int x, int y,
		std::string strURLFront,
		int iScale = 1,
		bool useTopLeftFor00 = false, bool bVisible = true)
		: DragableTokenContainer(x, y, pEngine, 200/ iScale, 200/ iScale, useTopLeftFor00),
		m_strImageFrontURL(strURLFront),
		m_iFrontImageX(0), m_iFrontImageY(0), m_iFrontImageWidth(-1), m_iFrontImageHeight(-1),
		m_strImageBackURL(strURLFront),
		m_iBackImageX(0), m_iBackImageY(0), m_iBackImageWidth(-1), m_iBackImageHeight(-1),
		m_image(),
		m_bShowingFront(false),
		m_bShouldShowFront(true),
		m_bIsTile(true),
		m_iCurrentScalingLevel(iScale),
		m_iNormalScalingLevel(iScale),
		m_iTransparencyX(-1),
		m_iTransparencyY(-1),
		m_iDefaultTransparencyColour(-1)
	{
		setVisible(true);
	}

	// Specify which pixel gives the transparency colour. Specify -1 to not set any.
	void setTransparencyPixel(int x, int y)
	{
		m_iTransparencyX = x;
		m_iTransparencyY = y;
		updateTransparency();
	}

	// Set a default transparency colour if no pixel position has been specified. Use -1 for none.
	void setTransparencyPixel(int colour)
	{
		m_iDefaultTransparencyColour = colour;
	}

	// Tell the image what the transparency colour is
	void updateTransparency()
	{
		if (m_iTransparencyX > -1 && m_iTransparencyY > -1)
		{
			if (m_image.getRawImageData() != nullptr)
			{
				int transparencyColour = m_image.getRawImageData()->getRawPixelColour(m_iTransparencyX, m_iTransparencyY);
				m_iTransparencyColour = transparencyColour;
			}
			// Else don't change it because image not loaded anyway
		}
		else
		{
			m_iTransparencyColour = m_iDefaultTransparencyColour;
		}
	}

	// Bring this item to the front - virtual so subclass can turn off the facility
	virtual void bringToFront()
	{
		if ( !m_bIsTile ) // Only if not a tile - leaves tiles at the back!
			getEngine()->moveToLast(this);
	}


	// Load/prepare the front image
	void readyFrontImage()
	{
		if (!m_bShowingFront) // Need to switch to front
		{
			m_image = ImageManager::get()->getImagebyURL(m_strImageFrontURL, true, false);
			if (m_image.getRawImageData() == nullptr)
			{
				m_iWidthInImage = 200;
				m_iHeightInImage = 200;
				m_bShowingFront = true;
				return;
			}
			updateTransparency();
			if (m_iFrontImageWidth == -1)
				m_iFrontImageWidth = m_image.getRawImageData()->getWidth() - m_iFrontImageX;
			if (m_iFrontImageHeight == -1)
				m_iFrontImageHeight = m_image.getRawImageData()->getHeight() - m_iFrontImageY;
			// Store the current values
			m_iLeftInImage = m_iFrontImageX;
			m_iTopInImage = m_iFrontImageY;
			m_iWidthInImage = m_iFrontImageWidth;
			m_iHeightInImage = m_iFrontImageHeight;
			m_bShowingFront = true;

			// If it's a tile then force the width/height of this item to match the image height/width
			if (m_bIsTile) // Set drawing width and height to match the image
				this->setSize(m_iFrontImageWidth, m_iFrontImageHeight);
		}
	}

	// Load/prepare the back image
	void readyBackImage()
	{
		if (m_bShowingFront) // Need to switch to back
		{
			m_image = ImageManager::get()->getImagebyURL(m_strImageBackURL, true, false);
			if (m_image.getRawImageData() == nullptr)
			{
				m_iWidthInImage = 200;
				m_iHeightInImage = 200;
				m_bShowingFront = false;
				return;
			}
			updateTransparency();
			if (m_iBackImageWidth == -1)
				m_iBackImageWidth = m_image.getRawImageData()->getWidth() - m_iBackImageX;
			if (m_iBackImageHeight == -1)
				m_iBackImageHeight = m_image.getRawImageData()->getHeight() - m_iBackImageY;
			// Store the current values
			m_iLeftInImage = m_iBackImageX;
			m_iTopInImage = m_iBackImageY;
			m_iWidthInImage = m_iBackImageWidth;
			m_iHeightInImage = m_iBackImageHeight;
			m_bShowingFront = false;

			// If it's a tile then force the width/height of this item to match the image height/width
			if (m_bIsTile) // Set drawing width and height to match the image
				this->setSize(m_iBackImageWidth, m_iBackImageHeight);
		}
	}


	// Ensure that the correct image is ready for use
	void showCorrectImage()
	{
		if (m_bShouldShowFront)
			readyFrontImage();
		else
			readyBackImage();
	}


	// Draw the object - override to implement the actual drawing of the correct object
	virtual void virtDraw()
	{
		if (isVisible())
		{
			showCorrectImage();

			if (m_image.getRawImageData() == nullptr)
			{ // Not loaded
				// Draw the object - using the default colour
				for (int iX = m_iCurrentScreenX + m_iStartDrawPosX; iX < (m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth); iX++)
					for (int iY = m_iCurrentScreenY + m_iStartDrawPosY; iY < (m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawWidth); iY++)
						m_pEngine->setForegroundPixel(iX, iY, 0xff0000);
				getEngine()->drawForegroundString(m_iCurrentScreenX + m_iStartDrawPosX + 3, m_iCurrentScreenY + m_iStartDrawPosY + 26,
					"Load", 0xffffff, getEngine()->getFont("Cornerstone Regular.ttf", 20));
				getEngine()->drawForegroundString(m_iCurrentScreenX + m_iStartDrawPosX + 3, m_iCurrentScreenY + m_iStartDrawPosY + 66,
					"Failed", 0xffffff, getEngine()->getFont("Cornerstone Regular.ttf", 20));
			}
			else
			{	
				// Fix draw width and height now, so that partial redraw works correctly
				m_iDrawWidth = m_iWidthInImage / m_iCurrentScalingLevel;
				m_iDrawHeight = m_iHeightInImage / m_iCurrentScalingLevel;
				//std::cout << "Draw card or tile: isTile? " << std::to_string(m_bIsTile) << " width " << std::to_string(m_iDrawWidth) << ", height " << std::to_string(m_iDrawHeight) << std::endl;
				// Draw the image
				m_image.simpleRender(getEngine(), getEngine()->getForegroundSurface(), m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, 
					m_iLeftInImage, m_iTopInImage, m_iWidthInImage, m_iHeightInImage, m_iCurrentScalingLevel, m_iTransparencyColour);
				//if (m_bIsTile)
				//	m_imageRenderer.simpleRender(getEngine(),getEngine()->getForegroundSurface(), m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iCurrentScalingLevel);
				//else
				//	m_imageRenderer.render(getEngine()->getForegroundSurface(), m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY);
			}

			// After drawing the images etc, draw the tokens
			drawTokensOnItem();

		}
		else
		{
			//std::cout << "Card is invisible!" << std::endl;
		}
	}

	virtual bool virtSingleItemMouseDown(int iButton, int iX, int iY) override
	{
		//std::cout << "virtSingleItemMouseDown " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;

		if (virtIsPositionWithinObject(iX, iY))
		{
			// Since this was the container which was clicked, move it to the top of the containers now
			bringToFront();

			if (iButton == 3) // Right mouse button works even if clicking on a token, so check it first
			{
				if (m_iCurrentScalingLevel == m_iNormalScalingLevel)
					m_iCurrentScalingLevel = 1;
				else
					m_iCurrentScalingLevel = m_iNormalScalingLevel;
				int m_iNormalScalingLevel;
				return true; // We accepted the mouse button
			}

			// If f is pressed and this is a card then flip it, even if you click on a token
			if (!m_bIsTile && getEngine()->isKeyPressed('f'))
			{
				//std::cout << "*** flip card now" << std::endl;
				m_bShouldShowFront = !m_bShouldShowFront;
				showCorrectImage();
				redrawDisplay();
			}

			// Check for token. If not then probably dragging...
			if (checkForAndHandleTokenClick(iX, iY))
			{
				return true;
			}
			else
			{
				// If M is pressed OR this is a card then start the drag
				if ( !m_bIsTile || getEngine()->isKeyPressed('m') )
				{
					// if not then drag this item 
					startDrag(iX, iY);
					return true;
				}

				this->virtRedrawRectangle();
			}
		}

		return false;
	}


	/* Override if necessary - Mouse event - mouse button released. Must call notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called */
	virtual void virtMouseUp(int iButton, int iX, int iY)
	{
		if (m_bIsBeingDragged)
		{
			virtMouseMoved(iX, iY); // Ensure it moves to the position before drag ends
			//std::cout << "Mouse up " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
			m_bIsBeingDragged = false;
			// Check for card appearing over a card container
			DisplayableObject* pObject = getEngine()->getTopObjectForNotifyValue(NOTIFY_IS_CARD_DECK_AT_LOCATION, NOTIFY_YES, iX, iY);
			if (pObject != nullptr)
			{ // We found a container which we should be given to
				CardContainer* pContainer = dynamic_cast<CardContainer*>(pObject);
				if (pContainer != nullptr)
				{
					//std::cout << "Card ended up on container - add to container now..." << std::endl;
					getEngine()->removeDisplayableObject(this);
					pContainer->addCardToContainer(this, getEngine()->isKeyPressed('d'));
				}
			}
			this->virtRedrawRectangle();
		}
	}

protected:
	int m_iTopInImage;
	int m_iLeftInImage;
	int m_iWidthInImage;
	int m_iHeightInImage;
	int m_iTransparencyColour;
};
