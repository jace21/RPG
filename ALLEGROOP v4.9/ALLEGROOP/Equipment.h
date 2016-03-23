#ifndef EQUIPMENT_H
#define EQUIPMENT_H

#include <string.h>
#include <cstring>
#include <string>
#include "GameObject.h"
class Equipment :
	public GameObject
{
public:
	Equipment();
	Equipment(ALLEGRO_BITMAP *avatar, const string&, const string&, int, int, int, int, int, int, bool, int, int, int);

	void destroy();

	void render(float, float);

	string getName() { return name; }
	string getDescription() { return description; }
	int getStrength() { return strength; }
	int getDefense() { return defense; }
	int getMagicDefense() { return magicDefense; }
	int getIntelligence() { return intelligence; }
	int getValue() { return value; }
	int getType() { return type; }
	bool isEquipment() { return equip; }
	int getHeal(){ return heal; }
	int getMPHeal(){ return mpHeal; }
	int getQuantity(){ return quantity; }

	void setName(const string& n) { name = n; }
	void setDescription(const string& d) { description = d; }
	void setStrength(int s) { strength = s; }
	void setDefense(int def) { defense = def; }
	void setMagicDefense(int magDef) { magicDefense = magDef; }
	void setIntelligence(int intel) { intelligence = intel; }
	void setValue(int val) { value = val; }
	void setType(int typ) { type = typ; }
	void setEquip(bool eq) { equip = eq; }
	void setHeal(int h) { heal = h; }
	void setMPHeal(int mph) { mpHeal = mph; }
	void setQuantity(int q){ quantity = q; }

	void increaseQuantity(){ if (quantity != 99 && type != -1) { quantity++; } }
	void decreaseQuantity(){ if (quantity != 0 && type != -1) { quantity--; } }

	int healPlayer();
	int mpHealPlayer();
	void swapStats(Equipment &oldEquip);
	void equipDelete();

private:

	ALLEGRO_BITMAP *image;
	string name;
	string description;
	int strength;
	int defense;
	int magicDefense;
	int intelligence;
	int value;
	int type;
	bool equip;
	int heal;
	int mpHeal;
	int quantity;
};
#endif
