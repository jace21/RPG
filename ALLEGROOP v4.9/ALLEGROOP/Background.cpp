#include "Background.h"

Background::Background(ALLEGRO_BITMAP *image, float velX, float velY, int selectionMax)
{
	GameObject::init(0, 0, velX, velY, 3.6, 3.6, 0, 0);

	setID(MISC);
	setCollidable(false);
	setAlive(true);
	Background::image = image;

	max = selectionMax;
	current = 0;
	
	frameWidth = al_get_bitmap_width(image);
	frameHeight = al_get_bitmap_height(image);
}
void Background::destroy()
{
	GameObject::destroy();
}
void Background::setBattleBackground(ALLEGRO_BITMAP *image)
{
	Background::image = image;
	x = 0;
	y = 0;
}
void Background::update()
{
	GameObject::update();
	x += velX * dirX;
	y += velY * dirY;
}
void Background::render()
{
	GameObject::render();
	al_draw_bitmap(image, x, y, 0);
}
void Background::moveUp()
{
	dirY = 1;
	dirX = 0;
}
void Background::moveDown()
{
	dirY = -1;
	dirX = 0;
}
void Background::moveLeft()
{
	dirY = 0;
	dirX = 1;
}
void Background::moveRight()
{
	dirY = 0;
	dirX = -1;
}
void Background::reset()
{
	dirY = 0;
	dirX = 0;
}
void Background::selectDecrease()
{
	current--;
}
void Background::selectIncrease()
{
	current++;
}
void Background::selectUpdate()
{
	if (current < 0)
	{
		current = max;
	}
	else if (current > max)
	{
		current = 0;
	}
}
void Background::selectRender(ALLEGRO_BITMAP *arrow)
{
	al_draw_bitmap(arrow, select[current].getX(), select[current].getY(), 0);
}
void Background::setSelect(float x, float y, int i)
{
	select[i].setStats(x, y);
}
int Background::getSelect()
{
	return current;
}