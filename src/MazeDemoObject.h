#pragma once
#include "DisplayableObject.h"
#include "MovementPosition.h"

class MazeDemoMain;


class MazeDemoObject :
	public DisplayableObject
{
public:
	MazeDemoObject(MazeDemoMain* pEngine, int iMapX, int iMapY);
	~MazeDemoObject(void);

	void virtDraw() override;
	void virtDoUpdate( int iCurrentTime ) override;

	int getXDiffForDirection( int iDir )
	{
		switch( iDir%4 )
		{
		case 0: return 0; // Up
		case 1: return 1; // Right
		case 2: return 0; // Down
		case 3: return -1; // Left
		default: return 0; // Can never happen
		}
	}

	int getYDiffForDirection( int iDir )
	{
		switch( iDir%4 )
		{
		case 0: return -1; // Up
		case 1: return 0; // Right
		case 2: return 1; // Down
		case 3: return 0; // Left
		default: return 0; // Can never happen
		}
	}

private:
	MazeDemoMain* m_pMainEngine;
	MovementPosition m_oMover;

	int m_iMapX;
	int m_iMapY;
	int m_iDir;
};
