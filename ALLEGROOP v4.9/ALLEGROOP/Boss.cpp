#include "Boss.h"

Boss::Boss(ALLEGRO_BITMAP *image)
{
	Boss::image = image;
}
void Boss::init(int h, int d, int magD, int mag, int mon, int exp, int thp, int maxF, int frDly, int frW, int frH)
{
	GameObject::init((WIDTH / 4), HEIGHT / 2, 0, 0, 0, 0, 96, 96);

	hp = h;
	damage = d;
	magicDefense = magD;
	magic = mag;
	moneyValue = mon;
	expValue = exp;
	totalHealth = thp;
	setID(ENEMY);
	setAlive(true);

	maxFrame = maxF;
	curFrame = 8;
	animationColumns = 8;
	animationRows = 4;
	frameCount = 0;
	frameDelay = frDly;
	frameWidth = frW;
	frameHeight = frH;
}
void Boss::destroy()
{
	GameObject::destroy();
}
void Boss::render()
{
	GameObject::render();
	int fx = (curFrame % 4) * frameWidth;
	int fy = (curFrame / 4) * frameHeight;

	al_draw_bitmap_region(image, fx, fy, frameWidth,
		frameHeight, x - frameWidth, y - frameHeight, 0);
}
void Boss::bodyRender()
{
	GameObject::render();
	int fx = (curFrame % 1) * frameWidth;
	int fy = (curFrame / 1) * frameHeight;

	al_draw_bitmap_region(image, fx, fy, frameWidth,
		frameHeight, x - frameWidth, y - frameHeight, 0);
}

void Boss::update()
{
	GameObject::update();
	if (++frameCount >= frameDelay)
	{
		curFrame += 1;
		if (curFrame >= maxFrame)
			curFrame = animationColumns;
		else if (curFrame <= 0)
			curFrame = maxFrame;

		frameCount = 0;
	}
}
void Boss::loselife(int playerDamage)
{
	hp -= playerDamage;
}

void Boss::heal()
{
	if ((hp + (magic * (damage / 25)) < totalHealth))
		hp += magic * (damage / 25);
	else
		hp = totalHealth;
}