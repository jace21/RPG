#ifndef MAGIC_H
#define MAGIC_H

#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include "GameObject.h"
#include "player.h"
#include "AnimationEffects.h"
class Magic :
	public GameObject

{
public:
	Magic();

	void destroy();

	void init(int, int, int, int, bool, bool, string, string, AnimationEffects *);

	int damage(int, int);

	bool doesItHeal(){ return heal; }
	bool doesItSupport(){ return buff; }
	int getCost(){ return cost; }
	int getAttack(){ return attack; }
	int getDefense(){ return defense; }
	int getMagicDefense(){ return magicDefense; }
	string getName(){ return name; }
	string getDescription(){ return description; }

	void playAnim(){ anim->setAlive(true); }
	void setUsed(bool slotted){ used = slotted; }
	bool getUsed() { return used; }

private:
	int cost;
	int attack;
	int defense;
	int magicDefense;
	bool heal;
	bool buff;
	bool used;
	string name;
	string description;
	AnimationEffects *anim;
};
#endif
