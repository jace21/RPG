#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <iostream>
#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include "Globals.h"

using namespace std;
class GameObject
{
public:
	GameObject();

	void virtual destroy();

	void init(float x, float y, float velX, float velY, int dirX, int dirY, int boundX, int boundY);
	void virtual update();
	void virtual render();

	float getX() { return x;}
	float getY() { return y;}

	void setX(float x) { GameObject::x = x;}
	void setY(float y) { GameObject::y = y;}

	int getBoundX() { return boundX;}
	int getBoundY() { return boundY;}

	int getID() { return ID;}
	void setID(int ID){GameObject::ID = ID;}

	bool getAlive() { return alive;}
	void setAlive(bool alive){ GameObject::alive = alive;}

	bool getCollidable() { return collidable;}
	void setCollidable(bool collidable) { GameObject::collidable = collidable;}

	bool checkCollisions(GameObject *otherObject);
	void virtual collided(int objectID);
	bool canCollidable();

private:
	int ID;
	bool alive;
	bool collidable;

protected:
	float x;
	float y;

	float velX;
	float velY;

	int dirX;
	int dirY;

	int boundX;
	int boundY;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameHeight;
	int frameWidth;
	int animationColumns;
	int animationRows;
	int animationDirection;

	ALLEGRO_BITMAP *image;
};
#endif
