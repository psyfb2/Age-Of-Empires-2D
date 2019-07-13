#pragma once
#include "TileManager.h"
class MyTileManager :
	public TileManager
{
public:
	MyTileManager();
	~MyTileManager();
	void virtDrawTileAt(BaseEngine* pEngine, DrawingSurface* pSurface, int mapX, int mapY, 
		int startPositionScreenX, int startPositionSceenY) const;
};

