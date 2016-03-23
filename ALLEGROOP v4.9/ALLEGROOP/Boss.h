#ifndef BOSS_H
#define BOSS_H
#include "GameObject.h"

class Boss : public GameObject
{
public:
	Boss(ALLEGRO_BITMAP *image);

	void init(int h, int d, int magD, int mag, int mon, int exp, int thp, int maxF, int frDly, int frW, int frH);
	void destroy();
	void bodyRender();
	void render();
	void update();
	void attack();
	void loselife(int);
	void heal();

	int getHealth() { return hp; }
	int getDamage() { return damage; }
	int getMagicDefense(){ return magicDefense; }
	int getMoneyValue() { return moneyValue; }
	int getExpValue() { return expValue; }
	int getTotalHealth() { return totalHealth; }
	void setMaxFrame(int i) { maxFrame = i; }
	void setCurrentFrame(int i) { curFrame = i; }
	void setAnimationColumns(int i) { animationColumns = i; }
	void setAnimationRows(int i) { animationRows = i; }
private:
	int hp;
	int damage;
	int magicDefense;
	int magic;
	int moneyValue;
	int expValue;
	int totalHealth;
};
#endif