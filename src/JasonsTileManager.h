#pragma once
#include "TileManager.h"
class JasonsTileManager :
	public TileManager
{
public:
	JasonsTileManager();
	~JasonsTileManager();

	// Draw a tile at a location on the screen determined by the tile position and the base X and Y co-ordinates
	// OVERRIDE THIS TO DRAW YOUR TILE!!!
	// Note: it can be const since the function does not alter the tile manager, even though it does alter the screen
	virtual void virtDrawTileAt(
		BaseEngine* pEngine,
		DrawingSurface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY) const;

};

