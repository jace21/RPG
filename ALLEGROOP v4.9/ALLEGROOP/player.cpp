#include "player.h"

Player::Player(ALLEGRO_BITMAP *image)
{
	if (image != NULL)
	{
		GameObject::image = image;
	}
}
void Player::Destroy()
{
	GameObject::destroy();
}
void Player::init()
{
	GameObject::init(WIDTH / 2, HEIGHT / 2, 3.6, 3.6, 0, 0, 32, 64);

	setID(PLAYER);
	setAlive(true);
	maxFrame = 273;
	curFrame = 0;
	frameCount = 0;
	frameDelay = 6;
	frameWidth = 64;
	frameHeight = 64;
	animationColumns = 1;
	animationDirection = 1;
	animationRows = 2;
	strength = 10;
	intelligence = 10;
	magicDefense = 20;
	defense = 20;
	money = 1000;
	hp = 100;
	mp = 100;
	energy = 0;
	totalEnergy = 20;
	totalHealth = 100;
	totalMp = 100;
	attack = 10;
	levelExp = 250;
	totalExp = 0;
	currentExp = 0;
	level = 1;
	attacking = false;
	slot1 = -1;
	slot2 = -1;
	slot3 = -1;
	slot4 = -1;
}
void Player::update()
{
	GameObject::update();
	if (++frameCount >= frameDelay)
	{
		curFrame += animationDirection;
		if (curFrame >= (13 * animationRows) + animationColumns)
			curFrame = (13 * animationRows);
		else if (curFrame <= 13 * animationRows)
			curFrame = (13 * animationRows) + animationColumns - 1;

		frameCount = 0;
	}

	x += velX * dirX;
	y += velY * dirY;

	if (!attacking)
	{
		if (x <= 640 / 2 - frameWidth / 2)
			x = 640 / 2 - frameWidth / 2;
		else if (x >= 640 / 2 - frameWidth / 2)
			x = 640 / 2 - frameWidth / 2;
		if (y <= 480 / 2 - frameHeight / 2)
			y = 480 / 2 - frameHeight / 2;
		else if (y >= 480 / 2 - frameHeight / 2)
			y = 480 / 2 - frameHeight / 2;
	}
}
void Player::render()
{
	GameObject::render();
	int fx = (curFrame % 13) * frameWidth;
	int fy = (curFrame / 13) * frameHeight;

	al_draw_bitmap_region(image, fx, fy, frameWidth,
		frameHeight, x, y, 0);
}
void Player::setHp(int hp)
{
	Player::hp = hp;
}
void Player::moveUp()
{
	dirY = -1;
	dirX = 0;
	animationColumns = 8;
	animationRows = 8;
	curFrame = 104;
}
void Player::moveDown()
{
	dirY = 1;
	dirX = 0;
	animationColumns = 8;
	animationRows = 10;
	curFrame = 130;
}
void Player::moveLeft()
{
	dirX = -1;
	dirY = 0;
	animationColumns = 8;
	animationRows = 9;
	curFrame = 117;
}
void Player::moveRight()
{
	dirX = 1;
	dirY = 0;
	animationColumns = 8;
	animationRows = 11;
	curFrame = 143;
}
void Player::resetUp()
{
	dirY = 0;
	dirX = 0;
	animationColumns = 1;
	animationRows = 8;
	curFrame = 104;
}
void Player::resetDown()
{
	dirY = 0;
	dirX = 0;
	animationColumns = 1;
	animationRows = 10;
	curFrame = 130;
}
void Player::resetLeft()
{
	dirY = 0;
	dirX = 0;
	animationColumns = 1;
	animationRows = 9;
	curFrame = 117;
}
void Player::resetRight()
{
	dirY = 0;
	dirX = 0;
	animationColumns = 1;
	animationRows = 11;
	curFrame = 143;
}
void Player::setBattle()
{
	x = (WIDTH * 3) / 4;
	y = HEIGHT / 2;
	animationColumns = 1;
	animationRows = 9;
	curFrame = 117;
}
void Player::attackStance()
{
	x = 640 / 3;
	y = 240;
	animationColumns = 12;
	animationRows = 17;
	curFrame = 189;
	dirX = -1;
}
void Player::addMoney(int newMoney)
{
	money += newMoney;
}
void Player::increaseEnergy()
{
	if (!recharge)
	{
		if ((energy + 5) < totalEnergy)
		{
			energy += 5;
			recharge = true;
		}
		else if ((energy + 5) >= totalEnergy)
		{
			energy = totalEnergy;
		}
	}
}
void Player::addExp(int newExp)
{
	currentExp += newExp;
	totalExp += newExp;
	if (currentExp >= levelExp)
	{
		level += 1;
		magicDefense += 6;
		defense += 6;
		strength += 4;
		attack += 5;
		intelligence += 5;
		totalHealth += 50;
		totalMp += 20;
		mp = totalMp;
		hp = totalHealth;
		currentExp = currentExp % levelExp;
		levelExp = levelExp * 1.2;
	}
}
void Player::loseLife(int damage)
{
	if (!hit)
	{
		if (defend == false)
		{
			if (.2 * (defense + magicDefense) <= damage)
			{
				hp -= damage - (.2 *(defense + magicDefense));
			}
			hit = true;
		}
		else if (defend == true)
		{
			if (.5 * (defense + magicDefense) <= damage)
			{
				hp -= damage - (.5 * (defense + magicDefense));
			}
			hit = true;
		}
	}
}
void Player::subtractStats(Equipment oldEquip)
{
	strength -= oldEquip.getStrength();
	defense -= oldEquip.getDefense();
	magicDefense -= oldEquip.getMagicDefense();
	intelligence -= oldEquip.getIntelligence();
}
void Player::addStats(Equipment newEquip)
{
	strength += newEquip.getStrength();
	defense += newEquip.getDefense();
	magicDefense += newEquip.getMagicDefense();
	intelligence += newEquip.getIntelligence();
}
int Player::changeEquip(Equipment &newEquip, int i)
{
	int temp = -1;
	switch (newEquip.getType())
	{
	case 0:
		subtractStats(playerEquips[0]);
		addStats(newEquip);
		playerEquips[0].swapStats(newEquip);
		temp = slot1;
		slot1 = i;
		break;
	case 1:
		subtractStats(playerEquips[1]);
		addStats(newEquip);
		playerEquips[1].swapStats(newEquip);
		temp = slot2;
		slot2 = i;
		break;
	case 2:
		subtractStats(playerEquips[2]);
		addStats(newEquip);
		playerEquips[2].swapStats(newEquip);
		temp = slot3;
		slot3 = i;
		break;
	case 3:
		subtractStats(playerEquips[3]);
		addStats(newEquip);
		playerEquips[3].swapStats(newEquip);
		temp = slot4;
		slot4 = i;
		break;
	}
	return temp;
}
void Player::addHealth(int h)
{
	if ((hp + h) < totalHealth)
	{
		hp += h;
	}
	else
	{
		hp = totalHealth;
	}
}
void Player::addMana(int m)
{
	if ((mp + m) < totalMp)
	{
		mp += m;
	}
	else
	{
		mp = totalMp;
	}
}
void Player::addSupport(int s, int d, int m)
{
	strength += s;
	defense += d;
	magicDefense += m;
	tempS += s;
	tempD += d;
	tempMD += m;
}
void Player::noSupport()
{
	strength -= tempS;
	defense -= tempD;
	magicDefense -= tempMD;
	tempS = 0;
	tempD = 0;
	tempMD = 0;
}
void Player::setLevel(int newLevel)
{
	level = newLevel;
	magicDefense = 20 + (6 * (newLevel - 1));
	defense = 20 + (6 * (newLevel - 1));
	strength = 10 + (4 * (newLevel - 1));
	attack = 10 + (5 * (newLevel - 1));
	intelligence = 10 + (5 * (newLevel - 1));
	totalHealth = 100 + (50 * (newLevel - 1));
	totalMp = 100 + (20 * (newLevel - 1));
	hp = totalHealth;
	mp = totalMp;
}