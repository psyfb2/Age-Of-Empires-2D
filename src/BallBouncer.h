#pragma once
#include "SpecialAbilityDeployer.h"
#include "BaseEngine.h"
#include "Psyfb2TileManager.h"

class BallBouncer :
	public SpecialAbilityDeployer
{
public:
	BallBouncer(BaseEngine* engine, int centreX, int centreY, int radius, unsigned int color, Psyfb2TileManager* triangleTM = nullptr, int* otherBallIndexes = nullptr, int otherBallIndexesSize = 0);
	~BallBouncer();
	virtual void virtDraw() override;
	void generateRandomCoordOnCircumfrence(int * x, int * y);
	virtual void virtDoUpdate(int currentTime) override;
	void setXSpeed(double xSpeed);
	void setYSpeed(double ySpeed);
	double getXSpeed();
	double getYSpeed();
private:
	double xSpeed;
	double ySpeed;
	int* otherBallIndexes; // used for collisions
	int otherBallIndexesSize;
	Psyfb2TileManager* triangleTM;
};

