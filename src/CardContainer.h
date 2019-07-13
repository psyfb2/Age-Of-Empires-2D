#pragma once

#include "templates.h"

#include "DragableTokenContainer.h"
#include "DragableToken.h"

//#include "CardOrTile.h"
#include "DisplayableObjectContainer.h"

class CardOrTile;

/*
This class can contain Cards/Tiles and optionally appears on the screen
Things which are added to the container will no longer appear on the screen.
This can easily be used as a deck of cards!

Notes:
Card container as implemented here has TWO decks of cards, one is the draw deck and one is the discard deck.
Press d to change to discard deck - then draw card, adding card, shuffle etc all apply to the discard deck.
Press s before clicking to do a shuffle rather than a draw.
Clicking on top and bottom regions allow dragging of the container to a new position on the table.
Clicking on middle of the container does the draw a card, shuffle, etc features.
*/
class CardContainer : public DragableTokenContainer
{
	std::string m_strLabel;
	// This container can have cards in it, but can also have tokens on it, so base class is used for the tokens and composition for the cards
	DisplayableObjectContainer m_cardContainer;
	// Pile of discarded cards
	DisplayableObjectContainer m_cardDiscardContainer;

	// This is used for setting a temporary drawing colour while clicked
	int m_iTempDrawColour;

public:
	CardContainer(
		BaseEngine* pEngine,
		int iX, int iY,
		std::string strLabel )
		: DragableTokenContainer(iX, iY, pEngine, 100, 75, true), m_strLabel(strLabel), m_iTempDrawColour(-1)
	{
	}

	// Create a new object which is a clone of this one. Uses the Java approach to avoid issues of assignment operator
	DisplayableObject* cloneObject() 
	{ 
		// Create new container with no contents in it
		CardContainer* pNew = new CardContainer(*this); 
		pNew->m_cardContainer.clearContents();
		pNew->m_cardDiscardContainer.clearContents();
		pNew->clearContents();
		return pNew;
	}

	// Mouse buttom was clicked on THIS object as top object, so we should handle it now...
	virtual bool virtSingleItemMouseDown(int iButton, int iX, int iY) override
	{
		//std::cout << "virtSingleItemMouseDown " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		if (virtIsPositionWithinObject(iX, iY))
		{
			// Since this was the container which was clicked, move it to the top of the containers now
			bringToFront();

			// We could have a token on us, at the click point, so ask the tokens to handle it first, in case they are on top...
			if (checkForAndHandleTokenClick(iX, iY))
			{
				// We do nothing because it was a click on the token, not this object
				return true;
			}
			else // Not on a token
			{
				// Clicked on the item
				int iLeft = getDrawingRegionLeft();
				int iTop = getDrawingRegionTop();

				if (getEngine()->isKeyPressed('s')) // Shuffle if anywhere clicked when in shuffle mode
				{
					if (getEngine()->isKeyPressed('d')) // Shuffle discard pile
					{
						m_cardDiscardContainer.shuffle();
						m_cardDiscardContainer.shuffle();
						m_cardDiscardContainer.shuffle();
					}
					else // Shuffle normal pile
					{
						m_cardContainer.shuffle();
						m_cardContainer.shuffle();
						m_cardContainer.shuffle();
					}
					m_iTempDrawColour = 0xff0000; // Force redraw in red temporarily. Note the reset in mouse up handler
					virtRedrawRectangle(); // Force refresh of this part of screen
				}
				else	// Draw only if on the draw bit of the display
				{
					if ((iY - iTop >= 22) && (iY - iTop <= 50)) // Gives a few pixels around because hard to get right when clicking
					{
						// If no special keys pressed, draw a new card
						{
							// Remove top card from container if it exists
							DisplayableObject* oCard = removeTopCardFromContainer(getEngine()->isKeyPressed('d'));
							if (oCard != nullptr)
							{
								// Set position so that it appears in same place as deck
								oCard->setPosition(this->getXCentre(), this->getYCentre());
								// Display card on main display...
								getEngine()->appendObjectToArray(oCard); // Add to main display
								m_iTempDrawColour = 0x800000; // Force redraw in red temporarily. Note the reset in mouse up handler
								virtRedrawRectangle(); // Force refresh of this part of screen
							}
						}
					}
					else // Start drag if not on the draw bit
					{
						// if not then drag this item 
						startDrag(iX, iY);
						return true;
					}
				}
			}
			return true;
		}
		return false;
	}

	/* Override if necessary - Mouse event - mouse button released. Must call notifyObjectsAboutMouse to tell base engine to notify objects, or this will not be called */
	virtual void virtMouseUp(int iButton, int iX, int iY)
	{
		// Clear the colour if it was set, otherwise just allow base class to do it's work...
		m_iTempDrawColour = -1;
		virtRedrawRectangle(); // Force refresh of this part of screen, because we changed something to do with the drawing of this item
		DragableTokenContainer::virtMouseUp( iButton, iX, iY);
	}

	void virtDraw() override
	{
		// Don't draw if not visible
		if (!isVisible())
			return;

		// If not redrawing all screen, and this is not in the redraw region then skip it
		if (!m_pEngine->getRedrawAllScreen() && !virtIsObjectInRedrawArea())
			return;

		getEngine()->drawForegroundRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY,
			m_iCurrentScreenX + m_iStartDrawPosX + 99, m_iCurrentScreenY + m_iStartDrawPosY + 24,
			0xB0B0B0);

		// Shuffle
		if (getEngine()->isKeyPressed('s'))
		{
			getEngine()->drawForegroundRectangle(
				m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY + 25,
				m_iCurrentScreenX + m_iStartDrawPosX + 99, m_iCurrentScreenY + m_iStartDrawPosY + 49,
				m_iTempDrawColour != -1 ? m_iTempDrawColour : 0x800080);
			getEngine()->drawForegroundString(m_iCurrentScreenX + m_iStartDrawPosX + 3, m_iCurrentScreenY + m_iStartDrawPosY + 26,
				"Shuffle", 0xffffff, getEngine()->getFont("Cornerstone Regular.ttf", 20));
		}
		else // Draw card
		{
			getEngine()->drawForegroundRectangle(
				m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY + 25,
				m_iCurrentScreenX + m_iStartDrawPosX + 99, m_iCurrentScreenY + m_iStartDrawPosY + 49,
				m_iTempDrawColour != -1 ? m_iTempDrawColour : 0x00ff00);
			getEngine()->drawForegroundString(m_iCurrentScreenX + m_iStartDrawPosX + 15, m_iCurrentScreenY + m_iStartDrawPosY + 26,
				"Draw", 0, getEngine()->getFont("Cornerstone Regular.ttf", 20));
		}

		getEngine()->drawForegroundRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY + 49,
			m_iCurrentScreenX + m_iStartDrawPosX + 99, m_iCurrentScreenY + m_iStartDrawPosY + 74,
			0xB0B0B0);


		getEngine()->drawForegroundString(m_iCurrentScreenX + m_iStartDrawPosX + 3, m_iCurrentScreenY + m_iStartDrawPosY + 1,
			m_strLabel.c_str(), 0xffffff, getEngine()->getFont("Cornerstone Regular.ttf", 16));


		if (getEngine()->isKeyPressed('d')) // Discard deck vs standard deck
			getEngine()->drawForegroundString(m_iCurrentScreenX + m_iStartDrawPosX + 2, m_iCurrentScreenY + m_iStartDrawPosY + 51,
				"DISCARD", 0xffffff, getEngine()->getFont("Cornerstone Regular.ttf", 18));
		else
			getEngine()->drawForegroundString(m_iCurrentScreenX + m_iStartDrawPosX + 20, m_iCurrentScreenY + m_iStartDrawPosY + 51,
				"DECK", 0xffffff, getEngine()->getFont("Cornerstone Regular.ttf", 18));

		getEngine()->drawForegroundLine(m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth-1, m_iCurrentScreenY + m_iStartDrawPosY, m_bIsBeingDragged ? 0x0000ff : 0xffffff);
		getEngine()->drawForegroundLine(m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY, m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight-1, m_bIsBeingDragged ? 0x0000ff : 0xffffff);
		getEngine()->drawForegroundLine(m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY+m_iDrawHeight-1, m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth-1, m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight - 1, m_bIsBeingDragged ? 0x0000ff : 0xffffff);
		getEngine()->drawForegroundLine(m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth - 1, m_iCurrentScreenY + m_iStartDrawPosY, m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth - 1, m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight - 1, m_bIsBeingDragged ? 0x0000ff : 0xffffff);

		// Now draw all tokens which are contained in the card
		drawTokensOnItem();
	}

	// Handle a notification by a caller. The SignalNumber will have an application-specific meaning, i.e. you can use whatever you wish. See the NotifyAllObjects... functions in BaseEngine.
	virtual long virtNotify(int iSignalNumber, int x, int y) override
	{
		switch (iSignalNumber)
		{
		case NOTIFY_IS_CARD_DECK_AT_LOCATION:	// Ask whether this container is at a drop position for a token
			if (virtIsPositionWithinObject(x, y))
			{
				//std::cout << "Card release point is within card container... " << std::to_string(x) << " " << std::to_string(y) << std::endl;
				return NOTIFY_YES;
			}
			else
				return NOTIFY_NO;
			break;
		}
		// Handle other notifications, e.g. grabbing tokens
		return DragableTokenContainer::virtNotify( iSignalNumber, x, y);
	}





	// Add a card to the container
	bool addCardToContainer(CardOrTile* pCard, bool bDiscardPile = false)
	{
		if (bDiscardPile)
			m_cardDiscardContainer.appendObjectToArray((DisplayableObject*)pCard);
		else
			m_cardContainer.appendObjectToArray((DisplayableObject*)pCard);
		return true;
	}

	// Get the top card from the container
	DisplayableObject* getTopCardFromContainer(bool bDiscardPile = false)
	{
		DisplayableObject* pCard;
		if (bDiscardPile)
		{
			for (int i = m_cardDiscardContainer.getContentCount() - 1; i >= 0; i--)
			{
				pCard = m_cardDiscardContainer.getContentItem(i);
				if (pCard != nullptr)
					return pCard;
			}
		}
		else
		{
			for (int i = m_cardContainer.getContentCount() - 1; i >= 0; i--)
			{
				pCard = m_cardContainer.getContentItem(i);
				if (pCard != nullptr)
					return pCard;
			}
		}
		return nullptr; // No cards
	}


	// Get and remove the top card from the container
	DisplayableObject* removeTopCardFromContainer(bool bDiscardPile = false)
	{
		DisplayableObject* pCard = getTopCardFromContainer(bDiscardPile);
		if (bDiscardPile)
		{
			if (m_cardDiscardContainer.removeDisplayableObject(pCard))
				return pCard;
		}
		else
		{
			if (m_cardContainer.removeDisplayableObject(pCard))
				return pCard;
		}
		return nullptr; // No cards
	}

	// Get the bottom card from the container
	DisplayableObject* getBottomCardFromContainer(bool bDiscardPile = false)
	{
		DisplayableObject* pCard;
		if (bDiscardPile)
		{
			for (int i = 0; i < m_cardDiscardContainer.getContentCount(); i++)
			{
				pCard = m_cardDiscardContainer.getContentItem(i);
				if (pCard != nullptr)
					return pCard;
			}
		}
		else
		{
			for (int i = 0; i < m_cardContainer.getContentCount(); i++)
			{
				pCard = m_cardContainer.getContentItem(i);
				if (pCard != nullptr)
					return pCard;
			}
		}
		return nullptr; // No cards
	}


	// Get and remove the top card from the container
	DisplayableObject* removeBottomCardFromContainer(bool bDiscardPile = false)
	{
		DisplayableObject* pCard = getBottomCardFromContainer(bDiscardPile);
		if (bDiscardPile)
		{
			if (m_cardDiscardContainer.removeDisplayableObject(pCard))
				return pCard;
		}
		else
		{
			if (m_cardContainer.removeDisplayableObject(pCard))
				return pCard;
		}
		return nullptr; // No cards
	}
};
