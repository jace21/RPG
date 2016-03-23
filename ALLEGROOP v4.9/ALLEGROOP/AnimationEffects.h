#ifndef ANIMATIONEFFECTS_H
#define ANIMATIONEFFECTS_H

#include "GameObject.h"

class AnimationEffects : public GameObject
{
public:
	AnimationEffects(ALLEGRO_BITMAP *image);

	void destroy();

	void update();
	void render();

	int getCurFrame() { return curFrame; }
	int getMaxFrame() { return maxFrame; }
	void setFrameWidth(int i) { frameWidth = i; }
	void setFrameHeight(int i) { frameHeight = i; }
	void setmaxFrame(int i) { maxFrame = i; }
	void setFrameDelay(int i) { frameDelay = i; }
	void setCurrentFrame(int i) { curFrame = i; }
	void setAnimationColumns(int i) { animationColumns = i; }
	void setAnimationRows(int i) { animationRows = i; }
	void setChara(bool what) { chara = what; }
	

private:
	bool chara;

};
#endif