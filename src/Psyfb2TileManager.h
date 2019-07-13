#pragma once
#include "TileManager.h"
#include "BaseEngine.h"

class Psyfb2TileManager :
	public TileManager
{
public:
	Psyfb2TileManager(int tileWidth, int tileHeight, int numberOfXTiles, int numberOfYTiles);
	~Psyfb2TileManager();
	void virtual virtDrawTileAt(
		BaseEngine* pEngine,
		DrawingSurface* pSurface,
		int iMapX, int iMapY,
		int iStartPositionScreenX, int iStartPositionScreenY) const override;
};

