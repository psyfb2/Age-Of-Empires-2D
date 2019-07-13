#pragma once
#include "DisplayableObject.h"


/* Circular Object which travels only across one line (x direction) and can deploy a special ability */
class SpecialAbilityDeployer :
	public DisplayableObject
{
public:
	// will not deviate from xStart
	SpecialAbilityDeployer(BaseEngine* engine, int centreX, int centreY, int radius, unsigned int color, const char* text, int textOffset, int leftBarrier, int rightBarrier);
	~SpecialAbilityDeployer();

	virtual inline bool isInBorders() {
		return (m_iCurrentScreenX + m_iStartDrawPosX >= leftBarrier) && (m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth <= rightBarrier);
	}

	// Tell the framework how to draw this object
	virtual void virtDraw() override;
	virtual void virtDoUpdate(int currentTime) override;
	virtual void virtMouseDown(int iButton, int x, int y) {
		if (iButton == SDL_BUTTON_LEFT) {
			if (x >= m_iCurrentScreenX + m_iStartDrawPosX && x <= m_iCurrentScreenX + m_iStartDrawPosX + m_iDrawWidth
				&& y >= m_iCurrentScreenY + m_iStartDrawPosY && y <= m_iCurrentScreenY + m_iStartDrawPosY + m_iDrawHeight) {
				// circle is clicked
				color = rand() % 256;
			}
		}
	}

protected:
	unsigned int color;
	const char* text;
	int textOffset;
	int leftBarrier;
	int rightBarrier;
	int speed;
};

