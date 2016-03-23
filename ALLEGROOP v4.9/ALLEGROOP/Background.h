#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "GameObject.h"
#include "Selection.h"

class Background : public GameObject
{
public:
	Background(ALLEGRO_BITMAP *image, float velX, float velY, int selectionMax);

	void destroy();
	void update();
	void render();

	void setBattleBackground(ALLEGRO_BITMAP *image);
	void setBackground();

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	void reset();

	void selectDecrease();
	void selectIncrease();
	void selectUpdate();
	void selectRender(ALLEGRO_BITMAP *arrow);
	void setSelect(float x, float y, int i);
	
	int getSelect();

private:
	int max;
	int current;
	Selection select[40];
};

#endif

