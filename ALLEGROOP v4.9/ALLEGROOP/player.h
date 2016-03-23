#ifndef PLAYER_H
#define PLAYER_H
#include "GameObject.h"
#include "Equipment.h"
#include <string>
#include <cstring>

class Player : public GameObject
{
public:
	Player(ALLEGRO_BITMAP *image = NULL);
	void Destroy();

	void init();
	void update();
	void battleUpdate();
	void render();

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

	void resetUp();
	void resetDown();
	void resetLeft();
	void resetRight();

	void setBattle();
	void attackStance();

	int getHealth() { return hp; }
	int getMoney() { return money; }
	int getStrength() { return strength; }
	int getDefense() { return defense; }
	int getMagicDefense() { return magicDefense; }
	int getIntelligence() { return intelligence; }
	int attackValue() { return (attack + (strength * (rand() % 5) * .8)); }
	int getExp() { return currentExp; }
	int getTotalExp() { return totalExp; }
	int getTotalHealth() { return totalHealth; }
	int getMP() { return mp; }
	int getTotalMP() { return totalMp; }
	int getLevel() { return level; }
	int getNextLevel() { return levelExp; }
	int getMaxFrame() { return maxFrame; }
	int getCurFrame() { return curFrame; }
	int getEnergy() { return energy; }
	int getTotalEnergy() { return totalEnergy; }
	void setHp(int);
	void addMoney(int);
	void decreaseMoney(int i) { money -= i; }
	void addExp(int);
	void setMp(int newMp) { mp = newMp; }
	void loseLife(int);
	void setHittable(){ hit = false; }
	void setDefend(bool stance) { defend = stance; }
	void setAttacking(bool stance) { attacking = stance; }
	bool getDefend() { return defend; }
	void increaseEnergy();
	void setRecharge() { recharge = false; }
	void decreaseEnergy(int i){ energy -= i; }
	void setEnergy() { energy = 0; }
	int changeEquip(Equipment &, int);
	void subtractStats(Equipment);
	void addStats(Equipment);
	void addHealth(int);
	void addMana(int);
	void loseMana(int loss){ mp -= loss; };
	void setLevel(int);
	void setInsufficient(bool i) { insufficient = i; }
	bool getInsufficient() { return insufficient; }
	void addSupport(int s, int d, int m);
	void setGold(int newGold) { money = newGold; }
	void noSupport();
	void setExp(int newExp) { currentExp = newExp; }
	void setTotalExp(int newExp) { totalExp = newExp; }
	void setNextLevel(int newExp) { levelExp = newExp; }
	string getEquipName(int i) { return playerEquips[i].getName(); }

	

private:
	int money;
	int strength;
	int defense;
	int magicDefense;
	int intelligence;
	int hp;
	int mp;
	int totalExp;
	int totalHealth;
	int totalEnergy;
	int totalMp;
	int attack;
	int levelExp;
	int level;
	int currentExp;
	int energy;
	int tempS;
	int tempD;
	int tempMD;
	bool hit;
	bool defend;
	bool attacking;
	bool recharge;
	bool insufficient;
	int slot1;
	int slot2;
	int slot3;
	int slot4;
	Equipment playerEquips[4];
};

#endif