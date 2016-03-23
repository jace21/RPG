#include "Magic.h"


Magic::Magic()
{
	cost = 0;
	attack = 0;
	defense = 0;
	magicDefense = 0;
	heal = false;
}
void Magic::init(int c, int a, int d, int m, bool h, bool b, string n, string desc, AnimationEffects *an)
{
	cost = c;
	attack = a;
	defense = d;
	magicDefense = m;
	heal = h;
	buff = b;
	name = n;
	description = desc;
	anim = an;
}
void Magic::destroy()
{
}
int Magic::damage(int intel, int magDef)
{
	if (heal)
	{
		return (intel * attack);
	}
	else
	{
		return (intel * attack) - (magDef * attack) / 2 * (rand() % 1 + 0.8);
	}
}