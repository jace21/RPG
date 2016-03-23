#include "Equipment.h"

Equipment::Equipment()
{
	image = NULL;
	strength = 0;
	defense = 0;
	magicDefense = 0;
	intelligence = 0;
	value = 0;
	type = -1;
	equip = false;
	heal = 0;
	mpHeal = 0;
	quantity = 0;
}
Equipment::Equipment(ALLEGRO_BITMAP *avatar, const string& title, const string& descr, int str, int def, int magDef,
	int intel, int val, int typ, bool eq, int h, int mph, int q)
{
	image = avatar;
	name = title;
	description = descr;
	strength = str;
	defense = def;
	magicDefense = magDef;
	intelligence = intel;
	value = val;
	type = typ;
	equip = eq;
	heal = h;
	mpHeal = mph;
	quantity = q;
}
void Equipment::render(float x, float y)
{
	int fx = 0;
	int fy = 0;

	al_draw_bitmap_region(image, fx, fy, frameWidth,
		frameHeight, x, y, 0);
}
void Equipment::swapStats(Equipment &oldEquip)
{
	setName(oldEquip.getName());
	setDescription(oldEquip.getDescription());
	setStrength(oldEquip.getStrength());
	setDefense(oldEquip.getDefense());
	setMagicDefense(oldEquip.getMagicDefense());
	setIntelligence(oldEquip.getIntelligence());
	setValue(oldEquip.getValue());
	setType(oldEquip.getType());
	setEquip(oldEquip.isEquipment());
	setHeal(oldEquip.getHeal());
	setMPHeal(oldEquip.getMPHeal());
	setQuantity(oldEquip.getQuantity());
}
int Equipment::healPlayer()
{
	if (!equip)
	{
		return heal;
	}
	else
	{
		return NULL;
	}
}
int Equipment::mpHealPlayer()
{
	if (!equip)
	{
		return mpHeal;
	}
	else
	{
		return NULL;
	}
}
void Equipment::equipDelete()
{
	image = NULL;
	name = "";
	description = "";
	strength = 0;
	defense = 0;
	magicDefense = 0;
	intelligence = 0;
	value = 0;
	type = 0;
	equip = false;
	heal = 0;
	mpHeal = 0;
	quantity = 0;
}
void Equipment::destroy()
{
}