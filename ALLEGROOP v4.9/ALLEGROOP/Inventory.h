#ifndef INVENTORY_H
#define INVENTORY_H

#include "GameObject.h"
#include "Equipment.h"
#include "player.h"
#include <iostream>
#include <string>
#include <string.h>
#include <cstring>

class Inventory :
	public GameObject
{
public:
	Inventory();

	void init();

	void swapItem(int i, int j);
	void addItem(int i);
	void removeItem(int i);
	void useItem(Player *play, int i);
	void equipItem(Player *play, int i);

	string getDescription(int i) { return playerInv[i].getDescription(); }
	string getName(int i) { return playerInv[i].getName(); }
	string getDescriptionTotal(int i) { return total[i].getDescription(); }
	string getNameTotal(int i) { return total[i].getName(); }
	int getValue(int i) { return playerInv[i].getValue(); }
	int getValueTotal(int i) { return total[i].getValue(); }
	int getQuantity(int i) { return playerInv[i].getQuantity(); }
	bool isEquipment(int i) { return playerInv[i].isEquipment(); }
	int getStrength(int i) { return playerInv[i].getStrength();  }
	int getDefense(int i) { return playerInv[i].getDefense(); }
	int getMagicDefense(int i) { return playerInv[i].getMagicDefense(); }
	int getIntelligence(int i) { return playerInv[i].getIntelligence(); }

	int getMasterNumber(int i);
	void addFromMaster(int i, int j, int q);
	int playerFromMaster(Player *play, int i);
	void equipFromMaster(Player *play, int i);

	void destroy();

private:
	Equipment total[40];
	Equipment playerInv[40];
	int current;
};

#endif