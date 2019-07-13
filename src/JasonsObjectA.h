#pragma once
#include "DisplayableObject.h"

class JasonsObjectA :
	public DisplayableObject
{
public:
	JasonsObjectA(BaseEngine* pEngine);
	~JasonsObjectA();

	bool deleteOnRemoval() const override { return false; }
	void virtDraw() override;
	void virtDoUpdate(int iCurrentTime) override;
};

