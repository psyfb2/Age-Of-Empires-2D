#pragma once

#include "templates.h"

#include "DragableTokenContainer.h"
#include "DragableToken.h"

class TokenTray :
	public DragableTokenContainer
{
public:
	TokenTray(
		BaseEngine* pEngine,
		int iX, int iY)
		: DragableTokenContainer(iX, iY, pEngine, 100, 75, true )
	{
	}

	~TokenTray(void)
	{
	}

	virtual bool virtSingleItemMouseDown(int iButton, int iX, int iY) override
	{
		//std::cout << "virtSingleItemMouseDown " << std::to_string(iX) << " " << std::to_string(iY) << std::endl;
		if (virtIsPositionWithinObject(iX, iY))
		{
			// Since this was the container which was clicked, move it to the top of the containers now
			bringToFront();

			if (checkForAndHandleTokenClick(iX, iY))
			{
				return true;
			}
			else
			{
				int iLeft = m_iCurrentScreenX + m_iStartDrawPosX;
				int iTop = m_iCurrentScreenY + m_iStartDrawPosY;

				if (iY - iTop < 25)
				{
					DragableTokenContainer::virtSingleItemMouseDown(iButton, iX, iY);
				}
				else if (iY - iTop < 50)
				{
					if (iX - iLeft < 25)
						createToken(0x010101, iX, m_iCurrentScreenY + m_iStartDrawPosY - 25);
					else if (iX - iLeft < 50)
						createToken(0x010000, iX, m_iCurrentScreenY + m_iStartDrawPosY - 25);
					else if (iX - iLeft < 75)
						createToken(0x000100, iX, m_iCurrentScreenY + m_iStartDrawPosY - 25);
					else
						createToken(0x000001, iX, m_iCurrentScreenY + m_iStartDrawPosY - 25);
				}
				else if (iY - iTop < 75)
				{
					if (iX - iLeft < 25)
						createToken(0x000101, iX, m_iCurrentScreenY + m_iStartDrawPosY - 25);
					else if (iX - iLeft < 50)
						createToken(0x010001, iX, m_iCurrentScreenY + m_iStartDrawPosY - 25);
					else if (iX - iLeft < 75)
						createToken(0x010100, iX, m_iCurrentScreenY + m_iStartDrawPosY - 25);
					else
					{
						// This is a special identified which indicates to delete all tokens on this container at the moment
						for (int i = m_vecDisplayableObjects.size() - 1; i > 0; i--)
						{
							if (m_vecDisplayableObjects[i])
							{
								delete m_vecDisplayableObjects[i];
								m_vecDisplayableObjects[i] = nullptr;
							}
						}
						m_vecDisplayableObjects.clear();
					}
				}
				return true;
			}
		}
		return false;
	}

	void virtDraw() override
	{
		// Don't draw if not visible
		if (!isVisible())
			return;

		// If not redrawing all screen, and this is not in the redraw region then skip it
		if (!m_pEngine->getRedrawAllScreen() && !virtIsObjectInRedrawArea())
			return;

		// Ensure that this entire draw region is redrawn - so its tokens appear too.
		virtRedrawRectangle();

		getEngine()->drawForegroundRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY,
			m_iCurrentScreenX + m_iStartDrawPosX + 99, m_iCurrentScreenY + m_iStartDrawPosY + 24,
			0x808080);
		getEngine()->drawForegroundRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY+25,
			m_iCurrentScreenX + m_iStartDrawPosX+24, m_iCurrentScreenY + m_iStartDrawPosY + 49,
			0xffffff);
		getEngine()->drawForegroundRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX+25, m_iCurrentScreenY + m_iStartDrawPosY + 25,
			m_iCurrentScreenX + m_iStartDrawPosX+49, m_iCurrentScreenY + m_iStartDrawPosY + 49,
			0xff0000);
		getEngine()->drawForegroundRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX+50, m_iCurrentScreenY + m_iStartDrawPosY + 25,
			m_iCurrentScreenX + m_iStartDrawPosX+74, m_iCurrentScreenY + m_iStartDrawPosY + 49,
			0x00ff00);
		getEngine()->drawForegroundRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX+75, m_iCurrentScreenY + m_iStartDrawPosY + 25,
			m_iCurrentScreenX + m_iStartDrawPosX+99, m_iCurrentScreenY + m_iStartDrawPosY + 49,
			0x0000ff);
		getEngine()->drawForegroundRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY + 50,
			m_iCurrentScreenX + m_iStartDrawPosX + 24, m_iCurrentScreenY + m_iStartDrawPosY + 74,
			0x00ffff);
		getEngine()->drawForegroundRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX + 25, m_iCurrentScreenY + m_iStartDrawPosY + 50,
			m_iCurrentScreenX + m_iStartDrawPosX + 49, m_iCurrentScreenY + m_iStartDrawPosY + 74,
			0xff00ff);
		getEngine()->drawForegroundRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX + 50, m_iCurrentScreenY + m_iStartDrawPosY + 50,
			m_iCurrentScreenX + m_iStartDrawPosX + 74, m_iCurrentScreenY + m_iStartDrawPosY + 74,
			0xffff00);
		getEngine()->drawForegroundRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX + 75, m_iCurrentScreenY + m_iStartDrawPosY + 50,
			m_iCurrentScreenX + m_iStartDrawPosX + 99, m_iCurrentScreenY + m_iStartDrawPosY + 74,
			0x0);

		getEngine()->drawForegroundLine(
			m_iCurrentScreenX + m_iStartDrawPosX + 75, m_iCurrentScreenY + m_iStartDrawPosY + 50,
			m_iCurrentScreenX + m_iStartDrawPosX + 99, m_iCurrentScreenY + m_iStartDrawPosY + 74,
			0xffffff);
		getEngine()->drawForegroundLine(
			m_iCurrentScreenX + m_iStartDrawPosX + 99, m_iCurrentScreenY + m_iStartDrawPosY + 50,
			m_iCurrentScreenX + m_iStartDrawPosX + 75, m_iCurrentScreenY + m_iStartDrawPosY + 74,
			0xffffff);

/*
		// Whether items can be dragged or not
		GetEngine()->DrawScreenRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX, m_iCurrentScreenY + m_iStartDrawPosY + 50,
			m_iCurrentScreenX + m_iStartDrawPosX + 24, m_iCurrentScreenY + m_iStartDrawPosY + 74,
			0xffffff);
		GetEngine()->DrawScreenRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX + 1, m_iCurrentScreenY + m_iStartDrawPosY + 51,
			m_iCurrentScreenX + m_iStartDrawPosX + 23, m_iCurrentScreenY + m_iStartDrawPosY + 73,
			0x0);
		GetEngine()->DrawScreenString(m_iCurrentScreenX + m_iStartDrawPosX + 5, m_iCurrentScreenY + m_iStartDrawPosY + 52,
			(CardGame::AllowItemDragging() ? "M" : "P"), 0xffff00, GetEngine()->GetFont("arial.ttf", 20));

		GetEngine()->DrawScreenRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX + 25, m_iCurrentScreenY + m_iStartDrawPosY + 50,
			m_iCurrentScreenX + m_iStartDrawPosX + 49, m_iCurrentScreenY + m_iStartDrawPosY + 74,
			0xffffff);
		GetEngine()->DrawScreenRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX + 26, m_iCurrentScreenY + m_iStartDrawPosY + 51,
			m_iCurrentScreenX + m_iStartDrawPosX + 48, m_iCurrentScreenY + m_iStartDrawPosY + 73,
			0x0);
		GetEngine()->DrawScreenString(m_iCurrentScreenX + m_iStartDrawPosX + 30, m_iCurrentScreenY + m_iStartDrawPosY + 52,
			(CardGame::ShouldAddCardToTopOfDeck() ? "T" : "B"), 0xffff00, GetEngine()->GetFont("arial.ttf", 20));

		GetEngine()->DrawScreenRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX + 50, m_iCurrentScreenY + m_iStartDrawPosY + 50,
			m_iCurrentScreenX + m_iStartDrawPosX + 74, m_iCurrentScreenY + m_iStartDrawPosY + 74,
			0x0ffffff);
		GetEngine()->DrawScreenRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX + 51, m_iCurrentScreenY + m_iStartDrawPosY + 51,
			m_iCurrentScreenX + m_iStartDrawPosX + 73, m_iCurrentScreenY + m_iStartDrawPosY + 73,
			0x0);
		GetEngine()->DrawScreenString(m_iCurrentScreenX + m_iStartDrawPosX + 55, m_iCurrentScreenY + m_iStartDrawPosY + 52,
			(CardGame::MakeCardsDisabled() ? "D" : "d"), 0xffff00, GetEngine()->GetFont("arial.ttf", 20));

		GetEngine()->DrawScreenRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX + 75, m_iCurrentScreenY + m_iStartDrawPosY + 50,
			m_iCurrentScreenX + m_iStartDrawPosX + 99, m_iCurrentScreenY + m_iStartDrawPosY + 74,
			0x0ffffff);
		GetEngine()->DrawScreenRectangle(
			m_iCurrentScreenX + m_iStartDrawPosX + 76, m_iCurrentScreenY + m_iStartDrawPosY + 51,
			m_iCurrentScreenX + m_iStartDrawPosX + 98, m_iCurrentScreenY + m_iStartDrawPosY + 73,
			0x0);
		GetEngine()->DrawScreenString(m_iCurrentScreenX + m_iStartDrawPosX + 80, m_iCurrentScreenY + m_iStartDrawPosY + 52,
			(CardGame::FlippingCards() ? "F" : "f"), 0xffff00, GetEngine()->GetFont("arial.ttf", 20));
*/
		getEngine()->drawForegroundString(m_iCurrentScreenX + m_iStartDrawPosX + 10, m_iCurrentScreenY + m_iStartDrawPosY + 1,
			"Tokens", 0, getEngine()->getFont("Cornerstone Regular.ttf", 20));

		// Now draw all tokens which are contained in the card
		drawTokensOnItem();
	}


	void createToken(unsigned int uiColour, int iX, int iY)
	{
		DisplayableObject* oNewToken = new DragableToken(getEngine(), iX, iY, uiColour);
		getEngine()->appendObjectToArray(oNewToken);
		this->redrawDisplay();
	}
};
