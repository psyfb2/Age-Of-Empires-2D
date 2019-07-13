#pragma once

#include "TileManager.h"

class MazeDemoTileManager :
	public TileManager
{
public:
	MazeDemoTileManager(void) : TileManager(50, 50)
	{
	}

	~MazeDemoTileManager(void)
	{
	}

	// Draw a tile at a location on the screen determined by the tile
	// position and the base X and Y co-ordinates
	// OVERRIDE THIS TO DRAW YOUR TILE!!!
	// Draw a tile
	virtual void virtDrawTileAt( 
		BaseEngine* pEngine, 
		DrawingSurface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY ) const override;
};

