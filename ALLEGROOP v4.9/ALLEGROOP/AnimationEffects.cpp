#include "AnimationEffects.h"

AnimationEffects::AnimationEffects(ALLEGRO_BITMAP *image)
{
	GameObject::init((WIDTH / 2) - 30, HEIGHT / 2 + 50 , 0, 0, 0, 0, 240, 240);
	setID(ANIMATIONS);
	setAlive(false);

	maxFrame = 8;
	curFrame = 0;
	animationColumns = 8;
	animationRows = 0;
	frameCount = 0;
	frameDelay = 5;
	frameWidth = 240;
	frameHeight = 240;
	chara = false;

	AnimationEffects::image = image;
}
void AnimationEffects::destroy()
{
	GameObject::destroy();
}
void AnimationEffects::update()
{
	GameObject::update();
	if (++frameCount >= frameDelay)
	{
		curFrame += 1;
		if (curFrame >= maxFrame)
		{
			curFrame = animationRows;
			if (!chara)
			setAlive(false);
		}
		frameCount = 0;
	}
}
void AnimationEffects::render()
{
	GameObject::render();
	int fx = (curFrame % animationColumns) * frameWidth;
	int fy = (curFrame / animationColumns) * frameHeight;

	al_draw_bitmap_region(image, fx, fy, frameWidth,
		frameHeight, x - frameWidth, y - frameHeight, 0);
}