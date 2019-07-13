#pragma once

#include "BaseEngine.h"
#include "TileManager.h"
#include "MazeDemoTileManager.h"

class MazeDemoMain :
	public BaseEngine
{
public:
	MazeDemoMain(void);
	~MazeDemoMain(void);

	virtual void virtSetupBackgroundBuffer() override;
	virtual int virtInitialiseObjects() override;
	virtual void virtDrawStringsUnderneath() override;
	virtual void virtMouseDown(int iButton, int iX, int iY) override;
	virtual void virtKeyDown(int iKeyCode) override;


	// Get a reference to the current tile manager
	MazeDemoTileManager& GetTileManager() { return m_oTiles; }

private:
	MazeDemoTileManager m_oTiles;

public:
	// State number
	enum State { stateInit, stateMain, statePaused };

	// Work out what this means yourself
	int m_iPauseStarted;

private:
	State m_state;
};

