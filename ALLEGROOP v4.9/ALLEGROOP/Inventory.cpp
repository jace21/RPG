#include "Inventory.h"

Inventory::Inventory()
{
	current = 0;
}
void Inventory::init()
{
	total[0].setName("Potion");
	total[0].setDescription("Heals the player for 20 HP.");
	total[0].setValue(200);
	total[0].setType(5);
	total[0].setHeal(20);

	total[1].setName("Ether");
	total[1].setDescription("Restores 20 points of the player's MP.");
	total[1].setValue(300);
	total[1].setType(5);
	total[1].setMPHeal(20);

	total[2].setName("Training Sword");
	total[2].setDescription("Everyone's first sword. It's pretty bad.");
	total[2].setStrength(4);
	total[2].setValue(100);
	total[2].setType(0);
	total[2].setEquip(true);

	total[3].setName("Iron Sword");
	total[3].setDescription("Forged from the finest of low quality ores.");
	total[3].setStrength(10);
	total[3].setDefense(1);
	total[3].setValue(1000);
	total[3].setType(0);
	total[3].setEquip(true);

	total[4].setName("Wooden Staff");
	total[4].setDescription("This is a twig. Seriously. A twig...");
	total[4].setMagicDefense(1);
	total[4].setIntelligence(10);
	total[4].setValue(1000);
	total[4].setType(0);
	total[4].setEquip(true);

	total[5].setName("Leather Clothing");
	total[5].setDescription("An awkward amount of leather.");
	total[5].setDefense(4);
	total[5].setMagicDefense(2);
	total[5].setValue(100);
	total[5].setType(1);
	total[5].setEquip(true);

	total[6].setName("Iron Breastplate");
	total[6].setDescription("A metal chest armor that is kinda strong.");
	total[6].setDefense(10);
	total[6].setMagicDefense(1);
	total[6].setValue(1000);
	total[6].setType(1);
	total[6].setEquip(true);

	total[7].setName("Mage's Cloak");
	total[7].setDescription("A magical cloak which stolen from some nerd.");
	total[7].setDefense(1);
	total[7].setMagicDefense(10);
	total[7].setValue(1000);
	total[7].setType(1);
	total[7].setEquip(true);

	total[8].setName("Leather Gloves");
	total[8].setDescription("The latest fashion, surely it'll crumble.");
	total[8].setDefense(2);
	total[8].setMagicDefense(4);
	total[8].setValue(100);
	total[8].setType(2);
	total[8].setEquip(true);

	total[9].setName("Iron Gauntlets");
	total[9].setDescription("Semi-sturdy metal gloves. Hard on the hands.");
	total[9].setStrength(1);
	total[9].setDefense(8);
	total[9].setMagicDefense(1);
	total[9].setValue(1000);
	total[9].setType(2);
	total[9].setEquip(true);

	total[10].setName("Silk Gloves");
	total[10].setDescription("Very smooth, very sleek, very flammable.");
	total[10].setDefense(1);
	total[10].setMagicDefense(8);
	total[10].setIntelligence(1);
	total[10].setValue(1000);
	total[10].setType(2);
	total[10].setEquip(true);

	total[11].setName("Leather Boots");
	total[11].setDescription("Made for walking, but they'll protect.");
	total[11].setDefense(2);
	total[11].setMagicDefense(2);
	total[11].setValue(100);
	total[11].setType(3);
	total[11].setEquip(true);

	total[12].setName("Iron Boots");
	total[12].setDescription("Extremely uncomfortable, but hey. Swords, man.");
	total[12].setDefense(6);
	total[12].setMagicDefense(1);
	total[12].setValue(1000);
	total[12].setType(3);
	total[12].setEquip(true);

	total[13].setName("Mage Boots");
	total[13].setDescription("Like leather boots, but this one says mage.");
	total[13].setDefense(1);
	total[13].setMagicDefense(6);
	total[13].setValue(1000);
	total[13].setType(3);
	total[13].setEquip(true);

	total[14].setName("High Potion");
	total[14].setDescription("Heals 100 HP.");
	total[14].setValue(1000);
	total[14].setType(5);
	total[14].setHeal(100);

	total[15].setName("Big Ether");
	total[15].setDescription("Restores 100 points of the player's MP.");
	total[15].setValue(2000);
	total[15].setType(5);
	total[15].setMPHeal(100);

	playerInv[current] = total[0];
	playerInv[current].setQuantity(3);
	current++;

	playerInv[current] = total[1];
	playerInv[current].setQuantity(1);
	current++;
}
void Inventory::swapItem(int i, int j)
{
	Equipment temp;
	temp = playerInv[i];
	playerInv[i] = playerInv[j];
	playerInv[j] = temp;
}
void Inventory::addItem(int i)
{
	if (i > 0)
	{
		bool found = false;
		int j = 0;

		while (!found && j < current)
		{
			if (playerInv[j].getName() == total[i].getName())
			{
				playerInv[j].increaseQuantity();
				found = true;
			}
			else if (playerInv[j].getName() == "")
			{
				playerInv[j] = total[i];
				playerInv[j].setQuantity(1);
				found = true;
			}
			j++;
		}
		if (!found)
		{
			playerInv[current] = total[i];
			playerInv[current].setQuantity(1);
			current++;
		}
	}
}
void Inventory::removeItem(int i)
{
	int j = i;

	playerInv[i].decreaseQuantity();

		if (playerInv[i].getQuantity() == 0)
		{
			while (j + 1 < current)
			{
				playerInv[j] = playerInv[j + 1];
				j++;
			}
			playerInv[j].equipDelete();
			current--;
		}
}
void Inventory::useItem(Player *play, int i)
{
	int j = i;

	if (!playerInv[i].isEquipment())
	{
		play->addHealth(playerInv[i].getHeal());
		play->addMana(playerInv[i].getMPHeal());
		playerInv[i].decreaseQuantity();

		if (playerInv[j].getQuantity() == 0)
		{
			while (j + 1 < current)
			{
				playerInv[j] = playerInv[j + 1];
				j++;
			}
			playerInv[j].equipDelete();
			current--;
		}
	}
}
void Inventory::equipItem(Player *play, int i)
{
	if (playerInv[i].isEquipment())
	{
		int j = 0;
		bool found = false;
		while (j < 16 && !found)
		{
			if (playerInv[i].getName() == total[j].getName())
			{
				j--;
				found = true;
			}
			j++;
		}
		addItem(play->changeEquip(playerInv[i], j));
		removeItem(i);
	}
}
int Inventory::getMasterNumber(int i)
{
	int j = 0;
	while (j < 16)
	{
		if (playerInv[i].getName() == total[j].getName())
		{
			return j;
		}
		j++;
	}
	return -1;
}
void Inventory::addFromMaster(int i, int j, int q)
{
	if (j > 0 && j < 16)
	{
		playerInv[i] = total[j];
		playerInv[i].setQuantity(q);
		current++;
	}
}
int Inventory::playerFromMaster(Player *play, int i)
{
	int j = 0;
	while (j < 16)
	{
		if (play->getEquipName(i) == total[j].getName())
		{
			return j;
		}
		j++;
	}
	return -1;
}
void Inventory::equipFromMaster(Player *play, int i)
{
	if (i > 0 && i < 16)
	{
		play->changeEquip(total[i], i);
	}
}
void Inventory::destroy()
{
}
