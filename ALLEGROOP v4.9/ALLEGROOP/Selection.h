#ifndef SELECTION_H
#define SELECTION_H

#include "GameObject.h"

class Selection : public GameObject
{
public:
	Selection();
	
	void destroy();

	void setStats(float x, float y);

	float getX();
	float getY();

private:
	float aX;
	float aY;
};

#endif

