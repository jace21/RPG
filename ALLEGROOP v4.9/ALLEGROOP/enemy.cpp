#include "enemy.h"

Enemy::Enemy(ALLEGRO_BITMAP *image)
{
	Enemy::image = image;
}
void Enemy::init()
{
	GameObject::init((WIDTH / 4), HEIGHT / 2, 0, 0, 0, 0, 58, 46);

	hp = 500;
	damage = 35;
	magicDefense = 10;
	moneyValue = 200;
	expValue = 250;
	totalHealth = 500;
	setID(ENEMY);
	setAlive(true);

	maxFrame = 8;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 58;
	frameHeight = 46;
}
void Enemy::destroy()
{
	GameObject::destroy();
}
void Enemy::render()
{
	GameObject::render();
	int fx = (curFrame % 8) * frameWidth;
	int fy = (curFrame / 8) * frameHeight;

	al_draw_bitmap_region(image, fx, fy, frameWidth,
		frameHeight, x - frameWidth, y - frameHeight, 0);
}
void Enemy::update()
{
	GameObject::update();
	if (++frameCount >= frameDelay)
	{
		curFrame += 1;
		if (curFrame >= maxFrame)
			curFrame = 0;
		else if (curFrame <= 0)
			curFrame = maxFrame;

		frameCount = 0;
	}
}
void Enemy::loselife(int playerDamage)
{
	hp -= playerDamage;
}