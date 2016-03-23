#include "Selection.h"

Selection::Selection()
{
}
void Selection::destroy()
{
	GameObject::destroy();
}
void Selection::setStats(float x, float y)
{
	aX = x;
	aY = y;
}
float Selection::getX()
{
	return aX;
}
float Selection::getY()
{
	return aY;
}
