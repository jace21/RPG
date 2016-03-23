#ifndef ENEMY_H
#define ENEMY_H
#include "GameObject.h"

class Enemy : public GameObject
{
public:
	Enemy(ALLEGRO_BITMAP *image);

	void init();
	void destroy();

	void render();
	void update();
	void attack();
	void loselife(int);

	int getHealth() { return hp; }
	int getDamage() { return damage;}
	int getMagicDefense(){ return magicDefense; }
	int getMoneyValue() { return moneyValue; }
	int getExpValue() { return expValue; }
	int getTotalHealth() { return totalHealth; }

private:
	int hp;
	int damage;
	int magicDefense;
	int moneyValue;
	int expValue;
	int totalHealth;
};
#endif

