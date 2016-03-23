#include <allegro5\allegro.h>
#include <allegro5\allegro_image.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <list>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#include "GameObject.h"
#include "player.h"
#include "Globals.h"
#include "enemy.h"
#include "AnimationEffects.h"
#include "Background.h"
#include "Magic.h"
#include "Equipment.h"
#include "Inventory.h"
#include "Boss.h"

#include <iostream>
using namespace std;

//Enumerators
bool options[3] = { true, true, true };
enum OPTIONS{ MUSIC, SFX, FRAMECOUNTER };
bool keys[7] = { false, false, false, false, false, false, false };
enum KEYS{UP, DOWN, LEFT, RIGHT, SPACE, ESCAPE, ENTER};

//Game Controls
int randomEncounter = 0;
int gameControl = 0;
int lastState = -1;
int levelState = -1;
int gambleInput = 0;
int elapsedControl = 0;

//Load Checks
int stage = 1;
int stage1 = 1;
int stage2 = 1;
int stage3 = 1;
int totalGameTime1 = 0;
int totalGameTime2 = 0;
int totalGameTime3 = 0;
int magic1 = 0;
int magic2 = 0;
int magic3 = 0;
int magic4 = 0;
bool level2 = false;
bool loadfile1 = false;
bool loadfile2 = false;
bool loadfile3 = false;
bool walkback = false;
bool walkback2 = false;
int bosspos = 200;
int bosspos2 = 400;
int conversationCount = 1;
int conversationEnd = 1;
int bossConversation = 1;

//Globals
Player *player;
Enemy *enemy;
Boss *boss;
Boss *boss2;
Boss *boss3;
Inventory inv;
Magic master[8];
Magic currentMag[4];

//Background
Background *bg;
Background *bg2;
Background *titleScreen1;
Background *titleScreen2;
Background *battleScreen;
Background *battleScreen2;
Background *menuScreen;
Background *lostScreen;
Background *battleControl;
Background *statusScreen;
Background *optionScreen;
Background *saveScreen;
Background *loadScreen;
Background *magicScreen;
Background *equipmentScreen;
Background *itemScreen;
Background *victoryScreen;
Background *magicControl;
Background *magicSlots;
Background *worldScreen;
Background *shopMenu;
Background *shopControl;
Background *buysellEquipment;
Background *buysellItem;
Background *gambleScreen;
Background *bossScreen;
Background *bossScreen2;
Background *buy;
Background *sell;
Background *conversation;
Background *about;
Background *credits;

//Animation Effects
AnimationEffects *slash;
AnimationEffects *enemySlash;
AnimationEffects *fire;
AnimationEffects *ice;
AnimationEffects *lightning;
AnimationEffects *wind;
AnimationEffects *defensive;
AnimationEffects *protect;
AnimationEffects *brave;
AnimationEffects *cure;
AnimationEffects *squidsplosion;
AnimationEffects *bossCure;
AnimationEffects *firebreath;
AnimationEffects *tsunami;
AnimationEffects *shell;
AnimationEffects *sayaka;
AnimationEffects *enemyIce;
AnimationEffects *bossHead;
AnimationEffects *arms;
AnimationEffects *hellfire;
AnimationEffects *meteor;
AnimationEffects *curse;
AnimationEffects *fist;
AnimationEffects *death;
AnimationEffects *teleport;

//Iterators
list<GameObject *> objects;
list<GameObject *>::iterator iter;
list<GameObject *>::iterator iter2;

//Sample Instances
ALLEGRO_SAMPLE_INSTANCE *songInstance;
ALLEGRO_SAMPLE_INSTANCE *changeInstance;
ALLEGRO_SAMPLE_INSTANCE *selectInstance;
ALLEGRO_SAMPLE_INSTANCE *battleInstance;
ALLEGRO_SAMPLE_INSTANCE *themeInstance;
ALLEGRO_SAMPLE_INSTANCE *gameOverInstance;
ALLEGRO_SAMPLE_INSTANCE *victoryInstance;
ALLEGRO_SAMPLE_INSTANCE *saveloadInstance;
ALLEGRO_SAMPLE_INSTANCE *attackInstance;
ALLEGRO_SAMPLE_INSTANCE *fleeInstance;
ALLEGRO_SAMPLE_INSTANCE *fireInstance;
ALLEGRO_SAMPLE_INSTANCE *thunderInstance;
ALLEGRO_SAMPLE_INSTANCE *iceInstance;
ALLEGRO_SAMPLE_INSTANCE *worldInstance;
ALLEGRO_SAMPLE_INSTANCE *shopInstance;
ALLEGRO_SAMPLE_INSTANCE *menuInstance;
ALLEGRO_SAMPLE_INSTANCE *barrierInstance;
ALLEGRO_SAMPLE_INSTANCE *healInstance;
ALLEGRO_SAMPLE_INSTANCE *protectInstance;
ALLEGRO_SAMPLE_INSTANCE *buySellInstance;
ALLEGRO_SAMPLE_INSTANCE *squidwalkInstance;
ALLEGRO_SAMPLE_INSTANCE *squidAttackInstance;
ALLEGRO_SAMPLE_INSTANCE *squidbossInstance;
ALLEGRO_SAMPLE_INSTANCE *dragonInstance;
ALLEGRO_SAMPLE_INSTANCE *leveltwoInstance;
ALLEGRO_SAMPLE_INSTANCE * windInstance;
ALLEGRO_SAMPLE_INSTANCE *conversationInstance;
ALLEGRO_SAMPLE_INSTANCE *tsunamiInstance;
ALLEGRO_SAMPLE_INSTANCE *demonRoarInstance;
ALLEGRO_SAMPLE_INSTANCE *lastBossInstance;
ALLEGRO_SAMPLE_INSTANCE *dragonRoarInstance;
ALLEGRO_SAMPLE_INSTANCE *firefallInstance;
ALLEGRO_SAMPLE_INSTANCE *clockInstance;
ALLEGRO_SAMPLE_INSTANCE *hittingInstance;


//Game Timers
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_TIMER *gameTimer = NULL;
ALLEGRO_TIMER *battleTimer = NULL;
ALLEGRO_TIMER *elapsed = NULL;

//State Machine Management
void changeState(int &state, int newState);
void save(int);
void load(int);
void checkload(int);

int main()
{
	//SHELL VARIABLES
	bool done = false;
	bool render = false;
	bool battleLock = false;


	//Game Timer Handlers
	float gameTime = 0;
	int frames = 0;
	int gameFPS = 0;

	//Random Variables
	int state = -1;

	//PROJECT VARIABLES
	ALLEGRO_BITMAP *playerImage = NULL;
	ALLEGRO_BITMAP *enemyImage = NULL;
	ALLEGRO_BITMAP *animationImage = NULL;
	ALLEGRO_BITMAP *bgImage = NULL;
	ALLEGRO_BITMAP *bg2Image = NULL;
	ALLEGRO_BITMAP *battleImage = NULL;
	ALLEGRO_BITMAP *battleImage2 = NULL;
	ALLEGRO_BITMAP *titleImage1 = NULL;
	ALLEGRO_BITMAP *titleImage2 = NULL;
	ALLEGRO_BITMAP *lostImage = NULL;
	ALLEGRO_BITMAP *menuImage = NULL;
	ALLEGRO_BITMAP *statusImage = NULL;
	ALLEGRO_BITMAP *optionImage = NULL;
	ALLEGRO_BITMAP *battleCommand = NULL;
	ALLEGRO_BITMAP *magicCommand = NULL;
	ALLEGRO_BITMAP *saveImage = NULL;
	ALLEGRO_BITMAP *loadImage = NULL;
	ALLEGRO_BITMAP *magicImage = NULL;
	ALLEGRO_BITMAP *optionOff = NULL;
	ALLEGRO_BITMAP *optionOn = NULL;
	ALLEGRO_BITMAP *arrow = NULL;
	ALLEGRO_BITMAP *equipmentImage = NULL;
	ALLEGRO_BITMAP *itemImage = NULL;
	ALLEGRO_BITMAP *victoryImage = NULL;
	ALLEGRO_BITMAP *slashImage = NULL;
	ALLEGRO_BITMAP *fireImage = NULL;
	ALLEGRO_BITMAP *iceImage = NULL;
	ALLEGRO_BITMAP *lightningImage = NULL;
	ALLEGRO_BITMAP *windImage = NULL;
	ALLEGRO_BITMAP *defenseImage = NULL;
	ALLEGRO_BITMAP *slotImage = NULL;
	ALLEGRO_BITMAP *worldImage = NULL;
	ALLEGRO_BITMAP *shopImage = NULL;
	ALLEGRO_BITMAP *shopControlImage = NULL;
	ALLEGRO_BITMAP *playerArrow = NULL;
	ALLEGRO_BITMAP *buysellEquipmentImage = NULL;
	ALLEGRO_BITMAP *buysellItemImage = NULL;
	ALLEGRO_BITMAP *gambleImage = NULL;
	ALLEGRO_BITMAP *oneMinute = NULL;
	ALLEGRO_BITMAP *square = NULL;
	ALLEGRO_BITMAP *protectImage = NULL;
	ALLEGRO_BITMAP *braveImage = NULL;
	ALLEGRO_BITMAP *bossbg = NULL;
	ALLEGRO_BITMAP *bossbg2 = NULL;
	ALLEGRO_BITMAP *bossImage = NULL;
	ALLEGRO_BITMAP *bossImage2 = NULL;
	ALLEGRO_BITMAP *cureImage = NULL;
	ALLEGRO_BITMAP *squidbomb = NULL;
	ALLEGRO_BITMAP *hime = NULL;
	ALLEGRO_BITMAP *himeSurprised = NULL;
	ALLEGRO_BITMAP *dialogue = NULL;
	ALLEGRO_BITMAP *fairy = NULL;
	ALLEGRO_BITMAP *fairydialogue = NULL;
	ALLEGRO_BITMAP *aboutMenu = NULL;
	ALLEGRO_BITMAP *ending = NULL;
	ALLEGRO_BITMAP *breathe = NULL;
	ALLEGRO_BITMAP *shellImage = NULL;
	ALLEGRO_BITMAP *sayakaRun = NULL;
	ALLEGRO_BITMAP *tsunamiImage = NULL;
	ALLEGRO_BITMAP *bodyImage = NULL;
	ALLEGRO_BITMAP *demon = NULL;
	ALLEGRO_BITMAP *armsImage = NULL;
	ALLEGRO_BITMAP *hellfireImage = NULL;
	ALLEGRO_BITMAP *meteorImage = NULL;
	ALLEGRO_BITMAP *demon1 = NULL;
	ALLEGRO_BITMAP *dialogueLB = NULL;
	ALLEGRO_BITMAP *curseImage = NULL;
	ALLEGRO_BITMAP *deathImage = NULL;
	ALLEGRO_BITMAP *fistImage = NULL;
	ALLEGRO_BITMAP *teleportImage = NULL;

	//Sound Samples
	ALLEGRO_SAMPLE *song = NULL;
	ALLEGRO_SAMPLE *selectSound = NULL;
	ALLEGRO_SAMPLE *battleSong = NULL;
	ALLEGRO_SAMPLE *themeSong = NULL;
	ALLEGRO_SAMPLE *gameOver = NULL;
	ALLEGRO_SAMPLE *victorySong = NULL;
	ALLEGRO_SAMPLE *saveloadSong = NULL;
	ALLEGRO_SAMPLE *attackSound = NULL;
	ALLEGRO_SAMPLE *fleeSound = NULL;
	ALLEGRO_SAMPLE *fireSound = NULL;
	ALLEGRO_SAMPLE *iceSound = NULL;
	ALLEGRO_SAMPLE *thunderSound = NULL;
	ALLEGRO_SAMPLE *worldSound = NULL;
	ALLEGRO_SAMPLE *shopSound = NULL;
	ALLEGRO_SAMPLE *healSound = NULL;
	ALLEGRO_SAMPLE *barrierSound = NULL;
	ALLEGRO_SAMPLE *windSound = NULL;
	ALLEGRO_SAMPLE *protectSound = NULL;
	ALLEGRO_SAMPLE *buySellSound = NULL;
	ALLEGRO_SAMPLE *squidWalk = NULL;
	ALLEGRO_SAMPLE *squidAttack = NULL;
	ALLEGRO_SAMPLE *squidBoss = NULL;
	ALLEGRO_SAMPLE *dragonSound = NULL;
	ALLEGRO_SAMPLE *leveltwo = NULL;
	ALLEGRO_SAMPLE *conversationSound = NULL;
	ALLEGRO_SAMPLE *tsunamiSound = NULL;
	ALLEGRO_SAMPLE *dragonRoar = NULL;
	ALLEGRO_SAMPLE *demonRoar = NULL;
	ALLEGRO_SAMPLE *lastBoss = NULL;
	ALLEGRO_SAMPLE *fireFall = NULL;
	ALLEGRO_SAMPLE *clockSound = NULL;
	ALLEGRO_SAMPLE *ramSound = NULL;

	//ALLEGRO VARIABLES
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_FONT *font18 = NULL;
	ALLEGRO_FONT *font24 = NULL;
	ALLEGRO_FONT *font32 = NULL;
	ALLEGRO_FONT *font48 = NULL;

	//Allegro Error Check
	if (!al_init())
	{
		cerr << "Allegro failed to Initiate";
		return -1;
	}
	display = al_create_display(WIDTH, HEIGHT);
	if (!display)
	{
		cerr << "Display failed to Initialize";
		return -1;
	}

	//ADDON INSTALL
	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();

	//Project Variable Init
	font18 = al_load_font("square.ttf", 18, 0);
	font24 = al_load_font("visitor1.ttf", 24, 0);
	font48 = al_load_font("visitor1.ttf", 48, 0);
	font32 = al_load_font("visitor1.ttf", 32, 0);
	al_reserve_samples(20);

	//Background Init
	bgImage = al_load_bitmap("forest.png");
	battleImage = al_load_bitmap("Grassyland.png");
	bg = new Background(bgImage, 1.8, 1.8, 0);
	objects.push_back(bg);
	
	//Background2 Init
	bg2Image = al_load_bitmap("Mountain.png");
	battleImage2 = al_load_bitmap("desert.png");
	bg2 = new Background(bg2Image, 1.8, 1.8, 0);
	objects.push_back(bg2);

	//Player Init;
	playerImage = al_load_bitmap("SaraFullSheet.png");
	al_convert_mask_to_alpha(playerImage, al_map_rgb(255, 0, 255));
	player = new Player(playerImage);
	objects.push_back(player);
	
	//Enemy Init;
	enemyImage = al_load_bitmap("wolves.gif");
	al_convert_mask_to_alpha(enemyImage, al_map_rgb(255, 0, 255));
	enemy = new Enemy(enemyImage);
	objects.push_back(enemy);
	
	//BOSS1
	bossImage = al_load_bitmap("deathaphro.png");
	al_convert_mask_to_alpha(bossImage, al_map_rgb(255, 0, 255));
	boss = new Boss(bossImage);
	objects.push_back(boss);

	//BOSS2
	bossImage2 = al_load_bitmap("dragon.png");
	al_convert_mask_to_alpha(bossImage2, al_map_rgb(255, 0, 255));
	boss2 = new Boss(bossImage2);
	objects.push_back(boss2);

	//BOSS3
	bodyImage = al_load_bitmap("body.gif");
	al_convert_mask_to_alpha(bodyImage, al_map_rgb(0, 0, 0));
	boss3 = new Boss(bodyImage);
	objects.push_back(boss3);

	demon = al_load_bitmap("demonhead2.gif");
	al_convert_mask_to_alpha(demon, al_map_rgb(0, 0, 0));
	bossHead = new AnimationEffects(demon);
	bossHead->setAnimationColumns(4);
	bossHead->setmaxFrame(9);
	bossHead->setFrameHeight(92);
	bossHead->setFrameWidth(122);
	bossHead->setChara(true);
	bossHead->setFrameDelay(6);
	bossHead->setX(235);
	bossHead->setY(215);
	objects.push_back(bossHead);



	armsImage = al_load_bitmap("arms.png");
	al_convert_mask_to_alpha(armsImage, al_map_rgb(0, 0, 0));
	arms = new AnimationEffects(armsImage);
	arms->setAnimationColumns(1);
	arms->setmaxFrame(1);
	arms->setFrameHeight(139);
	arms->setFrameWidth(170);
	arms->setChara(true);
	arms->setFrameDelay(12);
	arms->setX(245);
	arms->setY(300);
	objects.push_back(arms);


	fistImage = al_load_bitmap("fist.png");
	fist = new AnimationEffects(fistImage);
	fist->setAnimationColumns(1);
	fist->setmaxFrame(6);
	fist->setChara(true);
	fist->setX(325);
	fist->setY(300);
	objects.push_back(fist);

	teleportImage = al_load_bitmap("teleport.png");
	al_convert_mask_to_alpha(teleportImage, al_map_rgb(0, 0, 0));
	teleport = new AnimationEffects(teleportImage);
	teleport->setAnimationColumns(10);
	teleport->setmaxFrame(10);
	teleport->setFrameDelay(3);
	teleport->setX(300);
	teleport->setY(300);
	objects.push_back(teleport);

	curseImage = al_load_bitmap("curse.png");
	curse = new AnimationEffects(curseImage);
	curse->setAnimationColumns(8);
	curse->setmaxFrame(8);
	curse->setX(325);
	curse->setY(300);
	objects.push_back(curse);

	deathImage = al_load_bitmap("deathmark.png");
	death = new AnimationEffects(deathImage);
	death->setAnimationColumns(8);
	death->setmaxFrame(8);
	death->setX(640);
	death->setY(380);
	death->setChara(true);
	death->setFrameDelay(3);
	objects.push_back(death);

	//TEST
	sayakaRun = al_load_bitmap("sayaka.gif");
	al_convert_mask_to_alpha(sayakaRun, al_map_rgb(0, 255, 38));
	sayaka = new AnimationEffects(sayakaRun);
	sayaka->setAnimationColumns(8);
	sayaka->setmaxFrame(8);
	sayaka->setFrameHeight(167);
	sayaka->setFrameWidth(94);
	sayaka->setChara(false);
	sayaka->setFrameDelay(6);
	sayaka->setX(300);
	sayaka->setY(300);
	objects.push_back(sayaka);

	//Slash
	slashImage = al_load_bitmap("slash.png");
	slash = new AnimationEffects(slashImage);
	slash->setAnimationColumns(5);
	slash->setmaxFrame(5);
	objects.push_back(slash);


	enemySlash = new AnimationEffects(slashImage);
	enemySlash->setX(WIDTH + 20);
	enemySlash->setY(HEIGHT - 140 );
	enemySlash->setmaxFrame(5);
	enemySlash->setAnimationColumns(5);
	objects.push_back(enemySlash);
	
	//Lightning
	lightningImage = al_load_bitmap("lightning.png");
	lightning = new AnimationEffects(lightningImage);
	objects.push_back(lightning);

	meteorImage = al_load_bitmap("meteor.png");
	meteor = new AnimationEffects(meteorImage);
	meteor->setX(640);
	meteor->setFrameDelay(6);
	meteor->setChara(true);
	meteor->setY(50);
	objects.push_back(meteor);

	cureImage = al_load_bitmap("cure.png");
	cure = new AnimationEffects(cureImage);
	cure->setX(WIDTH);
	cure->setY(HEIGHT - 160);
	objects.push_back(cure);

	shellImage = al_load_bitmap("barrier.png");
	shell = new AnimationEffects(shellImage);
	shell->setX(WIDTH);
	shell->setY(HEIGHT - 160);
	shell->setFrameDelay(3);
	shell->setmaxFrame(15);
	shell->setAnimationColumns(5);
	objects.push_back(shell);

	hellfireImage = al_load_bitmap("hellfire.png");
	hellfire = new AnimationEffects(hellfireImage);
	hellfire->setAnimationColumns(1);
	hellfire->setAnimationRows(1);
	hellfire->setFrameWidth(640);
	hellfire->setFrameHeight(240);
	hellfire->setmaxFrame(7);
	hellfire->setCurrentFrame(1);
	hellfire->setX(640);
	hellfire->setY(350);
	hellfire->setFrameDelay(7);
	objects.push_back(hellfire);

	tsunamiImage = al_load_bitmap("tsunami.png");
	tsunami = new AnimationEffects(tsunamiImage);
	tsunami->setAnimationColumns(1);
	tsunami->setFrameWidth(640);
	tsunami->setFrameHeight(240);
	tsunami->setmaxFrame(7);
	tsunami->setX(640);
	tsunami->setY(400);
	tsunami->setFrameDelay(7);
	objects.push_back(tsunami);

	breathe = al_load_bitmap("firebreath.png");
	firebreath = new AnimationEffects(breathe);
	firebreath->setY(560);
	firebreath->setFrameDelay(6);
	firebreath->setmaxFrame(8);
	firebreath->setAnimationColumns(8);
	firebreath->setFrameHeight(640);
	firebreath->setX(bosspos2);
	objects.push_back(firebreath);

	bossCure = new AnimationEffects(cureImage);
	objects.push_back(bossCure);

	//Protect
	protectImage = al_load_bitmap("protect.png");
	protect = new AnimationEffects(protectImage);
	protect->setX(WIDTH);
	protect->setY(HEIGHT - 160);
	protect->setFrameDelay(3);
	protect->setmaxFrame(15);
	protect->setAnimationColumns(5);
	objects.push_back(protect);

	//Brave
	braveImage = al_load_bitmap("brave.png");
	brave = new AnimationEffects(braveImage);
	brave->setX(WIDTH + 20);
	brave->setY(HEIGHT - 100);
	brave->setFrameDelay(3);
	brave->setmaxFrame(15);
	brave->setAnimationColumns(5);
	objects.push_back(brave);
	
	//Fire
	fireImage = al_load_bitmap("fiyaa.png");
	fire = new AnimationEffects(fireImage);
	objects.push_back(fire);

	//Ice
	iceImage = al_load_bitmap("ice.png");
	ice = new AnimationEffects(iceImage);
	ice->setX(WIDTH / 2 - 70);
	ice->setY(HEIGHT / 2 + 100);
	objects.push_back(ice);

	enemyIce = new AnimationEffects(iceImage);
	objects.push_back(enemyIce);

	//Wind
	windImage = al_load_bitmap("wind.png");
	wind = new AnimationEffects(windImage);
	objects.push_back(wind);

	//Defense
	defenseImage = al_load_bitmap("defend.png");
	defensive = new AnimationEffects(defenseImage);
	defensive->setX(WIDTH - 10);
	defensive->setY(HEIGHT - 100);
	defensive->setmaxFrame(10);
	defensive->setAnimationColumns(5);
	objects.push_back(defensive);

	squidbomb = al_load_bitmap("squidsplosion.png");
	squidsplosion = new AnimationEffects(squidbomb);
	squidsplosion->setAnimationColumns(1);
	squidsplosion->setFrameWidth(640);
	squidsplosion->setFrameHeight(240);
	squidsplosion->setmaxFrame(7);
	squidsplosion->setX(840);
	squidsplosion->setY(400);
	squidsplosion->setFrameDelay(7);
	objects.push_back(squidsplosion);

	//Inventory Init
	inv.init();
	
	//Screen bitmap
	titleImage1 = al_load_bitmap("titlescreen2.png");
	titleImage2 = al_load_bitmap("titlescreen.png");
	lostImage = al_load_bitmap("gameover_new.png");
	menuImage = al_load_bitmap("mainmenu.png");
	battleCommand = al_load_bitmap("battlecommandwhite2.png");
	statusImage = al_load_bitmap("statusscreenhime.png");
	optionImage = al_load_bitmap("optiondefault.png");
	saveImage = al_load_bitmap("Savescreen.png");
	loadImage = al_load_bitmap("loadscreen.png");
	magicImage = al_load_bitmap("magicscreendefault.png");
	equipmentImage = al_load_bitmap("EquipmentMenuDefault.png");
	itemImage = al_load_bitmap("ItemMenu.png");
	victoryImage = al_load_bitmap("victoryscreen.png");
	magicCommand = al_load_bitmap("battlecommanddefault2.png");
	slotImage = al_load_bitmap("magicBox.png");
	worldImage = al_load_bitmap("worldmap.png");
	shopImage = al_load_bitmap("shopbackground.png");
	shopControlImage = al_load_bitmap("shopmenu.png");
	buysellEquipmentImage = al_load_bitmap("ShopScreen2.png");
	buysellItemImage = al_load_bitmap("ShopScreen1.png");
	gambleImage = al_load_bitmap("gamble.png");
	oneMinute = al_load_bitmap("1minuteshikanai.png");
	square = al_load_bitmap("square.png");
	bossbg = al_load_bitmap("boss.png");
	bossbg2 = al_load_bitmap("bossbg2.png");
	hime = al_load_bitmap("castle2hime.png");
	himeSurprised = al_load_bitmap("castle2himesurprise.png");
	dialogue = al_load_bitmap("conversationscreenhimedefault.png");
	fairydialogue = al_load_bitmap("conversationscreenfairy.png");
	aboutMenu = al_load_bitmap("about.png");
	ending = al_load_bitmap("Ending2.png");
	dialogueLB = al_load_bitmap("conversationscreenlb.png");

	//Fairy
	fairy = al_load_bitmap("fairy.png");
	al_convert_mask_to_alpha(fairy, al_map_rgb(255, 255, 255));

	demon1 = al_load_bitmap("demon1.gif");
	al_convert_mask_to_alpha(demon1, al_map_rgb(0, 0, 0));

	//Player Arrow 
	playerArrow = al_load_bitmap("himefront.png");
	al_convert_mask_to_alpha(playerArrow, al_map_rgb(255, 255, 255));
	
	//On option
	optionOn = al_load_bitmap("on.png");
	al_convert_mask_to_alpha(optionOn, al_map_rgb(255, 255, 255));

	//Off option
	optionOff = al_load_bitmap("off.png");
	al_convert_mask_to_alpha(optionOff, al_map_rgb(255, 255, 255));

	al_convert_mask_to_alpha(oneMinute, al_map_rgb(255, 255, 255));

	//Arrow Button
	arrow = al_load_bitmap("arrow1.png");

	//Background Init
	titleScreen1 = new Background(titleImage1, 0, 0, 1);
	titleScreen2 = new Background(titleImage2, 0, 0, 2);
	lostScreen = new Background(lostImage, 0, 0, 2);
	battleScreen = new Background(battleImage, 0, 0, 0);
	battleScreen2 = new Background(battleImage2, 0, 0, 0);
	menuScreen = new Background(menuImage, 0, 0, 7);
	battleControl = new Background(battleCommand, 0, 0, 3);
	statusScreen = new Background(statusImage, 0, 0, 0);
	optionScreen = new Background(optionImage, 0, 0, 2);
	saveScreen = new Background(saveImage, 0, 0, 2);
	loadScreen = new Background(loadImage, 0, 0, 2);
	magicScreen = new Background(magicImage, 0, 0, 9);
	equipmentScreen = new Background(equipmentImage, 0, 0, 35);
	itemScreen = new Background(itemImage, 0, 0, 25);
	victoryScreen = new Background(victoryImage, 0, 0, 0);
	magicControl = new Background(magicCommand, 0, 0, 3);
	magicSlots = new Background(slotImage, 0, 0, 3);
	worldScreen = new Background(worldImage, 0, 0, 2);
	buysellEquipment = new Background(buysellEquipmentImage, 0, 0, 11);
	buysellItem = new Background(buysellItemImage, 0, 0, 3);
	buy = new Background(buysellItemImage, 0, 0, 1);
	sell = new Background(buysellItemImage, 0, 0, 1);
	shopControl = new Background(shopControlImage, 0, 0, 3);
	shopMenu = new Background(shopImage, 0, 0, 0);
	gambleScreen = new Background(gambleImage, 0, 0, 0);
	bossScreen = new Background(bossbg, 0, 0, 0);
	bossScreen2 = new Background(bossbg2, 0, 0, 0);
	conversation = new Background(hime, 0, 0, 0);
	about = new Background(aboutMenu, 0, 0, 0);
	credits = new Background(ending, 0, 0, 0);
	shopControl->setX(350);
	shopControl->setY(180);


	//Magic Initialize
	master[0].init(10, 10, 0, 0, false, false, "Fire", "Shoots fire, man.", fire);
	master[1].init(10, 10, 0, 0, false, false, "Lightning", "Lightning from your fingertips.", lightning);
	master[2].init(10, 10, 0, 0, false, false, "Ice", "Freeze the air, do some damage", ice);
	master[3].init(10, 10, 0, 0, false, false, "Wind", "Make it slightly uncomfortably gusty.", wind);
	master[4].init(10, 10, 0, 0, true, false, "Cure", "Heal yourself, because magic.", cure);
	master[5].init(20, 10, 0, 0, false, true, "Brave", "Give yourself bravery, n' stuff", brave);
	master[6].init(20, 0, 10, 0, false, true, "Protect", "Win at shielding yourself.", protect);
	master[7].init(20, 0, 0, 10, false, true, "Shell", "Magic ain't got nothin' on you.", shell);

	//Title Selection
	titleScreen1->setSelect(230, 305, 0);
	titleScreen1->setSelect(230, 335, 1);
	titleScreen2->setSelect(220, 295, 0);
	titleScreen2->setSelect(220, 325, 1);
	titleScreen2->setSelect(220, 355, 2);


	//World Selection
	worldScreen->setSelect(350, 250, 0);
	worldScreen->setSelect(480, 200, 1);
	worldScreen->setSelect(460, 120, 2);

	//Shop Selection
	shopControl->setSelect(350, 200, 0);
	shopControl->setSelect(350, 230, 1);
	shopControl->setSelect(350, 265, 2);
	shopControl->setSelect(350, 300, 3);



	//Main Menu Selection
	menuScreen->setSelect(0, 25, 0);
	menuScreen->setSelect(0, 55, 1);
	menuScreen->setSelect(0, 85, 2);
	menuScreen->setSelect(0, 110, 3);
	menuScreen->setSelect(0, 140, 4);
	menuScreen->setSelect(0, 170, 5);
	menuScreen->setSelect(0, 200, 6);
	menuScreen->setSelect(0, 255, 7);

	//Battle Selection
	battleControl->setY(329);
	battleControl->setSelect(110, 345, 0);
	battleControl->setSelect(110, 370, 1);
	battleControl->setSelect(110, 400, 2);
	battleControl->setSelect(110, 430, 3);

	//Magic Selection
	magicControl->setY(329);
	magicControl->setSelect(110, 345, 0);
	magicControl->setSelect(110, 370, 1);
	magicControl->setSelect(110, 400, 2);
	magicControl->setSelect(110, 430, 3);

	//Game Over Selection
	lostScreen->setSelect(220, 300, 0);
	lostScreen->setSelect(220, 330, 1);
	lostScreen->setSelect(220, 360, 2);

	//Equipment Selection
	equipmentScreen->setSelect(20, 20, 0);
	equipmentScreen->setSelect(200, 20, 1);
	equipmentScreen->setSelect(20, 40, 2);
	equipmentScreen->setSelect(200, 40, 3);
	equipmentScreen->setSelect(20, 60, 4);
	equipmentScreen->setSelect(200, 60, 5);
	equipmentScreen->setSelect(20, 80, 6);
	equipmentScreen->setSelect(200, 80, 7);
	equipmentScreen->setSelect(20, 100, 8);
	equipmentScreen->setSelect(200, 100, 9);
	equipmentScreen->setSelect(20, 120, 10);
	equipmentScreen->setSelect(200, 120,11);
	equipmentScreen->setSelect(20, 140, 12);
	equipmentScreen->setSelect(200, 140, 13);
	equipmentScreen->setSelect(20, 160, 14);
	equipmentScreen->setSelect(200, 160, 15);
	equipmentScreen->setSelect(20, 180, 16);
	equipmentScreen->setSelect(200, 180, 17);
	equipmentScreen->setSelect(20, 200, 18);
	equipmentScreen->setSelect(200, 200, 19);
	equipmentScreen->setSelect(20, 220, 20);
	equipmentScreen->setSelect(200, 220, 21);
	equipmentScreen->setSelect(20, 240, 22);
	equipmentScreen->setSelect(200, 240, 23);
	equipmentScreen->setSelect(20, 260, 24);
	equipmentScreen->setSelect(200, 260, 25);
	equipmentScreen->setSelect(20, 280, 26);
	equipmentScreen->setSelect(200, 280, 27);
	equipmentScreen->setSelect(20, 300, 28);
	equipmentScreen->setSelect(200, 300, 29);
	equipmentScreen->setSelect(20, 320, 30);
	equipmentScreen->setSelect(200, 320, 31);
	equipmentScreen->setSelect(20, 340, 32);
	equipmentScreen->setSelect(200, 340, 33);
	equipmentScreen->setSelect(20, 360, 34);
	equipmentScreen->setSelect(200, 360, 35);

	//Item Selection
	itemScreen->setSelect(20, 80, 0);
	itemScreen->setSelect(320, 80, 1);
	itemScreen->setSelect(20, 100, 2);
	itemScreen->setSelect(320, 100, 3);
	itemScreen->setSelect(20, 120, 4);
	itemScreen->setSelect(320, 120, 5);
	itemScreen->setSelect(20, 140, 6);
	itemScreen->setSelect(320, 140, 7);
	itemScreen->setSelect(20, 160, 8);
	itemScreen->setSelect(320, 160, 9);
	itemScreen->setSelect(20, 180, 10);
	itemScreen->setSelect(320, 180, 11);
	itemScreen->setSelect(20, 200, 12);
	itemScreen->setSelect(320, 200, 13);
	itemScreen->setSelect(20, 220, 14);
	itemScreen->setSelect(320, 220, 15);
	itemScreen->setSelect(20, 240, 16);
	itemScreen->setSelect(320, 240, 17);
	itemScreen->setSelect(20, 260, 18);
	itemScreen->setSelect(320, 260, 19);
	itemScreen->setSelect(20, 280, 20);
	itemScreen->setSelect(320, 280, 21);
	itemScreen->setSelect(20, 300, 22);
	itemScreen->setSelect(320, 300, 23);
	itemScreen->setSelect(20, 320, 24);
	itemScreen->setSelect(320, 320, 25);

	//Option Selection
	optionScreen->setSelect(140, 170, 0);
	optionScreen->setSelect(140, 269, 1);
	optionScreen->setSelect(140, 367, 2);

	//Save Selection
	saveScreen->setSelect(10, 95, 0);
	saveScreen->setSelect(10, 225, 1);
	saveScreen->setSelect(10, 355, 2);

	//Load Selection
	loadScreen->setSelect(10, 95, 0);
	loadScreen->setSelect(10, 225, 1);
	loadScreen->setSelect(10, 355, 2);

	//Magic Selection
	magicScreen->setSelect(10, 150, 0);
	magicScreen->setSelect(310, 150, 1);
	magicScreen->setSelect(10, 195, 2);
	magicScreen->setSelect(310, 195, 3);
	magicScreen->setSelect(10, 240, 4);
	magicScreen->setSelect(310, 240, 5);
	magicScreen->setSelect(10, 285, 6);
	magicScreen->setSelect(310, 285, 7);
	magicScreen->setSelect(10, 330, 8);
	magicScreen->setSelect(310, 330, 9);

	//Slot Selection
	magicSlots->setY(320);
	magicSlots->setX(300);
	magicSlots->setSelect(320, 335, 0);
	magicSlots->setSelect(320, 370, 1);
	magicSlots->setSelect(320, 405, 2);
	magicSlots->setSelect(320, 440, 3);

	//buy selection
	buy->setSelect(0, 0, 0);
	buy->setSelect(0, 0, 1);

	//sell selection
	sell->setSelect(0, 0, 0);
	sell->setSelect(0, 0, 1);

	//Buy Sell Item Selection
	buysellItem->setSelect(10, 140, 0);
	buysellItem->setSelect(10, 160, 1);
	buysellItem->setSelect(10, 180, 2);
	buysellItem->setSelect(10, 200, 3);

	//Buy Sell Equipment Selection
	buysellEquipment->setSelect(10, 140, 0);
	buysellEquipment->setSelect(10, 160, 1);
	buysellEquipment->setSelect(10, 180, 2);
	buysellEquipment->setSelect(10, 200, 3);
	buysellEquipment->setSelect(10, 220, 4);
	buysellEquipment->setSelect(10, 240, 5);
	buysellEquipment->setSelect(10, 260, 6);
	buysellEquipment->setSelect(10, 280, 7);
	buysellEquipment->setSelect(10, 300, 8);
	buysellEquipment->setSelect(10, 320, 9);
	buysellEquipment->setSelect(10, 340, 10);
	buysellEquipment->setSelect(10, 360, 11);

	//Load song samples
	song = al_load_sample("levelone.ogg");
	battleSong = al_load_sample("battle.ogg");
	themeSong = al_load_sample("themesong.ogg");
	gameOver = al_load_sample("gameover.ogg");
	victorySong = al_load_sample("victorysong.ogg");
	saveloadSong = al_load_sample("saveloadsound.ogg");
	attackSound = al_load_sample("hittingsound2.ogg");
	fleeSound = al_load_sample("flee.ogg");
	fireSound = al_load_sample("fire.ogg");
	selectSound = al_load_sample("click.ogg");
	iceSound = al_load_sample("ice.ogg");
	thunderSound = al_load_sample("thunder.ogg");
	worldSound = al_load_sample("worldmap.ogg");
	shopSound = al_load_sample("shopsound.ogg");
	selectSound = al_load_sample("selectsound.ogg");
	healSound = al_load_sample("heal.ogg");
	barrierSound = al_load_sample("barrier.ogg");
	buySellSound = al_load_sample("buystuffsound.ogg");
	squidWalk = al_load_sample("squidwalk.ogg");
	squidAttack = al_load_sample("squidattack.ogg");
	squidBoss = al_load_sample("squidboss.ogg");
	dragonSound = al_load_sample("boss2.ogg");
	leveltwo = al_load_sample("leveltwo.ogg");
	windSound = al_load_sample("wind.ogg");
	conversationSound = al_load_sample("conversationsong.ogg");
	tsunamiSound = al_load_sample("tsunami.ogg");
	dragonRoar = al_load_sample("dragonroar.ogg");
	demonRoar = al_load_sample("demonroar.ogg");
	lastBoss = al_load_sample("lastboss.ogg");
	fireFall = al_load_sample("firefall.ogg");
	clockSound = al_load_sample("clocksound.ogg");
	ramSound = al_load_sample("hittingsound3.ogg");

	//Song to mixer
	conversationInstance = al_create_sample_instance(conversationSound);
	al_set_sample_instance_playmode(conversationInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(conversationInstance, al_get_default_mixer());

	clockInstance = al_create_sample_instance(clockSound);
	al_set_sample_instance_playmode(clockInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(clockInstance, al_get_default_mixer());

	songInstance = al_create_sample_instance(song);
	al_set_sample_instance_playmode(songInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(songInstance, al_get_default_mixer());

	leveltwoInstance = al_create_sample_instance(leveltwo);
	al_set_sample_instance_playmode(leveltwoInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(leveltwoInstance, al_get_default_mixer());

	themeInstance = al_create_sample_instance(themeSong);
	al_set_sample_instance_playmode(themeInstance, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(themeInstance, .8);
	al_attach_sample_instance_to_mixer(themeInstance, al_get_default_mixer());

	battleInstance = al_create_sample_instance(battleSong);
	al_set_sample_instance_playmode(battleInstance, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(battleInstance, .5);
	al_attach_sample_instance_to_mixer(battleInstance, al_get_default_mixer());

	gameOverInstance = al_create_sample_instance(gameOver);
	al_set_sample_instance_playmode(gameOverInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(gameOverInstance, al_get_default_mixer());

	selectInstance = al_create_sample_instance(selectSound);
	al_set_sample_instance_playmode(selectInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(selectInstance, al_get_default_mixer());

	victoryInstance = al_create_sample_instance(victorySong);
	al_set_sample_instance_playmode(victoryInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(victoryInstance, al_get_default_mixer());

	saveloadInstance = al_create_sample_instance(saveloadSong);
	al_set_sample_instance_playmode(saveloadInstance, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(saveloadInstance, .6);
	al_attach_sample_instance_to_mixer(saveloadInstance, al_get_default_mixer());

	attackInstance = al_create_sample_instance(attackSound);
	al_set_sample_instance_playmode(attackInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(attackInstance, al_get_default_mixer());

	fleeInstance = al_create_sample_instance(fleeSound);
	al_set_sample_instance_playmode(fleeInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(fleeInstance, al_get_default_mixer());

	fireInstance = al_create_sample_instance(fireSound);
	al_set_sample_instance_playmode(fireInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(fireInstance, al_get_default_mixer());

	iceInstance = al_create_sample_instance(iceSound);
	al_set_sample_instance_playmode(iceInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(iceInstance, al_get_default_mixer());

	thunderInstance = al_create_sample_instance(thunderSound);
	al_set_sample_instance_playmode(thunderInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(thunderInstance, al_get_default_mixer());

	worldInstance = al_create_sample_instance(worldSound);
	al_set_sample_instance_playmode(worldInstance, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(worldInstance, .6);
	al_attach_sample_instance_to_mixer(worldInstance, al_get_default_mixer());

	shopInstance = al_create_sample_instance(shopSound);
	al_set_sample_instance_playmode(shopInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(shopInstance, al_get_default_mixer());

	menuInstance = al_create_sample_instance(selectSound);
	al_set_sample_instance_playmode(menuInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(menuInstance, al_get_default_mixer());

	barrierInstance = al_create_sample_instance(barrierSound);
	al_set_sample_instance_playmode(barrierInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(barrierInstance, al_get_default_mixer());

	healInstance = al_create_sample_instance(healSound);
	al_set_sample_instance_playmode(healInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(healInstance, al_get_default_mixer());

	windInstance = al_create_sample_instance(windSound);
	al_set_sample_instance_playmode(windInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(windInstance, al_get_default_mixer());

	buySellInstance = al_create_sample_instance(buySellSound);
	al_set_sample_instance_playmode(buySellInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(buySellInstance, al_get_default_mixer());

	squidwalkInstance = al_create_sample_instance(squidWalk);
	al_set_sample_instance_playmode(squidwalkInstance, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(squidwalkInstance, .4);
	al_attach_sample_instance_to_mixer(squidwalkInstance, al_get_default_mixer());

	squidAttackInstance = al_create_sample_instance(squidAttack);
	al_set_sample_instance_playmode(squidAttackInstance, ALLEGRO_PLAYMODE_ONCE);
	al_set_sample_instance_gain(squidAttackInstance, .6);
	al_attach_sample_instance_to_mixer(squidAttackInstance, al_get_default_mixer());

	squidbossInstance = al_create_sample_instance(squidBoss);
	al_set_sample_instance_playmode(squidbossInstance, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(squidbossInstance, .7);
	al_attach_sample_instance_to_mixer(squidbossInstance, al_get_default_mixer());

	dragonInstance = al_create_sample_instance(dragonSound);
	al_set_sample_instance_playmode(dragonInstance, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(dragonInstance, al_get_default_mixer());

	tsunamiInstance = al_create_sample_instance(tsunamiSound);
	al_set_sample_instance_playmode(tsunamiInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(tsunamiInstance, al_get_default_mixer());

	lastBossInstance = al_create_sample_instance(lastBoss);
	al_set_sample_instance_playmode(lastBossInstance, ALLEGRO_PLAYMODE_LOOP);
	al_set_sample_instance_gain(lastBossInstance, .7);
	al_attach_sample_instance_to_mixer(lastBossInstance, al_get_default_mixer());


	demonRoarInstance = al_create_sample_instance(demonRoar);
	al_set_sample_instance_playmode(demonRoarInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(demonRoarInstance, al_get_default_mixer());

	dragonRoarInstance = al_create_sample_instance(dragonRoar);
	al_set_sample_instance_playmode(dragonRoarInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(dragonRoarInstance, al_get_default_mixer());

	firefallInstance = al_create_sample_instance(fireFall);
	al_set_sample_instance_playmode(firefallInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(firefallInstance, al_get_default_mixer());

	hittingInstance = al_create_sample_instance(ramSound);
	al_set_sample_instance_playmode(hittingInstance, ALLEGRO_PLAYMODE_ONCE);
	al_attach_sample_instance_to_mixer(hittingInstance, al_get_default_mixer());

	//Title Menu Game Start
	changeState(state, TITLE1);

	//RANDOM
	srand(time(NULL));

	//Timer and StartUp
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);
	gameTimer = al_create_timer(1.0 / 60);
	elapsed = al_create_timer(1.0 / 60);
	battleTimer = al_create_timer(1.0 / 60);

	//Register events
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//Game Timers
	al_start_timer(timer);
	gameTime = al_current_time();
	int secondCount = 0;
	int minuteCount = 0;
	int hourCount = 0;
	int battleSecond = 0;

	while (!done)
	{
		//Event Queue
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = true;

				if (state == TITLE1)
				{
					titleScreen1->selectDecrease();
				}
				else if (state == TITLE2)
				{
					titleScreen2->selectDecrease();
				}
				else if (state == WORLD)
				{
				}
				else if (state == SHOP)
				{
					shopControl->selectDecrease();
				}
				else if (state == GAMBLE)
				{
					if (gambleInput < player->getMoney())
					{
						gambleInput += 100;
					}
				}
				else if (state == BUY)
				{
					if (buy->getSelect() == 0)
					{
						buysellItem->selectDecrease();
					}
					else
					{
						buysellEquipment->selectDecrease();
					}
				}
				else if (state == SELL)
				{
					if (sell->getSelect() == 0)
					{
						buysellItem->selectDecrease();
					}
					else
					{
						buysellEquipment->selectDecrease();
					}
				}
				else if (state == MAIN)
				{
					menuScreen->selectDecrease();
				}
				else if (state == WORLD_MAIN)
				{
					menuScreen->selectDecrease();
				}
				else if (state == EQUIP)
				{
					equipmentScreen->selectDecrease();
					equipmentScreen->selectDecrease();
				}
				else if (state == ITEM)
				{
					itemScreen->selectDecrease();
					itemScreen->selectDecrease();
				}
				else if (state == OPTION)
				{
					optionScreen->selectDecrease();
				}
				else if (state == CONFIG)
				{
					optionScreen->selectDecrease();
				}
				else if (state == SAVE)
				{
					saveScreen->selectDecrease();
				}
				else if (state == LOAD1)
				{
					loadScreen->selectDecrease();
				}
				else if (state == LOAD2)
				{
					loadScreen->selectDecrease();
				}
				else if (state == MAGIC)
				{
					if (keys[SPACE] == false)
					{
						magicScreen->selectDecrease();
						magicScreen->selectDecrease();
					}

					if (keys[SPACE] == true)
					{
						magicSlots->selectDecrease();
					}
				}
				else if (state == OPTION)
				{
					optionScreen->selectDecrease();
				}
				else if (state == PLAYING)
				{
					if (keys[UP])
					{
						player->moveUp();
					}
				}
				else if (state == LEVEL2)
				{
					if (keys[UP])
					{
						player->moveUp();
					}
				}
				else if (state == BATTLE)
				{
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectDecrease();
					}
					else
					{
						battleControl->selectDecrease();
					}
				}
				else if (state == BATTLE2)
				{
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectDecrease();
					}
					else
					{
						battleControl->selectDecrease();
					}
				}
				else if (state == BOSS)
				{
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectDecrease();
					}
					else
					{
						battleControl->selectDecrease();
					}
				}
				else if (state == BOSS2)
				{
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectDecrease();
					}
					else
					{
						battleControl->selectDecrease();
					}
				}
				else if (state == BOSS3)
				{
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectDecrease();
					}
					else
					{
						battleControl->selectDecrease();
					}
				}
				else if (state == LOST)
				{
					lostScreen->selectDecrease();
				}
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;

				if (state == TITLE1)
				{
					titleScreen1->selectIncrease();
				}
				else if (state == TITLE2)
				{
					titleScreen2->selectIncrease();
				}
				else if (state == WORLD)
				{
				}
				else if (state == SHOP)
				{
					shopControl->selectIncrease();
				}
				else if (state == BUY)
				{
					if (buy->getSelect() == 0)
					{
						buysellItem->selectIncrease();
					}
					else
					{
						buysellEquipment->selectIncrease();
					}
				}
				else if (state == SELL)
				{
					if (sell->getSelect() == 0)
					{
						buysellItem->selectIncrease();
					}
					else
					{
						buysellEquipment->selectIncrease();
					}
				}
				else if (state == GAMBLE)
				{
					if (gambleInput > 0)
					{
						gambleInput -= 100;
					}
					else if (gambleInput < 0)
					{
						gambleInput = 0;
					}
				}
				else if (state == MAIN)
				{
					menuScreen->selectIncrease();
				}
				else if (state == WORLD_MAIN)
				{
					menuScreen->selectIncrease();
				}
				else if (state == LOST)
				{
					lostScreen->selectIncrease();
				}
				else if (state == OPTION)
				{
					optionScreen->selectIncrease();
				}
				else if (state == EQUIP)
				{
					equipmentScreen->selectIncrease();
					equipmentScreen->selectIncrease();
				}
				else if (state == ITEM)
				{
					itemScreen->selectIncrease();
					itemScreen->selectIncrease();
				}
				else if (state == OPTION)
				{
					optionScreen->selectIncrease();
				}
				else if (state == CONFIG)
				{
					optionScreen->selectIncrease();
				}
				else if (state == SAVE)
				{
					saveScreen->selectIncrease();
				}
				else if (state == LOAD1)
				{
					loadScreen->selectIncrease();
				}
				else if (state == LOAD2)
				{
					loadScreen->selectIncrease();
				}
				else if (state == MAGIC)
				{
					if (keys[SPACE] == false)
					{
						magicScreen->selectIncrease();
						magicScreen->selectIncrease();
					}
					if (keys[SPACE] == true)
					{
						magicSlots->selectIncrease();
					}
				}
				else if (state == PLAYING)
				{
					if (keys[DOWN])
					{
						player->moveDown();
					}
				}
				else if (state == LEVEL2)
				{
					if (keys[DOWN])
					{
						player->moveDown();
					}
				}
				else if (state == BATTLE)
				{	
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectIncrease();
					}
					else
					{
						battleControl->selectIncrease();
					}
				}
				else if (state == BATTLE2)
				{
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectIncrease();
					}
					else
					{
						battleControl->selectIncrease();
					}
				}
				else if (state == BOSS)
				{
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectIncrease();
					}
					else
					{
						battleControl->selectIncrease();
					}
				}
				else if (state == BOSS2)
				{
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectIncrease();
					}
					else
					{
						battleControl->selectIncrease();
					}
				}
				else if (state == BOSS3)
				{
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectIncrease();
					}
					else
					{
						battleControl->selectIncrease();
					}
				}
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				if (state == WORLD)
				{
					if (worldScreen->getSelect() != 0)
					{
						worldScreen->selectDecrease();
					}
				}
				else if (state == BUY)
				{
					buy->selectDecrease();
				}
				else if (state == SELL)
				{
					sell->selectDecrease();
				}
				else if (state == PLAYING)
				{
					if (keys[LEFT])
					{
						player->moveLeft();
					}
				}
				else if (state == LEVEL2)
				{
					if (keys[LEFT])
					{
						player->moveLeft();
					}
				}
				else if (state == EQUIP)
				{
					equipmentScreen->selectDecrease();
				}
				else if (state == ITEM)
				{
					itemScreen->selectDecrease();
				}
				else if (state == MAGIC)
				{
					if (keys[SPACE] == false)
					{
						magicScreen->selectDecrease();
					}
				}
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				if (state == WORLD)
				{
					if (worldScreen->getSelect() != 2)
					{
						if (worldScreen->getSelect() != 1 || level2)
						{
							worldScreen->selectIncrease();
						}
					}
				}
				else if (state == BUY)
				{
					buy->selectIncrease();
				}
				else if (state == SELL)
				{
					sell->selectIncrease();
				}
				else if (state == PLAYING)
				{
					if (keys[RIGHT])
					{
						player->moveRight();
					}
				}
				else if (state == LEVEL2)
				{
					if (keys[RIGHT])
					{
						player->moveRight();
					}
				}
				else if (state == EQUIP)
				{
					equipmentScreen->selectIncrease();
				}
				else if (state == ITEM)
				{
					itemScreen->selectIncrease();
				}
				else if (state == MAGIC)
				{
					if (keys[SPACE] == false)
					{
						magicScreen->selectIncrease();
					}
				}
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = !keys[SPACE];

				if (state == TITLE1)
				{
					switch (titleScreen1->getSelect())
					{
					case 0:
						changeState(state, TITLE2);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 1:
						changeState(state, OPTION);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					}
				}
				else if (state == TITLE2)
				{
					switch (titleScreen2->getSelect())
					{
					case 0:
						conversationCount = 0;
						changeState(state, CONVERSATION);
						currentMag[0] = master[8];
						currentMag[1] = master[8];
						currentMag[2] = master[8];
						currentMag[3] = master[8];
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 1:
						changeState(state, LOAD1);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 2:
						changeState(state, TITLE1);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					}
				}
				else if (state == CONVERSATION)
				{
					conversationCount++;
					if (options[SFX])
					{
						al_play_sample_instance(selectInstance);
					}
				}
				else if (state == BOSSTALK)
				{
					bossConversation++;
					if (options[SFX])
					{
						al_play_sample_instance(selectInstance);
					}
				}
				else if (state == END)
				{
					conversationEnd++;
					if (options[SFX])
					{
						al_play_sample_instance(selectInstance);
					}
				}
				else if (state == WORLD)
				{
					switch (worldScreen->getSelect())
					{
					case 0:
						changeState(state, SHOP);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 1:
						bg->setX(-100);
						bg->setY(-100);
						changeState(state, PLAYING);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 2:
						bg2->setX(-100);
						bg2->setY(-100);
						changeState(state, LEVEL2);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					}
				}
				else if (state == BUY)
				{
					if (buy->getSelect() == 0)
					{
						switch (buysellItem->getSelect())
						{
						case 0:
							if (player->getMoney() - inv.getValueTotal(0) >= 0)
							{
								inv.addItem(0);
								if (options[SFX])
								{
									al_play_sample_instance(buySellInstance);
								}
								player->decreaseMoney(inv.getValueTotal(0));
							}
							break;
						case 1:
							if (player->getMoney() - inv.getValueTotal(1) >= 0)
							{
								inv.addItem(1);
								if (options[SFX])
								{
									al_play_sample_instance(buySellInstance);
								}
								player->decreaseMoney(inv.getValueTotal(1));
							}
							break;
						case 2:
							if (player->getMoney() - inv.getValueTotal(14) >= 0)
							{
								inv.addItem(14);
								if (options[SFX])
								{
									al_play_sample_instance(buySellInstance);
								}
								player->decreaseMoney(inv.getValueTotal(14));
							}
							break;
						case 3:
							if (player->getMoney() - inv.getValueTotal(15) >= 0)
							{
								inv.addItem(15);
								if (options[SFX])
								{
									al_play_sample_instance(buySellInstance);
								}
								player->decreaseMoney(inv.getValueTotal(15));
							}
							break;
						}
					}
					else
					{
						if ((player->getMoney() - inv.getValueTotal(buysellEquipment->getSelect() + 2)) >= 0)
						{
							inv.addItem(buysellEquipment->getSelect() + 2);
							if (options[SFX])
							{
								al_play_sample_instance(buySellInstance);
							}
							player->decreaseMoney(inv.getValueTotal(buysellEquipment->getSelect() + 2));
						}
					}
				}
				else if (state == SELL)
				{
					if (sell->getSelect() == 0)
					{
						int j = 0;
						int i = 0;
						bool found = false;
						while (i < 16 && !found && inv.getQuantity(i) != 0)
						{
							if (!inv.isEquipment(i))
							{
								j++;
							}
							if (j == buysellItem->getSelect() + 1)
							{
								inv.removeItem(i);
								int value = inv.getValueTotal(i) * .6;
								player->addMoney(value);

								found = true;
							}
							i++;
						}
					}
					else if (sell->getSelect() == 1)
					{
						int j = 0;
						int i = 0;
						bool found = false;
						while (i < 16 && !found && inv.getQuantity(i) != 0)
						{
							if (inv.isEquipment(i))
							{
								j++;
							}
							if (j == buysellEquipment->getSelect() + 1)
							{
								inv.removeItem(i);
								int value = inv.getValueTotal(i) * .6;
								player->addMoney(value);

								found = true;
							}
							i++;
						}
					}
						if (options[SFX])
						{
							al_play_sample_instance(buySellInstance);
						}
				}
				else if (state == SHOP)
				{
					switch (shopControl->getSelect())
					{
					case 0:
						changeState(state, BUY);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 1:
						changeState(state, SELL);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 2:
						changeState(state, GAMBLE);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 3:
						al_stop_sample_instance(shopInstance);
						changeState(state, WORLD);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					}
				}
				else if (state == GAMBLE)
				{
					if (player->getMoney() - gambleInput >= 0)
					{
						player->decreaseMoney(gambleInput);

						int n;
						srand(time(NULL));
						if (options[SFX])
						{
							al_play_sample_instance(buySellInstance);
						}
						switch (gambleInput)
						{
						case 100:
							n = rand() % 2;
							if (n == 1)
							{
								inv.addItem(rand() % 16);
							}
							break;
						case 200:
							n = rand() % 3;
							if (n > 0)
							{
								inv.addItem(rand() % 16);
							}
							break;
						case 300:
							n = rand() % 4;
							if (n > 0)
							{
								inv.addItem(rand() % 16);
							}
							break;
						case 400:
							n = rand() % 5;
							if (n > 0)
							{
								inv.addItem(rand() % 16);
							}
							break;
						case 500:
							n = rand() % 6;
							if (n > 0)
							{
								inv.addItem(rand() % 16);
							}
							break;
						case 600:
							n = rand() % 7;
							if (n > 0)
							{
								inv.addItem(rand() % 16);
							}
							break;
						case 700:
							n = rand() % 8;
							if (n > 0)
							{
								inv.addItem(rand() % 16);
							}
							break;
						case 800:
							n = rand() % 9;
							if (n > 0)
							{
								inv.addItem(rand() % 16);
							}
							break;
						case 900:
							n = rand() % 10;
							if (n > 0)
							{
								inv.addItem(rand() % 16);
							}
							break;
						}

						if (gambleInput >= 1000)
						{
							inv.addItem(rand() % 16);
						}

						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
					}
				}
				else if (state == MAIN)
				{
					switch (menuScreen->getSelect())
					{
					case 0:
						changeState(state, ITEM);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 1:
						changeState(state, MAGIC);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						keys[SPACE] = false;
						break;
					case 2:
						changeState(state, EQUIP);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 3:
						changeState(state, STATUS);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 4:
						changeState(state, CONFIG);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 5:
						changeState(state, LOAD2);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 6:
						changeState(state, SAVE);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 7:
						changeState(state, WORLD);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						al_stop_sample_instance(songInstance);
						break;
					}
				}
				else if (state == WORLD_MAIN)
				{
					switch (menuScreen->getSelect())
					{
					case 0:
						changeState(state, ITEM);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 1:
						changeState(state, MAGIC);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						keys[SPACE] = false;
						break;
					case 2:
						changeState(state, EQUIP);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 3:
						changeState(state, STATUS);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 4:
						changeState(state, CONFIG);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 5:
						changeState(state, LOAD2);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 6:
						changeState(state, SAVE);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 7:
						changeState(state, TITLE1);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						al_stop_sample_instance(worldInstance);
						break;
					}
				}
				else if (state == SAVE)
				{
					switch (saveScreen->getSelect())
					{
					case 0:
						save(0);
						checkload(0);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 1:
						save(1);
						checkload(1);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 2:
						save(2);
						checkload(2);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					}
				}
				else if (state == LOAD1)
				{
					switch (loadScreen->getSelect())
					{
					case 0:
						checkload(0);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						if (loadfile1)
						{
							load(0);
							changeState(state, WORLD);
						}
						break;
					case 1:
						checkload(1);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						if (loadfile2)
						{
							load(1);
							changeState(state, WORLD);
						}
						changeState(state, WORLD);
						break;
					case 2:
						checkload(2);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						if (loadfile3)
						{
							load(2);
							changeState(state, WORLD);
						}
						break;
					}
				}
				else if (state == LOAD2)
				{
					switch (loadScreen->getSelect())
					{
					case 0:
						checkload(0);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						if (loadfile1)
						{
							load(0);
							changeState(state, WORLD);
						}
						break;
					case 1:
						checkload(1);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						if (loadfile2)
						{
							load(1);
							changeState(state, WORLD);
						}
						changeState(state, WORLD);
						break;
					case 2:
						checkload(2);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						if (loadfile3)
						{
							load(2);
							changeState(state, WORLD);
						}
						break;
					}
				}
				else if (state == MAGIC)
				{
					if (keys[SPACE] == false)
					{
						currentMag[magicSlots->getSelect()] = master[magicScreen->getSelect()];
						if (magicScreen->getSelect() < 8)
						{
							currentMag[magicSlots->getSelect()].setUsed(true);
						}
						if (magicSlots->getSelect() == 0)
						{
							magic1 = magicScreen->getSelect();
						}
						else if (magicSlots->getSelect() == 1)
						{
							magic2 = magicScreen->getSelect();
						}
						else if (magicSlots->getSelect() == 2)
						{
							magic3 = magicScreen->getSelect();
						}
						else if (magicSlots->getSelect() == 3)
						{
							magic4 = magicScreen->getSelect();
						}
					}
					if (options[SFX])
					{
						al_play_sample_instance(selectInstance);
					}
				}
				else if (state == OPTION)
				{
					switch (optionScreen->getSelect())
					{
					case 0:
						options[SFX] = !options[SFX];
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 1:
						options[MUSIC] = !options[MUSIC];
						if (options[MUSIC])
						{
							al_play_sample_instance(themeInstance);
						}
						else
						{
							al_stop_sample_instance(themeInstance);
						}
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 2:
						options[FRAMECOUNTER] = !options[FRAMECOUNTER];
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					}
				}
				else if (state == ITEM)
				{
					if (inv.getQuantity(itemScreen->getSelect()) > 0)
					{
						inv.useItem(player, itemScreen->getSelect());
					}
					if (options[SFX])
					{
						al_play_sample_instance(selectInstance);
					}
				}
				else if (state == EQUIP)
				{
					inv.equipItem(player, equipmentScreen->getSelect());
					if (options[SFX])
					{
						al_play_sample_instance(selectInstance);
					}
				}
				else if (state == CONFIG)
				{
					switch (optionScreen->getSelect())
					{
					case 0:
						options[SFX] = !options[SFX];
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 1:
						options[MUSIC] = !options[MUSIC];
						if (options[MUSIC])
						{
							al_play_sample_instance(songInstance);
						}
						else
						{
							al_stop_sample_instance(songInstance);
						}
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 2:
						options[FRAMECOUNTER] = !options[FRAMECOUNTER];
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					}
				}
				else if (state == PLAYING)
				{
				}
				else if (state == LEVEL2)
				{
				}
				else if (state == BATTLE)
				{
					if (battleControl->getSelect() == 0)
					{
						if (player->getEnergy() - 2 >= 0)
						{
							enemy->loselife(player->attackValue());
							slash->setAlive(true);
							player->decreaseEnergy(2);
							player->setInsufficient(false);
							if (options[SFX])
							{
								al_play_sample_instance(attackInstance);
							}
						}
						else
						{
							player->setInsufficient(true);
						}
					}
					else if (battleControl->getSelect() == 1 && keys[SPACE] == false)
					{
						if (magicControl->getSelect() == 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[0].getUsed() && ((player->getMP() - currentMag[0].getCost()) >= 0))
								{
									if (!currentMag[0].doesItSupport() && !currentMag[0].doesItHeal())
									{
										enemy->loselife(currentMag[0].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else if (currentMag[0].doesItHeal())
									{
										player->addHealth(currentMag[0].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[0].getAttack(), currentMag[0].getDefense(), currentMag[0].getMagicDefense());
									}
									if (currentMag[0].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[0].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[0].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[0].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[0].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[0].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[0].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[0].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[0].playAnim();
									player->loseMana(currentMag[0].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 1)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[1].getUsed() && ((player->getMP() - currentMag[1].getCost()) >= 0))
								{
									if (!currentMag[1].doesItSupport() && !currentMag[1].doesItHeal())
									{
										enemy->loselife(currentMag[1].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else if (currentMag[1].doesItHeal())
									{
										player->addHealth(currentMag[1].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[1].getAttack(), currentMag[1].getDefense(), currentMag[1].getMagicDefense());
									}
									if (currentMag[1].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[1].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[1].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[1].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[1].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[1].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[1].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[1].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[1].playAnim();
									player->loseMana(currentMag[1].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 2 && player->getMP() >= 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[2].getUsed() && ((player->getMP() - currentMag[2].getCost()) >= 0))
								{
									if (!currentMag[2].doesItSupport() && !currentMag[2].doesItHeal())
									{
										enemy->loselife(currentMag[2].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else if (currentMag[2].doesItHeal())
									{
										player->addHealth(currentMag[2].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[2].getAttack(), currentMag[2].getDefense(), currentMag[2].getMagicDefense());
									}
									if (currentMag[2].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[2].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[2].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[2].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[2].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[2].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[2].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[2].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[2].playAnim();
									player->loseMana(currentMag[2].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 3 && player->getMP() >= 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[3].getUsed() && ((player->getMP() - currentMag[3].getCost()) >= 0))
								{
									if (!currentMag[3].doesItSupport() && !currentMag[3].doesItHeal())
									{
										enemy->loselife(currentMag[3].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else if (currentMag[3].doesItHeal())
									{
										player->addHealth(currentMag[3].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[3].getAttack(), currentMag[3].getDefense(), currentMag[3].getMagicDefense());
									}
									if (currentMag[3].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[3].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[3].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[3].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[3].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[3].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[3].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[3].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[3].playAnim();
									player->loseMana(currentMag[3].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
					}
					else if (battleControl->getSelect() == 2)
					{
						if (player->getEnergy() - 4 >= 0)
						{
							defensive->setAlive(true);
							player->setDefend(true);
							player->decreaseEnergy(4);
							player->setInsufficient(false);
							al_play_sample_instance(barrierInstance);
						}
						else
						{
							player->setInsufficient(true);
						}
					}
					else if (battleControl->getSelect() == 3)
					{
						if ((rand() % 10) >= 4)
						{
							changeState(state, PLAYING);
							if (options[SFX])
							{
								al_play_sample_instance(fleeInstance);
							}
						}
						else
						{
							battleLock = true;
						}
					}
				}
				else if (state == BATTLE2)
				{
					if (battleControl->getSelect() == 0)
					{
						if (player->getEnergy() - 2 >= 0)
						{
							enemy->loselife(player->attackValue());
							slash->setAlive(true);
							player->decreaseEnergy(2);
							player->setInsufficient(false);
							if (options[SFX])
							{
								al_play_sample_instance(attackInstance);
							}
						}
						else
						{
							player->setInsufficient(true);
						}
					}
					else if (battleControl->getSelect() == 1 && keys[SPACE] == false)
					{
						if (magicControl->getSelect() == 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[0].getUsed() && ((player->getMP() - currentMag[0].getCost()) >= 0))
								{
									if (!currentMag[0].doesItSupport() && !currentMag[0].doesItHeal())
									{
										enemy->loselife(currentMag[0].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else if (currentMag[0].doesItHeal())
									{
										player->addHealth(currentMag[0].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[0].getAttack(), currentMag[0].getDefense(), currentMag[0].getMagicDefense());
									}
									if (currentMag[0].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[0].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[0].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[0].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[0].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[0].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[0].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[0].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[0].playAnim();
									player->loseMana(currentMag[0].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 1)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[1].getUsed() && ((player->getMP() - currentMag[1].getCost()) >= 0))
								{
									if (!currentMag[1].doesItSupport() && !currentMag[1].doesItHeal())
									{
										enemy->loselife(currentMag[1].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else if (currentMag[1].doesItHeal())
									{
										player->addHealth(currentMag[1].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[1].getAttack(), currentMag[1].getDefense(), currentMag[1].getMagicDefense());
									}
									if (currentMag[1].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[1].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[1].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[1].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[1].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[1].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[1].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[1].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[1].playAnim();
									player->loseMana(currentMag[1].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 2 && player->getMP() >= 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[2].getUsed() && ((player->getMP() - currentMag[2].getCost()) >= 0))
								{
									if (!currentMag[2].doesItSupport() && !currentMag[2].doesItHeal())
									{
										enemy->loselife(currentMag[2].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else if (currentMag[2].doesItHeal())
									{
										player->addHealth(currentMag[2].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[2].getAttack(), currentMag[2].getDefense(), currentMag[2].getMagicDefense());
									}
									if (currentMag[2].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[2].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[2].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[2].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[2].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[2].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[2].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[2].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[2].playAnim();
									player->loseMana(currentMag[2].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 3 && player->getMP() >= 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[3].getUsed() && ((player->getMP() - currentMag[3].getCost()) >= 0))
								{
									if (!currentMag[3].doesItSupport() && !currentMag[3].doesItHeal())
									{
										enemy->loselife(currentMag[3].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else if (currentMag[3].doesItHeal())
									{
										player->addHealth(currentMag[3].damage(player->getIntelligence(), enemy->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[3].getAttack(), currentMag[3].getDefense(), currentMag[3].getMagicDefense());
									}
									if (currentMag[3].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[3].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[3].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[3].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[3].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[3].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[3].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[3].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[3].playAnim();
									player->loseMana(currentMag[3].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
								else
								{

								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}

					}
					else if (battleControl->getSelect() == 2)
					{
						if (player->getEnergy() - 4 >= 0)
						{
							defensive->setAlive(true);
							player->setDefend(true);
							player->decreaseEnergy(4);
							player->setInsufficient(false);
							al_play_sample_instance(barrierInstance);
						}
						else
						{
							player->setInsufficient(true);
						}
					}
					else if (battleControl->getSelect() == 3)
					{
						if ((rand() % 10) >= 4)
						{
							changeState(state, LEVEL2);
							if (options[SFX])
							{
								al_play_sample_instance(fleeInstance);
							}
						}
						else
						{
							battleLock = true;
						}

					}
				}
				else if (state == BOSS)
				{
					if (battleControl->getSelect() == 0)
					{
						if (player->getEnergy() - 2 >= 0)
						{
							boss->loselife(player->attackValue());
							slash->setAlive(true);
							player->decreaseEnergy(2);
							player->setInsufficient(false);
							if (options[SFX])
							{
								al_play_sample_instance(attackInstance);
							}
						}
						else
						{
							player->setInsufficient(true);
						}
					}
					else if (battleControl->getSelect() == 1 && keys[SPACE] == false)
					{
						if (magicControl->getSelect() == 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[0].getUsed() && ((player->getMP() - currentMag[0].getCost()) >= 0))
								{
									if (!currentMag[0].doesItSupport() && !currentMag[0].doesItHeal())
									{
										boss->loselife(currentMag[0].damage(player->getIntelligence(), boss->getMagicDefense()));
									}
									else if (currentMag[0].doesItHeal())
									{
										player->addHealth(currentMag[0].damage(player->getIntelligence(), boss->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[0].getAttack(), currentMag[0].getDefense(), currentMag[0].getMagicDefense());
									}
									if (currentMag[0].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[0].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[0].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[0].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[0].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[0].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[0].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[0].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[0].playAnim();
									player->loseMana(currentMag[0].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 1)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[1].getUsed() && ((player->getMP() - currentMag[1].getCost()) >= 0))
								{
									if (!currentMag[1].doesItSupport() && !currentMag[1].doesItHeal())
									{
										boss->loselife(currentMag[1].damage(player->getIntelligence(), boss->getMagicDefense()));
									}
									else if (currentMag[1].doesItHeal())
									{
										player->addHealth(currentMag[1].damage(player->getIntelligence(), boss->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[1].getAttack(), currentMag[1].getDefense(), currentMag[1].getMagicDefense());
									}
									if (currentMag[1].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[1].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[1].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[1].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[1].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[1].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[1].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[1].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[1].playAnim();
									player->loseMana(currentMag[1].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 2 && player->getMP() >= 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[2].getUsed() && ((player->getMP() - currentMag[2].getCost()) >= 0))
								{
									if (!currentMag[2].doesItSupport() && !currentMag[2].doesItHeal())
									{
										boss->loselife(currentMag[2].damage(player->getIntelligence(), boss->getMagicDefense()));
									}
									else if (currentMag[2].doesItHeal())
									{
										player->addHealth(currentMag[2].damage(player->getIntelligence(), boss->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[2].getAttack(), currentMag[2].getDefense(), currentMag[2].getMagicDefense());
									}
									if (currentMag[2].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[2].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[2].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[2].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[2].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[2].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[2].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[2].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[2].playAnim();
									player->loseMana(currentMag[2].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 3 && player->getMP() >= 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[3].getUsed() && ((player->getMP() - currentMag[3].getCost()) >= 0))
								{
									if (!currentMag[3].doesItSupport() && !currentMag[3].doesItHeal())
									{
										boss->loselife(currentMag[3].damage(player->getIntelligence(), boss->getMagicDefense()));
									}
									else if (currentMag[3].doesItHeal())
									{
										player->addHealth(currentMag[3].damage(player->getIntelligence(), boss->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[3].getAttack(), currentMag[3].getDefense(), currentMag[3].getMagicDefense());
									}
									if (currentMag[3].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[3].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[3].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[3].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[3].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[3].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[3].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[3].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[3].playAnim();
									player->loseMana(currentMag[3].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
								else
								{

								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}

					}
					else if (battleControl->getSelect() == 2)
					{
						if (player->getEnergy() - 4 >= 0)
						{
							defensive->setAlive(true);
							player->setDefend(true);
							player->decreaseEnergy(4);
							player->setInsufficient(false);
							al_play_sample_instance(barrierInstance);
						}
						else
						{
							player->setInsufficient(true);
						}
					}
					else if (battleControl->getSelect() == 3)
					{
						battleLock = true;
					}
				}
				else if (state == BOSS2)
				{
					if (battleControl->getSelect() == 0)
					{
						if (player->getEnergy() - 2 >= 0)
						{
							boss2->loselife(player->attackValue());
							slash->setAlive(true);
							player->decreaseEnergy(2);
							player->setInsufficient(false);
							if (options[SFX])
							{
								al_play_sample_instance(attackInstance);
							}
						}
						else
						{
							player->setInsufficient(true);
						}
					}
					else if (battleControl->getSelect() == 1 && keys[SPACE] == false)
					{
						if (magicControl->getSelect() == 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[0].getUsed() && ((player->getMP() - currentMag[0].getCost()) >= 0))
								{
									if (!currentMag[0].doesItSupport() && !currentMag[0].doesItHeal())
									{
										boss2->loselife(currentMag[0].damage(player->getIntelligence(), boss2->getMagicDefense()));
									}
									else if (currentMag[0].doesItHeal())
									{
										player->addHealth(currentMag[0].damage(player->getIntelligence(), boss2->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[0].getAttack(), currentMag[0].getDefense(), currentMag[0].getMagicDefense());
									}
									if (currentMag[0].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[0].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[0].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[0].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[0].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[0].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[0].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[0].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[0].playAnim();
									player->loseMana(currentMag[0].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 1)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[1].getUsed() && ((player->getMP() - currentMag[1].getCost()) >= 0))
								{
									if (!currentMag[1].doesItSupport() && !currentMag[1].doesItHeal())
									{
										boss2->loselife(currentMag[1].damage(player->getIntelligence(), boss2->getMagicDefense()));
									}
									else if (currentMag[1].doesItHeal())
									{
										player->addHealth(currentMag[1].damage(player->getIntelligence(), boss2->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[1].getAttack(), currentMag[1].getDefense(), currentMag[1].getMagicDefense());
									}
									if (currentMag[1].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[1].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[1].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[1].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[1].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[1].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[1].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[1].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[1].playAnim();
									player->loseMana(currentMag[1].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 2 && player->getMP() >= 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[2].getUsed() && ((player->getMP() - currentMag[2].getCost()) >= 0))
								{
									if (!currentMag[2].doesItSupport() && !currentMag[2].doesItHeal())
									{
										boss2->loselife(currentMag[2].damage(player->getIntelligence(), boss2->getMagicDefense()));
									}
									else if (currentMag[2].doesItHeal())
									{
										player->addHealth(currentMag[2].damage(player->getIntelligence(), boss2->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[2].getAttack(), currentMag[2].getDefense(), currentMag[2].getMagicDefense());
									}
									if (currentMag[2].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[2].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[2].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[2].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[2].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[2].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[2].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[2].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[2].playAnim();
									player->loseMana(currentMag[2].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 3 && player->getMP() >= 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[3].getUsed() && ((player->getMP() - currentMag[3].getCost()) >= 0))
								{
									if (!currentMag[3].doesItSupport() && !currentMag[3].doesItHeal())
									{
										boss2->loselife(currentMag[3].damage(player->getIntelligence(), boss2->getMagicDefense()));
									}
									else if (currentMag[3].doesItHeal())
									{
										player->addHealth(currentMag[3].damage(player->getIntelligence(), boss2->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[3].getAttack(), currentMag[3].getDefense(), currentMag[3].getMagicDefense());
									}
									if (currentMag[3].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[3].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[3].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[3].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[3].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[3].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[3].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[3].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[3].playAnim();
									player->loseMana(currentMag[3].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
								else
								{

								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}

					}
					else if (battleControl->getSelect() == 2)
					{
						if (player->getEnergy() - 4 >= 0)
						{
							defensive->setAlive(true);
							player->setDefend(true);
							player->decreaseEnergy(4);
							player->setInsufficient(false);
							al_play_sample_instance(barrierInstance);
						}
						else
						{
							player->setInsufficient(true);
						}
					}
					else if (battleControl->getSelect() == 3)
					{
						if ((rand() % 10) >= 4)
						{
							changeState(state, LEVEL2);
							if (options[SFX])
							{
								al_play_sample_instance(fleeInstance);
							}
						}
						else
						{
							battleLock = true;
						}
					}
				}
				else if (state == BOSS3)
				{
					if (battleControl->getSelect() == 0)
					{
						if (player->getEnergy() - 2 >= 0)
						{
							boss3->loselife(player->attackValue());
							slash->setAlive(true);
							player->decreaseEnergy(2);
							player->setInsufficient(false);
							if (options[SFX])
							{
								al_play_sample_instance(attackInstance);
							}
						}
						else
						{
							player->setInsufficient(true);
						}
					}
					else if (battleControl->getSelect() == 1 && keys[SPACE] == false)
					{
						if (magicControl->getSelect() == 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[0].getUsed() && ((player->getMP() - currentMag[0].getCost()) >= 0))
								{
									if (!currentMag[0].doesItSupport() && !currentMag[0].doesItHeal())
									{
										boss3->loselife(currentMag[0].damage(player->getIntelligence(), boss3->getMagicDefense()));
									}
									else if (currentMag[0].doesItHeal())
									{
										player->addHealth(currentMag[0].damage(player->getIntelligence(), boss3->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[0].getAttack(), currentMag[0].getDefense(), currentMag[0].getMagicDefense());
									}
									if (currentMag[0].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[0].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[0].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[0].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[0].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[0].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[0].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[0].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[0].playAnim();
									player->loseMana(currentMag[0].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 1)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[1].getUsed() && ((player->getMP() - currentMag[1].getCost()) >= 0))
								{
									if (!currentMag[1].doesItSupport() && !currentMag[1].doesItHeal())
									{
										boss3->loselife(currentMag[1].damage(player->getIntelligence(), boss3->getMagicDefense()));
									}
									else if (currentMag[1].doesItHeal())
									{
										player->addHealth(currentMag[1].damage(player->getIntelligence(), boss3->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[1].getAttack(), currentMag[1].getDefense(), currentMag[1].getMagicDefense());
									}
									if (currentMag[1].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[1].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[1].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[1].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[1].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[1].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[1].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[1].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[1].playAnim();
									player->loseMana(currentMag[1].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 2 && player->getMP() >= 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[2].getUsed() && ((player->getMP() - currentMag[2].getCost()) >= 0))
								{
									if (!currentMag[2].doesItSupport() && !currentMag[2].doesItHeal())
									{
										boss3->loselife(currentMag[2].damage(player->getIntelligence(), boss3->getMagicDefense()));
									}
									else if (currentMag[2].doesItHeal())
									{
										player->addHealth(currentMag[2].damage(player->getIntelligence(), boss3->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[2].getAttack(), currentMag[2].getDefense(), currentMag[2].getMagicDefense());
									}
									if (currentMag[2].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[2].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[2].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[2].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[2].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[2].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[2].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[2].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[2].playAnim();
									player->loseMana(currentMag[2].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}
						else if (magicControl->getSelect() == 3 && player->getMP() >= 0)
						{
							if (player->getEnergy() - 3 >= 0)
							{
								if (currentMag[3].getUsed() && ((player->getMP() - currentMag[3].getCost()) >= 0))
								{
									if (!currentMag[3].doesItSupport() && !currentMag[3].doesItHeal())
									{
										boss3->loselife(currentMag[3].damage(player->getIntelligence(), boss3->getMagicDefense()));
									}
									else if (currentMag[3].doesItHeal())
									{
										player->addHealth(currentMag[3].damage(player->getIntelligence(), boss3->getMagicDefense()));
									}
									else
									{
										player->addSupport(currentMag[3].getAttack(), currentMag[3].getDefense(), currentMag[3].getMagicDefense());
									}
									if (currentMag[3].getName() == "Fire")
									{
										al_play_sample_instance(fireInstance);
									}
									else if (currentMag[3].getName() == "Cure")
									{
										al_play_sample_instance(healInstance);
									}
									else if (currentMag[3].getName() == "Lightning")
									{
										al_play_sample_instance(thunderInstance);
									}
									else if (currentMag[3].getName() == "Ice")
									{
										al_play_sample_instance(iceInstance);
									}
									else if (currentMag[3].getName() == "Wind")
									{
										al_play_sample_instance(windInstance);
									}
									else if (currentMag[3].getName() == "Brave")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[3].getName() == "Protect")
									{
										al_play_sample_instance(barrierInstance);
									}
									else if (currentMag[3].getName() == "Shell")
									{
										al_play_sample_instance(healInstance);
									}
									currentMag[3].playAnim();
									player->loseMana(currentMag[3].getCost());
									player->decreaseEnergy(3);
									player->setInsufficient(false);
								}
								else
								{

								}
							}
							else
							{
								player->setInsufficient(true);
							}
						}

					}
					else if (battleControl->getSelect() == 2)
					{
						if (player->getEnergy() - 4 >= 0)
						{
							defensive->setAlive(true);
							player->setDefend(true);
							player->decreaseEnergy(4);
							player->setInsufficient(false);
							al_play_sample_instance(barrierInstance);
						}
						else
						{
							player->setInsufficient(true);
						}
					}
					else if (battleControl->getSelect() == 3)
					{
						battleLock = true;
					}
				}
				else if (state == VICTORY)
				{
					if (lastState == BATTLE)
					{
						changeState(state, PLAYING);
					}
					else if (lastState == BOSS)
					{
						changeState(state, WORLD);
					}
					else if (lastState == BOSS2)
					{
						changeState(state, LEVEL2);
					}
					else if (lastState == BOSS3)
					{
						changeState(state, END);
					}
					if (options[SFX])
					{
						al_play_sample_instance(selectInstance);
					}
				}
				else if (state == LOST)
				{
					switch (lostScreen->getSelect())
					{
					case 0:
						changeState(state, WORLD);
						player->setHp(player->getTotalHealth());
						player->setMp(player->getTotalMP());
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 1:
						changeState(state, TITLE1);
						if (options[SFX])
						{
							al_play_sample_instance(selectInstance);
						}
						break;
					case 2:
						done = true;
						break;
					}
				}
				break;
			case ALLEGRO_KEY_ENTER:
				keys[ENTER] = true;
				break;
			case ALLEGRO_KEY_F1:
				if (state != ABOUT)
				{
					if (state == LEVEL2)
					{
						changeState(state, BOSSTALK);
					}
					else if (state == PLAYING)
					{
						changeState(state, BOSS);
					}
					else
					{
						changeState(state, ABOUT);
					}
				}
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = false;

				if (state == PLAYING)
				{
					if (!keys[DOWN] && !keys[RIGHT] && !keys[LEFT])
					{
						player->resetUp();
					}
				}
				else if (state == LEVEL2)
				{
					if (!keys[DOWN] && !keys[RIGHT] && !keys[LEFT])
					{
						player->resetUp();
					}
				}
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;

				if (state == PLAYING)
				{
					if (!keys[UP] && !keys[RIGHT] && !keys[LEFT])
					{
						player->resetDown();
					}
				}
				else if (state == LEVEL2)
				{
					if (!keys[UP] && !keys[RIGHT] && !keys[LEFT])
					{
						player->resetDown();
					}
				}
				break;
			case ALLEGRO_KEY_LEFT:

				keys[LEFT] = false;
				if (state == PLAYING)
				{
					if (!keys[UP] && !keys[RIGHT] && !keys[DOWN])
					{
						player->resetLeft();
					}
				}
				else if (state == LEVEL2)
				{
					if (!keys[UP] && !keys[RIGHT] && !keys[DOWN])
					{
						player->resetLeft();
					}
				}
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;

				if (state == PLAYING)
				{
					if (!keys[LEFT] && !keys[DOWN] && !keys[UP])
					{
						player->resetRight();
					}
				}
				else if (state == LEVEL2)
				{
					if (!keys[LEFT] && !keys[DOWN] && !keys[UP])
					{
						player->resetRight();
					}
				}
				break;
			case ALLEGRO_KEY_ESCAPE:
				if (state == TITLE1)
				{
					done = true;
				}
				else if (state == TITLE2)
				{
					done = true;
				}
				else if (state == ABOUT)
				{
					changeState(state, lastState);
				}
				else if (state == MAIN)
				{
					if (levelState == PLAYING)
					{
						changeState(state, PLAYING);
					}
					else if (levelState == LEVEL2)
					{
						changeState(state, LEVEL2);
					}
				}
				else if (state == WORLD)
				{
					changeState(state, WORLD_MAIN);
				}
				else if (state == WORLD_MAIN)
				{
					changeState(state, WORLD);
				}
				else if (state == BUY)
				{
					changeState(state, SHOP);
				}
				else if (state == SELL)
				{
					changeState(state, SHOP);
				}
				else if (state == SHOP)
				{
					changeState(state, WORLD);
					al_stop_sample_instance(shopInstance);
				}
				else if (state == GAMBLE)
				{
					changeState(state, SHOP);
				}
				else if (state == STATUS)
				{
					if (lastState == WORLD_MAIN)
					{
						changeState(state, WORLD_MAIN);
					}
					else if (lastState == MAIN)
					{
						changeState(state, MAIN);
					}
				}
				else if (state == OPTION)
				{
					changeState(state, TITLE1);
				}
				else if (state == CONFIG)
				{
					if (lastState == WORLD_MAIN)
					{
						changeState(state, WORLD_MAIN);
					}
					else if (lastState == MAIN)
					{
						changeState(state, MAIN);
					}
				}
				else if (state == EQUIP)
				{
					if (lastState == WORLD_MAIN)
					{
						changeState(state, WORLD_MAIN);
					}
					else if (lastState == MAIN)
					{
						changeState(state, MAIN);
					}
				}
				else if (state == ITEM)
				{
					if (lastState == WORLD_MAIN)
					{
						changeState(state, WORLD_MAIN);
					}
					else if (lastState == MAIN)
					{
						changeState(state, MAIN);
					}
				}
				else if (state == SAVE)
				{
					if (lastState == WORLD_MAIN)
					{
						changeState(state, WORLD_MAIN);
					}
					else if (lastState == MAIN)
					{
						changeState(state, MAIN);
					}
				}
				else if (state == LOAD1)
				{
					changeState(state, TITLE2);
				}
				else if (state == LOAD2)
				{
					if (lastState == WORLD_MAIN)
					{
						changeState(state, WORLD_MAIN);
					}
					else if (lastState == MAIN)
					{
						changeState(state, MAIN);
					}
				}
				else if (state == MAGIC)
				{
					if (lastState == WORLD_MAIN)
					{
						changeState(state, WORLD_MAIN);
					}
					else if (lastState == MAIN)
					{
						changeState(state, MAIN);
					}
				}
				else if (state == PLAYING)
				{
					bg->reset();
					player->resetDown();
					changeState(state, MAIN);
				}
				else if (state == LEVEL2)
				{
					bg2->reset();
					player->resetDown();
					changeState(state, MAIN);
				}
				else if (state == WORLD)
				{
					changeState(state, WORLD_MAIN);
				}
				else if (state == BATTLE)
				{
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						keys[SPACE] = false;
					}
				}
				else if (state == BATTLE2)
				{
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						keys[SPACE] = false;
					}
				}
				else if (state == BOSS)
				{
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						keys[SPACE] = false;
					}
				}
				else if (state == BOSS2)
				{
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						keys[SPACE] = false;
					}
				}
				else if (state == BOSS3)
				{
					if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						keys[SPACE] = false;
					}
				}
				break;
			case ALLEGRO_KEY_SPACE:
				if (state == BATTLE)
				{
					if (battleControl->getSelect() != 1)
					{
						keys[SPACE] = false;
					}
				}
				else if (state == BATTLE2)
				{
					if (battleControl->getSelect() != 1)
					{
						keys[SPACE] = false;
					}
				}
				else if (state == BOSS)
				{
					if (battleControl->getSelect() != 1)
					{
						keys[SPACE] = false;
					}
				}
				else if (state == BOSS2)
				{
					if (battleControl->getSelect() != 1)
					{
						keys[SPACE] = false;
					}
				}
				else if (state == BOSS3)
				{
					if (battleControl->getSelect() != 1)
					{
						keys[SPACE] = false;
					}
				}
				else if (state == MAGIC)
				{
				}
				break;
			case ALLEGRO_KEY_ENTER:
				keys[ENTER] = false;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			//Render
			render = true;
			
			//Timer
			secondCount = al_get_timer_count(elapsed) / 60;
			minuteCount = al_get_timer_count(elapsed) / 3600;
			hourCount = al_get_timer_count(elapsed) / 216000;
			battleSecond = al_get_timer_count(battleTimer) / 60;
			gameControl = al_get_timer_count(gameTimer) / 60;
			elapsedControl = al_get_timer_count(elapsed) / 60;

			//Game FPS
			frames++;
			if (al_current_time() - gameTime >= 1)
			{
				gameTime = al_current_time();
				gameFPS = frames;
				frames = 0;
			}
			if (gameControl == 60)
			{
				if (state != BOSS && state != BOSS2)
				{
					if (stage == 1)
					{
						al_set_timer_count(gameTimer, 0);
						changeState(state, BOSS);
					}
					else if (stage == 2)
					{
						al_set_timer_count(gameTimer, 0);
						changeState(state, BOSSTALK);
					}
				}
				else if (state == BOSS)
				{
				}
				else if (state == BOSS3)
				{
				}
				else
				{
					changeState(state, LOST);
				}
			}
			if (state == TITLE1)
			{
				titleScreen1->selectUpdate();
			}
			else if (state == TITLE2)
			{
				titleScreen2->selectUpdate();
			}
			else if (state == ABOUT)
			{
				about->update();
			}
			else if (state == CREDIT)
			{
				credits->update();
			}
			else if (state == CONVERSATION)
			{
				conversation->update();
			}
			else if (state == BOSSTALK)
			{
				bossScreen->update();
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if ((*iter)->getAlive() == true)
					{
						(*iter)->update();
					}
				}
			}
			else if (state == END)
			{
				bossScreen->update();
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if ((*iter)->getAlive() == true)
					{
						(*iter)->update();
					}
				}
			}
			else if (state == WORLD)
			{
				worldScreen->selectUpdate();
			}
			else if (state == SHOP)
			{
				shopMenu->update();
				shopControl->selectUpdate();
			}
			else if (state == BUY)
			{
				buy->selectUpdate();
				if (buy->getSelect() == 0)
				{
					buysellItem->selectUpdate();
				}
				else if (buy->getSelect() == 1)
				{
					buysellEquipment->selectUpdate();
				}
			}
			else if (state == SELL)
			{
				sell->selectUpdate();
				if (sell->getSelect() == 0)
				{
					buysellItem->selectUpdate();
				}
				else if (sell->getSelect() == 1)
				{
					buysellEquipment->selectUpdate();
				}
			}
			else if (state == GAMBLE)
			{
				gambleScreen->update();
			}
			else if (state == MAIN)
			{
				menuScreen->selectUpdate();
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					(*iter)->update();
				}
			}
			else if (state == WORLD_MAIN)
			{
				menuScreen->selectUpdate();
			}
			else if (state == EQUIP)
			{
				equipmentScreen->selectUpdate();
				equipmentScreen->update();
			}
			else if (state == ITEM)
			{
				itemScreen->selectUpdate();
			}
			else if (state == OPTION)
			{
				optionScreen->selectUpdate();
			}
			else if (state == CONFIG)
			{
				optionScreen->selectUpdate();
			}
			else if (state == SAVE)
			{
				saveScreen->selectUpdate();
			}
			else if (state == LOAD1)
			{
				loadScreen->selectUpdate();
			}
			else if (state == LOAD2)
			{
				loadScreen->selectUpdate();
			}
			else if (state == MAGIC)
			{
				magicScreen->selectUpdate();

				if (keys[SPACE] == true)
				{
					magicSlots->selectUpdate();
				}
			}
			else if (state == PLAYING)
			{
				//keys
				if (randomEncounter == 199)
				{
					changeState(state, BATTLE);
				}
				else
				{
					if (keys[LEFT] && !keys[RIGHT] && !keys[UP] && !keys[DOWN])
					{
						bg->moveLeft();
						randomEncounter = rand() % 200;
					}
					else if (keys[RIGHT] && !keys[LEFT] && !keys[UP] && !keys[DOWN])
					{
						bg->moveRight();
						randomEncounter = rand() % 200;
					}
					else if (keys[UP] && !keys[RIGHT] && !keys[DOWN] && !keys[LEFT])
					{
						bg->moveUp();
						randomEncounter = rand() % 200;
					}
					else if (keys[DOWN] && !keys[UP] && !keys[RIGHT] && !keys[LEFT])
					{
						bg->moveDown();
						randomEncounter = rand() % 200;
					}
					else
					{
						bg->reset();
					}
				}

				//update
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					(*iter)->update();
				}
			}
			else if (state == LEVEL2)
			{
				//keys
				if (randomEncounter == 199)
				{
					changeState(state, BOSS2);
				}
				else
				{
					if (keys[LEFT] && !keys[RIGHT] && !keys[UP] && !keys[DOWN])
					{
						bg2->moveLeft();
						randomEncounter = rand() % 200;
					}
					else if (keys[RIGHT] && !keys[LEFT] && !keys[UP] && !keys[DOWN])
					{
						bg2->moveRight();
						randomEncounter = rand() % 200;
					}
					else if (keys[UP] && !keys[RIGHT] && !keys[DOWN] && !keys[LEFT])
					{
						bg2->moveUp();
						randomEncounter = rand() % 200;
					}
					else if (keys[DOWN] && !keys[UP] && !keys[RIGHT] && !keys[LEFT])
					{
						bg2->moveDown();
						randomEncounter = rand() % 200;
					}
					else
					{
						bg2->reset();
					}
				}

				//update
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					(*iter)->update();
				}
			}
			else if (state == BATTLE)
			{
				fire->setX(enemy->getX() + 96);
				fire->setY(enemy->getY() + 70);
				lightning->setX(enemy->getX() + 96);
				lightning->setY(enemy->getY() + 72);
				ice->setX(enemy->getX() + 85);
				ice->setY(enemy->getY() + 70);
				wind->setX(enemy->getX() + 80);
				wind->setY(enemy->getY());
				slash->setX(enemy->getX() + 96);
				slash->setY(enemy->getY() + 48);
				if (battleSecond % 2 == 1)
				{
					player->increaseEnergy();
				}
				else if (battleSecond % 2 == 0)
				{
					player->setRecharge();
				}
				if (battleSecond % 3 == 2)
				{
					player->loseLife(enemy->getDamage());
					enemySlash->setAlive(true);
					player->setDefend(false);
					if (options[SFX])
					{
						al_play_sample_instance(attackInstance);
					}
				}
				else if (battleSecond % 3 == 0)
				{
					player->setHittable();
				}

				if (player->getHealth() > 0 && enemy->getHealth() > 0)
				{
					if (battleControl->getSelect() == 0 && keys[SPACE] == true)
					{
					}
					else if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectUpdate();
					}
					else
					{
						battleControl->selectUpdate();
					}
					for (iter = objects.begin(); iter != objects.end(); ++iter)
					{
						if ((*iter)->getAlive() == true)
						{
							(*iter)->update();
						}
					}
				}
				else
				{
					if (player->getHealth() <= 0)
					{
						changeState(state, LOST);
					}
					else
					{
						changeState(state, VICTORY);
						player->addExp(enemy->getExpValue());
						player->addMoney(enemy->getMoneyValue());
					}
				}

			}
			else if (state == BATTLE2)
			{
				fire->setX(enemy->getX() + 96);
				fire->setY(enemy->getY() + 70);
				lightning->setX(enemy->getX() + 96);
				lightning->setY(enemy->getY() + 72);
				ice->setX(enemy->getX() + 85);
				ice->setY(enemy->getY() + 70);
				wind->setX(enemy->getX() + 80);
				wind->setY(enemy->getY());
				slash->setX(enemy->getX() + 96);
				slash->setY(enemy->getY() + 48);

				if (battleSecond % 2 == 1)
				{
					player->increaseEnergy();
				}
				else if (battleSecond % 2 == 0)
				{
					player->setRecharge();
				}
				if (battleSecond % 3 == 2)
				{
					player->loseLife(enemy->getDamage());
					enemySlash->setAlive(true);
					player->setDefend(false);
					if (options[SFX])
					{
						al_play_sample_instance(attackInstance);
					}
				}
				else if (battleSecond % 3 == 0)
				{
					player->setHittable();
				}

				if (player->getHealth() > 0 && enemy->getHealth() > 0)
				{
					if (battleControl->getSelect() == 0 && keys[SPACE] == true)
					{
					}
					else if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectUpdate();
					}
					else
					{
						battleControl->selectUpdate();
					}
					for (iter = objects.begin(); iter != objects.end(); ++iter)
					{
						if ((*iter)->getAlive() == true)
						{
							(*iter)->update();
						}
					}
				}
				else
				{
					if (player->getHealth() <= 0)
					{
						changeState(state, LOST);
					}
					else
					{
						changeState(state, VICTORY);
						player->addExp(enemy->getExpValue());
						player->addMoney(enemy->getMoneyValue());
					}
				}

			}
			else if (state == BOSS)
			{
				fire->setX(boss->getX() + 60);
				fire->setY(boss->getY() + 48);
				lightning->setX(boss->getX() + 60);
				lightning->setY(boss->getY() + 48);
				ice->setX(boss->getX() + 60);
				ice->setY(boss->getY() + 48);
				wind->setX(boss->getX() + 60);
				wind->setY(boss->getY());
				slash->setX(boss->getX() + 96);
				slash->setY(boss->getY() + 48);
				bossCure->setX(boss->getX() + 96);
				bossCure->setY(boss->getY() + 48);
				enemyIce->setX(640);
				enemyIce->setY(380);
				al_play_sample_instance(squidwalkInstance);

				if (battleSecond % 2 == 1)
				{
					player->increaseEnergy();
				}
				else if (battleSecond % 2 == 0)
				{
					player->setRecharge();
				}
				if (battleSecond % 36 == 35)
				{
					tsunami->setAlive(true);
					tsunami->setChara(true);
					player->setHp(1);
					if (options[SFX])
					{
						al_play_sample_instance(tsunamiInstance);
					}
				}
				else if (battleSecond % 40 == 39)
				{
					tsunami->setChara(false);
				}
				if (boss->getX() == 300 && boss->getHealth() < 1000)
				{
					player->loseLife(50);
					enemyIce->setAlive(true);
					if (options[SFX])
					{
						al_play_sample_instance(iceInstance);
					}
					player->setHittable();
				}
				if (boss->getX() != (player->getX() + 32) && !walkback)
				{
					boss->setX(boss->getX() + 1);
					if (boss->getX() == (player->getX() + 32))
					{

						player->loseLife(boss->getDamage());
						squidsplosion->setAlive(true);
						player->setDefend(false);
						walkback = true;
						boss->setCurrentFrame(4);
						boss->setMaxFrame(7);
						boss->setAnimationColumns(4);
						if (options[SFX])
						{
							al_play_sample_instance(squidAttackInstance);
						}
					}
				}
				else if (boss->getX() != bosspos)
				{
					if (!squidsplosion->getAlive())
					{
						boss->setX(boss->getX() - 1);
						if (boss->getX() == bosspos)
						{
							walkback = false;
							player->setHittable();
							bosspos += 100;
							boss->heal();
							bossCure->setAlive(true);
							boss->setCurrentFrame(8);
							boss->setMaxFrame(11);
							boss->setAnimationColumns(8);
							if (options[SFX])
							{
								al_play_sample_instance(healInstance);
							}
							if (bosspos == 500)
							{
								bosspos = 100;
							}
						}
					}
				}
				if (player->getHealth() > 0 && boss->getHealth() > 0)
				{
					if (battleControl->getSelect() == 0 && keys[SPACE] == true)
					{
					}
					else if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectUpdate();
					}
					else
					{
						battleControl->selectUpdate();
					}
					for (iter = objects.begin(); iter != objects.end(); ++iter)
					{
						if ((*iter)->getAlive() == true)
						{
							(*iter)->update();
						}
					}
				}
				else
				{
					if (player->getHealth() <= 0)
					{
						changeState(state, LOST);
					}
					else
					{
						changeState(state, VICTORY);
						player->addExp(boss->getExpValue());
						player->addMoney(boss->getMoneyValue());
						level2 = true;
						stage = 2;
					}
				}

			}
			else if (state == BOSS2)
			{
				fire->setX(boss2->getX() + 60);
				fire->setY(boss2->getY() + 48);
				lightning->setX(boss2->getX() + 60);
				lightning->setY(boss2->getY() + 48);
				ice->setX(boss2->getX() + 60);
				ice->setY(boss2->getY() + 48);
				wind->setX(boss2->getX() + 60);
				wind->setY(boss2->getY());
				slash->setX(boss2->getX() + 96);
				slash->setY(boss2->getY() + 48);
				bossCure->setX(boss2->getX() + 96);
				bossCure->setY(boss2->getY() + 48);
				if (battleSecond % 2 == 1)
				{
					player->increaseEnergy();
				}
				else if (battleSecond % 2 == 0)
				{
					player->setRecharge();
				}
				if (boss2->getX() == 405 && boss2->getHealth() < 2000)
				{
					boss2->heal();
					bossCure->setAlive(true);
					if (options[SFX])
					{
						al_play_sample_instance(healInstance);
					}
				}
				if (boss2->getX() != (player->getX() + 32) && !walkback2)
				{
					if (firebreath->getAlive())
					{
						firebreath->setX(firebreath->getX() + 10);
						if (options[SFX])
						{
							al_play_sample_instance(dragonRoarInstance);
							al_play_sample_instance(fireInstance);
						}
						player->loseLife(100);
					}
					else if (!firebreath->getAlive())
					{
						boss2->setX(boss2->getX() + 1);
						player->setHittable();
						if (boss2->getX() == (player->getX() + 32))
						{
							player->loseLife(boss2->getDamage());
							enemySlash->setAlive(true);
							player->setDefend(false);
							walkback2 = true;
							firebreath->setX(bosspos2);
							if (options[SFX])
							{
								al_play_sample_instance(attackInstance);
							}
						}
					}
				}
				else if (boss2->getX() != bosspos2)
				{
					if (!enemySlash->getAlive())
					{
						boss2->setX(boss2->getX() - .5);
						if (boss2->getX() == bosspos2)
						{
							firebreath->setAlive(true);
							walkback2 = false;
							player->setHittable();
						}
					}
				}
				if (player->getHealth() > 0 && boss2->getHealth() > 0)
				{
					if (battleControl->getSelect() == 0 && keys[SPACE] == true)
					{
					}
					else if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectUpdate();
					}
					else
					{
						battleControl->selectUpdate();
					}
					for (iter = objects.begin(); iter != objects.end(); ++iter)
					{
						if ((*iter)->getAlive() == true)
						{
							(*iter)->update();
						}
					}
				}
				else
				{
					if (player->getHealth() <= 0)
					{
						changeState(state, LOST);
					}
					else
					{
						changeState(state, VICTORY);
						player->addExp(boss2->getExpValue());
						player->addMoney(boss2->getMoneyValue());
					}
				}
			}
			else if (state == BOSS3)
			{
				fire->setX(boss3->getX() + 60);
				fire->setY(boss3->getY() + 48);
				lightning->setX(boss3->getX() + 60);
				lightning->setY(boss3->getY() + 48);
				ice->setX(boss3->getX() + 60);
				ice->setY(boss3->getY() + 48);
				wind->setX(boss3->getX() + 60);
				wind->setY(boss3->getY());
				slash->setX(boss3->getX() + 96);
				slash->setY(boss3->getY() + 48);
				bossCure->setX(boss3->getX() + 96);
				bossCure->setY(boss3->getY() + 48);
				if (battleSecond % 2 == 1)
				{
					player->increaseEnergy();
				}
				else if (battleSecond % 2 == 0)
				{
					player->setRecharge();
				}
				if (battleSecond % 3 == 0)
				{
					hellfire->setAlive(true);
				}
				if (hellfire->getCurFrame() == 3)
				{
					player->setHp(player->getHealth() - 1);
					if (options[SFX])
					{
						al_play_sample_instance(fireInstance);
					}
				}
				if (battleSecond % 4 == 3)
				{
					if (options[SFX])
					{
						al_play_sample_instance(demonRoarInstance);
					}
				}
				if (battleSecond % 5 == 4)
				{
					meteor->setAlive(true);
					if (options[SFX])
					{
						al_play_sample_instance(firefallInstance);
					}
				}
				else if (meteor->getAlive())
				{
					meteor->setY(meteor->getY() + 10);
					if ((player->getY() + 96) <= meteor->getY())
					{
						meteor->setY(0);
						meteor->setAlive(false);
						player->loseLife(boss3->getDamage());
						player->setHittable();
						player->setDefend(false);

					}
				}
				if (battleSecond % 7 == 5)
				{
					arms->setmaxFrame(3);
					arms->setAnimationColumns(4);
					arms->setAnimationRows(3);
					curse->setAlive(true);
				}
				else if (curse->getAlive())
				{
					death->setAlive(true);
					if (curse->getCurFrame() == 7)
					{
						player->loseLife(boss3->getDamage() - 40);
						arms->setmaxFrame(1);
						arms->setAnimationColumns(1);
						arms->setAnimationRows(0);
						death->setAlive(false);
						player->setHittable();
					}
				}
				if (battleSecond % 11 == 10)
				{
					arms->setmaxFrame(4);
					arms->setAnimationColumns(4);
					arms->setAnimationRows(3);
				}
				else if (arms->getMaxFrame() == 4)
				{
					arms->setX(arms->getX() + 10);
					bossHead->setX(bossHead->getX() + 10);
					boss3->setX(boss3->getX() + 10);
					fist->setX(fist->getX() + 10);
					fist->setAlive(true);
					teleport->setAlive(true);
					if (boss3->getX() >= player->getX() + 120)
					{
						player->loseLife(boss3->getDamage());
						boss3->setX(300);
						arms->setX(245);
						bossHead->setX(235);
						fist->setX(325);
						arms->setmaxFrame(1);
						arms->setAnimationColumns(1);
						arms->setAnimationRows(0);
						player->setHittable();
						fist->setAlive(false);
						if (options[SFX])
						{
							al_play_sample_instance(hittingInstance);
						}
					}
				}
				if (player->getHealth() > 0 && boss3->getHealth() > 0)
				{
					if (battleControl->getSelect() == 0 && keys[SPACE] == true)
					{
					}
					else if (battleControl->getSelect() == 1 && keys[SPACE] == true)
					{
						magicControl->selectUpdate();
					}
					else
					{
						battleControl->selectUpdate();
					}
					for (iter = objects.begin(); iter != objects.end(); ++iter)
					{
						if ((*iter)->getAlive() == true)
						{
							(*iter)->update();
						}
					}
				}
				else
				{
					if (player->getHealth() <= 0)
					{
						changeState(state, LOST);
					}
					else
					{
						changeState(state, VICTORY);
						player->addExp(boss3->getExpValue());
						player->addMoney(boss3->getMoneyValue());
					}
				}
			}
			else if (state == VICTORY)
			{
				victoryScreen->selectUpdate();
			}
			else if (state == LOST)
			{
				lostScreen->selectUpdate();
			}
		}

		//Render
		if (render && al_is_event_queue_empty(event_queue))
		{
			//Stop Rendering
			render = false;

			if (state == TITLE1)
			{
				titleScreen1->render();
				titleScreen1->selectRender(arrow);
				al_draw_bitmap(oneMinute, 48, 40, 0);
			}
			else if (state == TITLE2)
			{
				titleScreen2->render();
				titleScreen2->selectRender(arrow);
				al_draw_bitmap(oneMinute, 48, 40, 0);
			}
			else if (state == ABOUT)
			{
				about->render();
			}
			else if (state == CREDIT)
			{
				credits->render();
				if ((credits->getY() - 480) == -2400)
				{
					changeState(state, WORLD);
				}
				credits->setY(credits->getY() - 1);
			}
			else if (state == CONVERSATION)
			{
				al_draw_bitmap(dialogue, 0, 364, 0);
				if (options[MUSIC])
				{
					al_play_sample_instance(conversationInstance);
				}
				if (conversationCount == 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "*YAWN* ......");
				}
				if (conversationCount == 1)
				{
					if (options[SFX])
					{
						al_play_sample_instance(clockInstance);
					}
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Ugh.... what time is it?");
				}
				if (conversationCount == 2)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Why is the time going so quick.");
				}
				if (conversationCount == 3)
				{
					conversation->render();
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Hey look. I'm at home");
				}
				if (conversationCount == 4)
				{
					conversation->render();
					al_draw_bitmap(fairydialogue, 0, 364, 0);
					al_draw_bitmap(fairy, 200, 100, 0);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Oi hime look over here.");
				}
				if (conversationCount == 5)
				{
					conversation->render();
					al_draw_bitmap(fairydialogue, 0, 364, 0);
					al_draw_bitmap(fairy, 200, 100, 0);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Look here we are running out of time.");
				}
				if (conversationCount == 6)
				{
					conversation->render();
					al_draw_bitmap(fairydialogue, 0, 364, 0);
					al_draw_bitmap(fairy, 200, 100, 0);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Our homeland got cursed and time's");
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 420, 0, "ticking");
				}
				if (conversationCount == 7)
				{
					conversation->render();
					al_draw_bitmap(fairydialogue, 0, 364, 0);
					al_draw_bitmap(fairy, 200, 100, 0);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Hime, you have 60 seconds to save the");
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 420, 0, "world.");
				}
				if (conversationCount >= 8)
				{
					al_stop_sample_instance(clockInstance);
					al_stop_sample_instance(conversationInstance);
					changeState(state, WORLD);
				}
			}
			else if (state == BOSSTALK)
			{
				bossScreen->render();
				al_draw_bitmap(demon1, 200, 100, 0);
				al_draw_bitmap(dialogue, 0, 364, 0);
				if (bossConversation == 1)
				{
					al_draw_bitmap(dialogueLB, 0, 364, 0);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Yes, I am the god of time.");
				}
				if (bossConversation == 2)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Why are you doing this.");
				}
				if (bossConversation == 3)
				{
					al_draw_bitmap(dialogueLB, 0, 364, 0);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Because.....");
				}
				if (bossConversation == 4)
				{
					al_draw_bitmap(dialogueLB, 0, 364, 0);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Im a douche bagel leave me alone.");
				}
				if (bossConversation == 5)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "u wot 1v1 me m8");
				}
				if (bossConversation == 6)
				{
					al_draw_bitmap(dialogueLB, 0, 364, 0);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Alright, I have some free time.");
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 420, 0, "Let's Play");
				}
				if (bossConversation == 7)
				{
					al_draw_bitmap(dialogueLB, 0, 364, 0);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Hold up. Can we take a quick selfie");
				}
				if (bossConversation == 8)
				{
					al_draw_bitmap(dialogueLB, 0, 364, 0);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Cheese.");
				}
				if (bossConversation >= 9)
				{
					changeState(state, BOSS3);
				}
			}
			else if (state == END)
			{
				bossScreen->render();
				boss3->bodyRender();
				bossHead->setAlive(true);
				arms->setAlive(true);
				player->setX((WIDTH * 3) / 4);
				player->setY(HEIGHT / 2);
				al_draw_bitmap(dialogue, 0, 364, 0);
				if (conversationEnd == 1)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "GG WP. Get rekt son.");
				}
				if (conversationEnd == 2)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Noob thought he could solo me.");
				}
				if (conversationEnd == 3)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Sent him home crying.");
				}
				if (conversationEnd == 4)
				{
					al_draw_bitmap(fairydialogue, 0, 364, 0);
					al_draw_bitmap(fairy, 200, 100, 0);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Yo hime run. Book it");
				}
				if (conversationEnd == 5)
				{
					al_draw_bitmap(fairydialogue, 0, 364, 0);
					al_draw_bitmap(fairy, 200, 100, 0);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "See ya nerd");
				}
				if (conversationEnd == 6)
				{
					al_draw_bitmap(fairydialogue, 0, 364, 0);
					al_draw_bitmap(fairy, 200, 100, 0);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Weeeeeeeeeeee");
				}
				if (conversationEnd== 7)
				{
					al_draw_bitmap(dialogue, 0, 364, 0);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 120, 400, 0, "Wait for me!!");
				}
				if (conversationEnd >= 8)
				{
					changeState(state, CREDIT);
					al_stop_sample_instance(lastBossInstance);
				}
			}
			else if (state == WORLD)
			{
				worldScreen->render();
				worldScreen->selectRender(playerArrow);
				al_draw_bitmap(square, 0, 400, 0);
				if (worldScreen->getSelect() == 0)
				{
					al_draw_textf(font48, al_map_rgb(255, 255, 255), 335, 400, ALLEGRO_ALIGN_CENTRE, "SHOP");
				}
				else if (worldScreen->getSelect() == 1)
				{
					al_draw_textf(font48, al_map_rgb(255, 255, 255), 335, 400, ALLEGRO_ALIGN_CENTRE, "LEVEL 1");
				}
				else if (worldScreen->getSelect() == 2)
				{
					al_draw_textf(font48, al_map_rgb(255, 255, 255), 335, 400, ALLEGRO_ALIGN_CENTRE, "LEVEL 2");
				}
			}
			else if (state == BUY)
			{
				if (buy->getSelect() == 0)
				{
					buysellItem->render();
					buysellItem->selectRender(arrow);

					string conversion = inv.getNameTotal(0);
					char *item = new char[100];
					strcpy(item, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 137, 0, item);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 137, 0, "%i", inv.getValueTotal(0));

					conversion = inv.getNameTotal(1);
					char *item1 = new char[100];
					strcpy(item1, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 157, 0, item1);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 157, 0, "%i", inv.getValueTotal(1));

					conversion = inv.getNameTotal(14);
					char *item2 = new char[100];
					strcpy(item2, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 177, 0, item2);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 177, 0, "%i", inv.getValueTotal(14));

					conversion = inv.getNameTotal(15);
					char *item3 = new char[100];
					strcpy(item3, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 197, 0, item3);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 197, 0, "%i", inv.getValueTotal(15));

					int n;

					switch (buysellItem->getSelect())
					{
					case 0:
						n = 0;
						break;
					case 1:
						n = 1;
						break;
					case 2:
						n = 14;
						break;
					case 3:
						n = 15;
						break;
					}

					conversion = inv.getDescriptionTotal(n);
					char *itemDesc = new char[100];
					strcpy(itemDesc, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 20, 30, 0, itemDesc);

					al_draw_textf(font32, al_map_rgb(255, 255, 255), 410, 90, 0, "Equipments");
					conversion = player->getEquipName(0);
					char *equip = new char[100];
					strcpy(equip, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 130, 0, equip);

					conversion = player->getEquipName(1);
					char *equip1 = new char[100];
					strcpy(equip1, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 160, 0, equip1);

					conversion = player->getEquipName(2);
					char *equip2 = new char[100];
					strcpy(equip2, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 190, 0, equip2);

					conversion = player->getEquipName(3);
					char *equip3 = new char[100];
					strcpy(equip3, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 220, 0, equip3);

					al_draw_textf(font32, al_map_rgb(255, 255, 255), 410, 250, ALLEGRO_ALIGN_LEFT, "Stats Lvl %i", player->getLevel());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 290, ALLEGRO_ALIGN_LEFT, "Strength %i", player->getStrength());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 320, ALLEGRO_ALIGN_LEFT, "Intelligence %i", player->getIntelligence());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 350, ALLEGRO_ALIGN_LEFT, "Defense %i", player->getDefense());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 380, ALLEGRO_ALIGN_LEFT, "Magic Defense %i", player->getMagicDefense());

					al_draw_textf(font24, al_map_rgb(255, 255, 255), 430, 445, 0, "Gold %i", player->getMoney());
				}
				else if (buy->getSelect() == 1)
				{
					buysellEquipment->render();
					buysellEquipment->selectRender(arrow);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 430, 445, 0, "Gold %i", player->getMoney());
					string conversion = inv.getNameTotal(2);
					char *equip = new char[100];
					strcpy(equip, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 137, 0, equip);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 137, 0, "%i", inv.getValueTotal(2));

					conversion = inv.getNameTotal(3);
					char *equip1= new char[100];
					strcpy(equip1, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 157, 0, equip1);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 157, 0, "%i", inv.getValueTotal(3));

					conversion = inv.getNameTotal(4);
					char *equip2 = new char[100];
					strcpy(equip2, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 177, 0, equip2);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 177, 0, "%i", inv.getValueTotal(4));

					conversion = inv.getNameTotal(5);
					char *equip3 = new char[100];
					strcpy(equip3, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 197, 0, equip3);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 197, 0, "%i", inv.getValueTotal(5));

					conversion = inv.getNameTotal(6);
					char *equip4 = new char[100];
					strcpy(equip4, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 217, 0, equip4);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 217, 0, "%i", inv.getValueTotal(6));

					conversion = inv.getNameTotal(7);
					char *equip5 = new char[100];
					strcpy(equip5, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 237, 0, equip5);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 237, 0, "%i", inv.getValueTotal(7));

					conversion = inv.getNameTotal(8);
					char *equip6 = new char[100];
					strcpy(equip6, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 257, 0, equip6);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 257, 0, "%i", inv.getValueTotal(8));

					conversion = inv.getNameTotal(9);
					char *equip7 = new char[100];
					strcpy(equip7, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 277, 0, equip7);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 277, 0, "%i", inv.getValueTotal(9));

					conversion = inv.getNameTotal(10);
					char *equip8 = new char[100];
					strcpy(equip8, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 297, 0, equip8);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 297, 0, "%i", inv.getValueTotal(10));

					conversion = inv.getNameTotal(11);
					char *equip9 = new char[100];
					strcpy(equip9, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 317, 0, equip9);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 317, 0, "%i", inv.getValueTotal(11));

					conversion = inv.getNameTotal(12);
					char *equip10 = new char[100];
					strcpy(equip10, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 337, 0, equip10);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 337, 0, "%i", inv.getValueTotal(12));

					conversion = inv.getNameTotal(13);
					char *equip11 = new char[100];
					strcpy(equip11, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 357, 0, equip11);
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 357, 0, "%i", inv.getValueTotal(13));

					conversion = inv.getDescriptionTotal(buysellEquipment->getSelect() + 2);
					char *equipDesc = new char[100];
					strcpy(equipDesc, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 20, 30, 0, equipDesc);

					al_draw_textf(font32, al_map_rgb(255, 255, 255), 410, 90, 0, "Equipments");
					conversion = player->getEquipName(0);
					char *equiped = new char[100];
					strcpy(equiped, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 130, 0, equiped);

					conversion = player->getEquipName(1);
					char *equiped1 = new char[100];
					strcpy(equiped1, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 160, 0, equiped1);

					conversion = player->getEquipName(2);
					char *equiped2 = new char[100];
					strcpy(equiped2, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 190, 0, equiped2);

					conversion = player->getEquipName(3);
					char *equiped3 = new char[100];
					strcpy(equiped3, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 220, 0, equiped3);

					al_draw_textf(font32, al_map_rgb(255, 255, 255), 410, 250, ALLEGRO_ALIGN_LEFT, "Stats Lvl %i", player->getLevel());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 290, ALLEGRO_ALIGN_LEFT, "Strength %i", player->getStrength());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 320, ALLEGRO_ALIGN_LEFT, "Intelligence %i", player->getIntelligence());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 350, ALLEGRO_ALIGN_LEFT, "Defense %i", player->getDefense());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 380, ALLEGRO_ALIGN_LEFT, "Magic Defense %i", player->getMagicDefense());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 430, 445, 0, "Gold %i", player->getMoney());
				}
			}
			else if (state == SELL)
			{
				if (sell->getSelect() == 0)
				{
					buysellItem->render();
					buysellItem->selectRender(arrow);

					int i = 0;
					int n = -1;
					int height = 137;
					while (i < 16)
					{
						string conversion;
						char *item = new char [40];

						if (!inv.isEquipment(i))
						{
							conversion = inv.getName(i);
							strcpy(item, conversion.c_str());
							al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, height, 0, item);
							if (inv.getValue(i) > 0)
							{
								int value = inv.getValue(i) * .6;
								al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, height, 0, "%i", value);
							}
							height += 20;
						}

						i++;
					}
					
					i = 0;
					do
					{
						if (!inv.isEquipment(i))
						{
							n++;
						}
						i++;
					} while (n != buysellItem->getSelect() && i < 16);

					i--;
					string conversion = inv.getDescription(i);
					char *itemDesc = new char[100];
					strcpy(itemDesc, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 20, 30, 0, itemDesc);
		
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 410, 90, 0, "Equipment");
					conversion = player->getEquipName(0);
					char *equipped = new char[100];
					strcpy(equipped, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 130, 0, equipped);

					conversion = player->getEquipName(1);
					char *equipped1 = new char[100];
					strcpy(equipped1, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 160, 0, equipped1);

					conversion = player->getEquipName(2);
					char *equipped2 = new char[100];
					strcpy(equipped2, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 190, 0, equipped2);

					conversion = player->getEquipName(3);
					char *equipped3 = new char[100];
					strcpy(equipped3, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 220, 0, equipped3);

					al_draw_textf(font32, al_map_rgb(255, 255, 255), 410, 250, ALLEGRO_ALIGN_LEFT, "Stats Lvl %i", player->getLevel());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 290, ALLEGRO_ALIGN_LEFT, "Strength %i", player->getStrength());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 320, ALLEGRO_ALIGN_LEFT, "Intelligence %i", player->getIntelligence());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 350, ALLEGRO_ALIGN_LEFT, "Defense %i", player->getDefense());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 380, ALLEGRO_ALIGN_LEFT, "Magic Defense %i", player->getMagicDefense());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 430, 445, 0, "Gold %i", player->getMoney());
				}
				else if (sell->getSelect() == 1)
				{
					buysellEquipment->render();
					buysellEquipment->selectRender(arrow);

					int i = 0;
					int n = -1;
					int height = 137;
					while (i < 16)
					{
						string conversion;
						char *equip = new char[40];

						if (inv.isEquipment(i))
						{
							conversion = inv.getName(i);
							strcpy(equip, conversion.c_str());
							al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, height, 0, equip);
							if (inv.getValue(i) > 0)
							{
								int value = inv.getValue(i) * .6;
								al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, height, 0, "%i", value);
							}
							height += 20;
						}
						i++;
					}

					i = 0;
					do
					{
						if (inv.isEquipment(i))
						{
							n++;
						}
						i++;
					} while (n != buysellEquipment->getSelect() && i < 16);

					i--;
					string conversion = inv.getDescription(i);
					char *itemDesc = new char[100];
					strcpy(itemDesc, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 20, 30, 0, itemDesc);

					al_draw_textf(font32, al_map_rgb(255, 255, 255), 410, 90, 0, "Equipment");
					conversion = player->getEquipName(0);
					char *equiped = new char[100];
					strcpy(equiped, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 130, 0, equiped);

					conversion = player->getEquipName(1);
					char *equiped1 = new char[100];
					strcpy(equiped1, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 160, 0, equiped1);

					conversion = player->getEquipName(2);
					char *equiped2 = new char[100];
					strcpy(equiped2, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 190, 0, equiped2);

					conversion = player->getEquipName(3);
					char *equiped3 = new char[100];
					strcpy(equiped3, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 220, 0, equiped3);

					al_draw_textf(font32, al_map_rgb(255, 255, 255), 410, 250, ALLEGRO_ALIGN_LEFT, "Stats Lvl %i", player->getLevel());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 290, ALLEGRO_ALIGN_LEFT, "Strength %i", player->getStrength());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 320, ALLEGRO_ALIGN_LEFT, "Intelligence %i", player->getIntelligence());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 350, ALLEGRO_ALIGN_LEFT, "Defense %i", player->getDefense());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 420, 380, ALLEGRO_ALIGN_LEFT, "Magic Defense %i", player->getMagicDefense());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 430, 445, 0, "Gold %i", player->getMoney());
				}
			}
			else if (state == SHOP)
			{
				shopMenu->render();
				shopControl->render();
				shopControl->selectRender(arrow);
			}
			else if (state == GAMBLE)
			{
				gambleScreen->render();
				al_draw_textf(font48, al_map_rgb(255, 255, 255), 300, 230, 0, "%i", gambleInput);
				al_draw_textf(font48, al_map_rgb(255, 255, 255), 300, 400, 0, "%i", player->getMoney());

			}
			else if (state == MAIN)
			{
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if ((*iter)->getAlive() == true)
					{
						(*iter)->render();
					}
				}
				menuScreen->render();
				menuScreen->selectRender(arrow);
				al_draw_textf(font48, al_map_rgb(255, 255, 255), 265, 20, 0, "%i.%i", 59 - gameControl, (((((3600 - al_get_timer_count(gameTimer)) * 100) / 60)) % 100));
			}
			else if (state == WORLD_MAIN)
			{
				worldScreen->render();
				worldScreen->selectRender(playerArrow);
				al_draw_bitmap(square, 0, 400, 0);
				if (worldScreen->getSelect() == 0)
				{
					al_draw_textf(font48, al_map_rgb(255, 255, 255), 335, 400, ALLEGRO_ALIGN_CENTRE, "SHOP");
				}
				else if (worldScreen->getSelect() == 1)
				{
					al_draw_textf(font48, al_map_rgb(255, 255, 255), 335, 400, ALLEGRO_ALIGN_CENTRE, "LEVEL 1");
				}
				else if (worldScreen->getSelect() == 2)
				{
					al_draw_textf(font48, al_map_rgb(255, 255, 255), 335, 400, ALLEGRO_ALIGN_CENTRE, "LEVEL 2");
				}
				menuScreen->render();
				menuScreen->selectRender(arrow);
			
			}
			else if (state == STATUS)
			{

				statusScreen->render();
				al_draw_textf(font48, al_map_rgb(255, 255, 255), 30, 20, 0, "Hime");
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 140, 20, 0, "HP %i/%i", player->getHealth(), player->getTotalHealth());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 140, 40, 0, "MP %i/%i", player->getMP(), player->getTotalMP());

				al_draw_textf(font32, al_map_rgb(255, 255, 255), 350, 10, 0, "Level");
				al_draw_textf(font32, al_map_rgb(255, 255, 255), 550, 10, 0, "%i", player->getLevel());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 350, 40, 0, "Strength");
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 550, 40, 0, "%i", player->getStrength());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 350, 60, 0, "Intelligence");
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 550, 60, 0, "%i", player->getIntelligence());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 350, 80, 0, "Defense");
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 550, 80, 0, "%i", player->getDefense());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 350, 100, 0, "Magic Defense");
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 550, 100, 0, "%i", player->getMagicDefense());

				al_draw_textf(font24, al_map_rgb(255, 255, 255), 0, 370, ALLEGRO_ALIGN_LEFT, " Next Level    %i", (player->getNextLevel() - player->getExp()));
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 0, 390, ALLEGRO_ALIGN_LEFT, " EXP           %i", player->getTotalExp());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 0, 410, ALLEGRO_ALIGN_LEFT, " Gold          %i", player->getMoney());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 0, 430, ALLEGRO_ALIGN_LEFT, " Time       %iH %iM %iS", hourCount, minuteCount % 60, secondCount % 60);

				al_draw_textf(font24, al_map_rgb(255, 255, 255), 350, 170, 0, "Equipment");
				string conversion = player->getEquipName(0);
				char *equip = new char[100];
				strcpy(equip, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 370, 190, 0, equip);

				conversion = player->getEquipName(1);
				char *equip1 = new char[100];
				strcpy(equip1, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 370, 220, 0, equip1);

				conversion = player->getEquipName(2);
				char *equip2 = new char[100];
				strcpy(equip2, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 370, 250, 0, equip2);

				conversion = player->getEquipName(3);
				char *equip3 = new char[100];
				strcpy(equip3, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 370, 280, 0, equip3);


				al_draw_textf(font24, al_map_rgb(255, 255, 255), 350, 310, 0, "Magic");

				conversion = currentMag[0].getName();
				char *spell1 = new char[100];
				strcpy(spell1, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 370, 340, 0, spell1);

				conversion = currentMag[1].getName();
				char *spell2 = new char[100];
				strcpy(spell2, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 370, 370, 0, spell2);

				conversion = currentMag[2].getName();
				char *spell3 = new char[100];
				strcpy(spell3, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 370, 400, 0, spell3);

				conversion = currentMag[3].getName();
				char *spell4 = new char[100];
				strcpy(spell4, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 370, 430, 0, spell4);

				conversion.erase();
			}
			else if (state == OPTION)
			{
				optionScreen->render();
				optionScreen->selectRender(arrow);

				if (options[SFX])
				{
					al_draw_bitmap(optionOn, 400, 160, 0);
				}
				else
				{
					al_draw_bitmap(optionOff, 400, 160, 0);
				}
				if (options[MUSIC])
				{
					al_draw_bitmap(optionOn, 400, 259, 0);
				}
				else
				{
					al_draw_bitmap(optionOff, 400, 259, 0);
				}
				if (options[FRAMECOUNTER])
				{
					al_draw_bitmap(optionOn, 400, 357, 0);
				}
				else
				{
					al_draw_bitmap(optionOff, 400, 357, 0);
				}
			}
			else if (state == CONFIG)
			{
				optionScreen->render();
				optionScreen->selectRender(arrow);

				if (options[SFX])
				{
					al_draw_bitmap(optionOn, 400, 160, 0);
				}
				else
				{
					al_draw_bitmap(optionOff, 400, 160, 0);
				}
				if (options[MUSIC])
				{
					al_draw_bitmap(optionOn, 400, 259, 0);
				}
				else
				{
					al_draw_bitmap(optionOff, 400, 259, 0);
				}
				if (options[FRAMECOUNTER])
				{
					al_draw_bitmap(optionOn, 400, 357, 0);
				}
				else
				{
					al_draw_bitmap(optionOff, 400, 357, 0);
				}
			}
			else if (state == SAVE)
			{
				saveScreen->render();
				saveScreen->selectRender(arrow);
				if (!loadfile1)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 150, 0, "There is no save in this slot.");
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 92, 0, "Stage %i", stage1);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 92, 0, "%i", totalGameTime1);
				}
				else if (loadfile1)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 92, 0, "Stage %i", stage1);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 92, 0, "%i", totalGameTime1);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 150, 0, "\"Stuck already?\"");
				}
				if (!loadfile2)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 280, 0, "There is no save in this slot.");
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 220, 0, "Stage: %i", stage2);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 220, 0, "%i", totalGameTime2);
				}
				else if (loadfile2)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 220, 0, "Stage: %i", stage2);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 220, 0, "%i", totalGameTime2);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 280, 0, "\"Too hard.\"");
				}
				if (!loadfile3)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 410, 0, "There is no save in this slot.");
				}
				else if (loadfile3)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 350, 0, "Stage: %i", stage3);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 350, 0, "%i", totalGameTime3);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 410, 0, "\"60 seconds left.\"");
				}
			}
			else if (state == LOAD1)
			{
				loadScreen->render();
				loadScreen->selectRender(arrow);
				if (!loadfile1)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 150, 0, "There is no save in this slot.");
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 92, 0, "Stage %i", stage1);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 92, 0, "%i", totalGameTime1);
				}
				else if (loadfile1)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 92, 0, "Stage %i", stage1);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 92, 0, "%i", totalGameTime1);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 150, 0, "\"Stuck already?\"");
				}
				if (!loadfile2)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 280, 0, "There is no save in this slot.");
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 220, 0, "Stage: %i", stage2);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 220, 0, "%i", totalGameTime2);
				}
				else if (loadfile2)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 220, 0, "Stage: %i", stage2);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 220, 0, "%i", totalGameTime2);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 280, 0, "\"Too hard.\"");
				}
				if (!loadfile3)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 410, 0, "There is no save in this slot.");
				}
				else if (loadfile3)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 350, 0, "Stage: %i", stage3);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 350, 0, "%i", totalGameTime3);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 410, 0, "\"60 seconds left.\"");
				}
			}
			else if (state == LOAD2)
			{
				loadScreen->render();
				loadScreen->selectRender(arrow);
				if (!loadfile1)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 150, 0, "There is no save in this slot.");
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 92, 0, "Stage %i", stage1);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 92, 0, "%i", totalGameTime1);
				}
				else if (loadfile1)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 92, 0, "Stage %i", stage1);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 92, 0, "%i", totalGameTime1);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 150, 0, "\"Stuck already?\"");
				}
				if (!loadfile2)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 280, 0, "There is no save in this slot.");
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 220, 0, "Stage: %i", stage2);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 220, 0, "%i", totalGameTime2);
				}
				else if (loadfile2)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 220, 0, "Stage: %i", stage2);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 220, 0, "%i", totalGameTime2);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 280, 0, "\"Too hard.\"");
				}
				if (!loadfile3)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 410, 0, "There is no save in this slot.");
				}
				else if (loadfile3)
				{
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 250, 350, 0, "Stage: %i", stage3);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 500, 350, 0, "%i", totalGameTime3);
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 20, 410, 0, "\"60 seconds left.\"");
				}
			}
			else if (state == MAGIC)
			{
				magicScreen->render();
				magicScreen->selectRender(arrow);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 75, 20, 0, "Magic");

				al_draw_textf(font24, al_map_rgb(255, 255, 255), 75, 20, 0, "Magic");
				string conversion = master[magicScreen->getSelect()].getDescription();
				char *magic = new char[20];
				strcpy(magic, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 30, 400, 0, magic);

				conversion = master[0].getName();
				char *magic1 = new char[20];
				strcpy(magic1, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 30, 147, 0, magic1);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 250, 147, 0, "%i", master[0].getCost());

				conversion = master[1].getName();
				char *magic2 = new char[20];
				strcpy(magic2, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 330, 147, 0, magic2);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 550, 147, 0, "%i", master[1].getCost());

				conversion = master[2].getName();
				char *magic3 = new char[20];
				strcpy(magic3, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 30, 192, 0, magic3);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 250, 192, 0, "%i", master[2].getCost());

				conversion = master[3].getName();
				char *magic4 = new char[20];
				strcpy(magic4, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 330, 192, 0, magic4);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 550, 192, 0, "%i", master[3].getCost());

				conversion = master[4].getName();
				char *magic5 = new char[20];
				strcpy(magic5, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 30, 237, 0, magic5);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 250, 237, 0, "%i", master[4].getCost());

				conversion = master[5].getName();
				char *magic6 = new char[20];
				strcpy(magic6, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 330, 237, 0, magic6);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 550, 237, 0, "%i", master[5].getCost());

				conversion = master[6].getName();
				char *magic7 = new char[20];
				strcpy(magic7, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 30, 282, 0, magic7);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 250, 282, 0, "%i", master[6].getCost());

				conversion = master[7].getName();
				char *magic8 = new char[20];
				strcpy(magic8, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 330, 282, 0, magic8);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 550, 282, 0, "%i", master[7].getCost());

				if (keys[SPACE] == true)
				{
					magicSlots->render();
					magicSlots->selectRender(arrow);

					al_draw_textf(font24, al_map_rgb(255, 255, 255), 345, 332, 0, "Slot 1");
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 345, 367, 0, "Slot 2");
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 345, 402, 0, "Slot 3");
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 345, 437, 0, "Slot 4");
				}

			}
			else if (state == EQUIP)
			{
				equipmentScreen->render();
				equipmentScreen->selectRender(arrow);

				string conversion = inv.getDescription(equipmentScreen->getSelect());
				char *equip = new char[100];
				strcpy(equip, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 20, 420, 0, equip);

				conversion = inv.getName(0);
				char *equip1 = new char[20];
				strcpy(equip1, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 45, 20, 0, equip1);

				conversion = inv.getName(1);
				char *equip2 = new char[20];
				strcpy(equip2, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 225, 20, 0, equip2);

				conversion = inv.getName(2);
				char *equip3 = new char[20];
				strcpy(equip3, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 45, 40, 0, equip3);

				conversion = inv.getName(3);
				char *equip4 = new char[20];
				strcpy(equip4, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 225, 40, 0, equip4);

				conversion = inv.getName(4);
				char *equip5 = new char[20];
				strcpy(equip5, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 45, 60, 0, equip5);

				conversion = inv.getName(5);
				char *equip6 = new char[20];
				strcpy(equip6, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 225, 60, 0, equip6);

				conversion = inv.getName(6);
				char *equip7 = new char[20];
				strcpy(equip7, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 45, 80, 0, equip7);

				conversion = inv.getName(7);
				char *equip8 = new char[20];
				strcpy(equip8, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 225, 80, 0, equip8);

				conversion = inv.getName(8);
				char *equip9 = new char[20];
				strcpy(equip9, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 45, 100, 0, equip9);

				conversion = inv.getName(9);
				char *equip10 = new char[20];
				strcpy(equip10, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 225, 100, 0, equip10);

				conversion = inv.getName(10);
				char *equip11 = new char[20];
				strcpy(equip11, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 45, 120, 0, equip11);

				conversion = inv.getName(11);
				char *equip12 = new char[20];
				strcpy(equip12, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 225, 120, 0, equip12);

				conversion = inv.getName(12);
				char *equip13 = new char[20];
				strcpy(equip13, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 45, 140, 0, equip13);

				conversion = inv.getName(13);
				char *equip14 = new char[20];
				strcpy(equip14, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 225, 140, 0, equip14);

				conversion = inv.getName(14);
				char *equip15 = new char[20];
				strcpy(equip15, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 45, 160, 0, equip15);

				conversion = inv.getName(15);
				char *equip16 = new char[20];
				strcpy(equip16, conversion.c_str());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 225, 160, 0, equip16);

				al_draw_textf(font32, al_map_rgb(255, 255, 255), 420, 10, 0, "Equipments");
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 400, 260, 0, "Strength");
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 260, 0, "%i", player->getStrength());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 400, 290, 0, "Intelligence");
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 290, 0, "%i", player->getIntelligence());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 400, 320, 0, "Defense");
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 320, 0, "%i", player->getDefense());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 400, 350, 0, "Magic Defense");
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 350, 0, "%i", player->getMagicDefense());

				conversion = player->getEquipName(0);
				char *equipDisplay = new char[100];
				strcpy(equipDisplay, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 400, 60, 0, equipDisplay);

				conversion = player->getEquipName(1);
				char *equipDisplay1 = new char[100];
				strcpy(equipDisplay1, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 400, 100, 0, equipDisplay1);

				conversion = player->getEquipName(2);
				char *equipDisplay2 = new char[100];
				strcpy(equipDisplay2, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 400, 140, 0, equipDisplay2);

				conversion = player->getEquipName(3);
				char *equipDisplay3 = new char[100];
				strcpy(equipDisplay3, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 400, 180, 0, equipDisplay3);

				conversion.erase();
			}
			else if (state == ITEM)
			{
				itemScreen->render();
				itemScreen->selectRender(arrow);

				string conversion = inv.getDescription(itemScreen->getSelect());
				char *item = new char[100];
				strcpy(item, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 20, 380, 0, item);

				conversion = inv.getName(0);
				char *item1 = new char[20];
				strcpy(item1, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 45, 77, 0, item1);
				if (inv.getQuantity(0) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 77, 0, "%i", inv.getQuantity(0));
				}

				conversion = inv.getName(1);
				char *item2 = new char[20];
				strcpy(item2, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 345, 77, 0, item2);
				if (inv.getQuantity(1) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 77, 0, "%i", inv.getQuantity(1));
				}

				conversion = inv.getName(2);
				char *item3 = new char[20];
				strcpy(item3, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 45, 97, 0, item3);
				if (inv.getQuantity(2) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 97, 0, "%i", inv.getQuantity(2));
				}

				conversion = inv.getName(3);
				char *item4 = new char[20];
				strcpy(item4, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 345, 97, 0, item4);
				if (inv.getQuantity(3) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 97, 0, "%i", inv.getQuantity(3));
				}

				conversion = inv.getName(4);
				char *item5 = new char[20];
				strcpy(item5, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 45, 117, 0, item5);
				if (inv.getQuantity(4) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 117, 0, "%i", inv.getQuantity(4));
				}

				conversion = inv.getName(5);
				char *item6 = new char[20];
				strcpy(item6, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 345, 117, 0, item6);
				if (inv.getQuantity(5) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 117, 0, "%i", inv.getQuantity(5));
				}

				conversion = inv.getName(6);
				char *item7 = new char[20];
				strcpy(item7, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 45, 137, 0, item7);
				if (inv.getQuantity(6) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 137, 0, "%i", inv.getQuantity(6));
				}

				conversion = inv.getName(7);
				char *item8 = new char[20];
				strcpy(item8, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 345, 137, 0, item8);
				if (inv.getQuantity(7) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 137, 0, "%i", inv.getQuantity(7));
				}

				conversion = inv.getName(8);
				char *item9 = new char[20];
				strcpy(item9, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 45, 157, 0, item9);
				if (inv.getQuantity(8) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 157, 0, "%i", inv.getQuantity(8));
				}

				conversion = inv.getName(9);
				char *item10 = new char[20];
				strcpy(item10, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 345, 157, 0, item10);
				if (inv.getQuantity(9) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 157, 0, "%i", inv.getQuantity(9));
				}

				conversion = inv.getName(10);
				char *item11 = new char[20];
				strcpy(item11, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 45, 177, 0, item11);
				if (inv.getQuantity(10) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 177, 0, "%i", inv.getQuantity(10));
				}

				conversion = inv.getName(11);
				char *item12 = new char[20];
				strcpy(item12, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 345, 177, 0, item12);
				if (inv.getQuantity(11) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 177, 0, "%i", inv.getQuantity(11));
				}

				conversion = inv.getName(12);
				char *item13 = new char[20];
				strcpy(item13, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 45, 197, 0, item13);
				if (inv.getQuantity(12) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 197, 0, "%i", inv.getQuantity(12));
				}

				conversion = inv.getName(13);
				char *item14 = new char[20];
				strcpy(item14, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 345, 197, 0, item14);
				if (inv.getQuantity(13) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 197, 0, "%i", inv.getQuantity(13));
				}

				conversion = inv.getName(14);
				char *item15 = new char[20];
				strcpy(item15, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 45, 217, 0, item15);
				if (inv.getQuantity(14) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 300, 217, 0, "%i", inv.getQuantity(14));
				}

				conversion = inv.getName(15);
				char *item16 = new char[20];
				strcpy(item16, conversion.c_str());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 345, 217, 0, item16);
				if (inv.getQuantity(15) != 0)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 600, 217, 0, "%i", inv.getQuantity(15));
				}

				conversion.erase();
			}
			else if (state == PLAYING)
			{
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if ((*iter)->getAlive() == true)
					{
						(*iter)->render();
					}
				}

				if (options[FRAMECOUNTER])
				{
					al_draw_textf(font18, al_map_rgb(255, 255, 0), 5, 5, 0, " FPS: %i", gameFPS);
				}

				al_draw_textf(font48, al_map_rgb(255, 255, 255), 265, 20, 0, "%i.%i", 59 - gameControl, (((((3600 - al_get_timer_count(gameTimer)) * 100) / 60)) % 100));
			}
			else if (state == LEVEL2)
			{
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if ((*iter)->getAlive() == true)
					{
						(*iter)->render();
					}
				}
				if (options[FRAMECOUNTER])
				{
					al_draw_textf(font18, al_map_rgb(255, 255, 0), 5, 5, 0, " FPS: %i", gameFPS);
				}
				al_draw_textf(font48, al_map_rgb(255, 255, 255), 265, 20, 0, "%i.%i", 59 - gameControl, (((((3600 - al_get_timer_count(gameTimer)) * 100) / 60)) % 100));
			}
			else if (state == BATTLE)
			{
				battleScreen->render();
				player->setX((WIDTH * 3) / 4);
				player->setY(HEIGHT / 2);
				if (player->getInsufficient() == true)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 280, 0, "Not enough Energy!");
				}
				if (battleControl->getSelect() == 0 && keys[SPACE] == true)
				{
					battleControl->render();
					battleControl->selectRender(arrow);
				}
				else if (battleControl->getSelect() == 1 && keys[SPACE] == true)
				{
					magicControl->render();
					magicControl->selectRender(arrow);

					string conversion = currentMag[0].getName();
					char magicD1[20];
					strcpy(magicD1, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 345, 0, magicD1);

					conversion = currentMag[1].getName();
					char magicD2[20];
					strcpy(magicD2, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 370, 0, magicD2);

					conversion = currentMag[2].getName();
					char magicD3[20];
					strcpy(magicD3, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 400, 0, magicD3);

					conversion = currentMag[3].getName();
					char magicD4[20];
					strcpy(magicD4, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 430, 0, magicD4);

				}
				else
				{
					battleControl->render();
					battleControl->selectRender(arrow);
				}
				if (player->getDefend())
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 300, 0, "You braced yourself for the next attack!");
				}
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if ((*iter)->getAlive() == true)
					{
						(*iter)->render();
					}
				}
				if (battleLock == true)
				{
					if (battleControl->getSelect() == 3)
					{
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 20, 60, 0, "You could not run away!");
					}
					else
					{
						battleLock = false;
					}
				}
				if (options[FRAMECOUNTER])
				{
					al_draw_textf(font18, al_map_rgb(255, 255, 0), 5, 5, 0, " FPS: %i", gameFPS);
				}
				al_draw_textf(font18, al_map_rgb(255, 255, 0), 5, 20, 0, " Time: %is ", battleSecond % 60);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 320, 340, 0, "Hime   HP %i/%i", player->getHealth(), player->getTotalHealth());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 310, 360, 0, "       MP %i", player->getMP());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 310, 380, 0, "       Energy %i/%i", player->getEnergy(), player->getTotalEnergy());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 150, 180, 0, " HP %i/%i", enemy->getHealth(), enemy->getTotalHealth());
				al_draw_textf(font48, al_map_rgb(255, 255, 255), 265, 20, 0, "%i.%i", 59 - gameControl, (((((3600 - al_get_timer_count(gameTimer)) * 100) / 60)) % 100));

				if (battleControl->getSelect() == 1)
				{
					if (!currentMag[magicControl->getSelect()].getUsed() && keys[SPACE])
					{
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 300, 0, "There is no Spell in this Slot!");
					}
				}
			}
			else if (state == BATTLE2)
			{
				battleScreen2->render();

				player->setX((WIDTH * 3) / 4);
				player->setY(HEIGHT / 2);
				if (player->getInsufficient() == true)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 280, 0, "Not enough Energy!");
				}
				if (battleControl->getSelect() == 0 && keys[SPACE] == true)
				{
					battleControl->render();
					battleControl->selectRender(arrow);
				}
				else if (battleControl->getSelect() == 1 && keys[SPACE] == true)
				{
					magicControl->render();
					magicControl->selectRender(arrow);

					string conversion = currentMag[0].getName();
					char magicD1[20];
					strcpy(magicD1, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 345, 0, magicD1);

					conversion = currentMag[1].getName();
					char magicD2[20];
					strcpy(magicD2, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 370, 0, magicD2);

					conversion = currentMag[2].getName();
					char magicD3[20];
					strcpy(magicD3, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 400, 0, magicD3);

					conversion = currentMag[3].getName();
					char magicD4[20];
					strcpy(magicD4, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 430, 0, magicD4);

				}
				else
				{
					battleControl->render();
					battleControl->selectRender(arrow);
				}
				if (player->getDefend())
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 300, 0, "You braced yourself for the next attack!");
				}
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if ((*iter)->getAlive() == true)
					{
						(*iter)->render();
					}
				}
				if (battleLock == true)
				{
					if (battleControl->getSelect() == 3)
					{
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 20, 60, 0, "You could not run away!");
					}
					else
					{
						battleLock = false;
					}
				}
				if (options[FRAMECOUNTER])
				{
					al_draw_textf(font18, al_map_rgb(255, 255, 0), 5, 5, 0, " FPS: %i", gameFPS);
				}
				al_draw_textf(font18, al_map_rgb(255, 255, 0), 5, 20, 0, " Time: %is ", battleSecond % 60);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 320, 340, 0, "Hime   HP %i/%i", player->getHealth(), player->getTotalHealth());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 310, 360, 0, "       MP %i", player->getMP());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 310, 380, 0, "       Energy %i/%i", player->getEnergy(), player->getTotalEnergy());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 150, 180, 0, " HP %i/%i", enemy->getHealth(), enemy->getTotalHealth());
				al_draw_textf(font48, al_map_rgb(255, 255, 255), 265, 20, 0, "%i.%i", 59 - gameControl, (((((3600 - al_get_timer_count(gameTimer)) * 100) / 60)) % 100));

				if (battleControl->getSelect() == 1)
				{
					if (!currentMag[magicControl->getSelect()].getUsed() && keys[SPACE])
					{
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 300, 0, "There is no Spell in this Slot!");
					}
				}
			}
			else if (state == BOSS)
			{
				bossScreen->render();
				player->setX((WIDTH * 3) / 4);
				player->setY(HEIGHT / 2);
				if (player->getInsufficient() == true)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 280, 0, "Not enough Energy!");
				}
				if (battleControl->getSelect() == 0 && keys[SPACE] == true)
				{
					battleControl->render();
					battleControl->selectRender(arrow);
				}
				else if (battleControl->getSelect() == 1 && keys[SPACE] == true)
				{
					magicControl->render();
					magicControl->selectRender(arrow);

					string conversion = currentMag[0].getName();
					char magicD1[20];
					strcpy(magicD1, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 345, 0, magicD1);

					conversion = currentMag[1].getName();
					char magicD2[20];
					strcpy(magicD2, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 370, 0, magicD2);

					conversion = currentMag[2].getName();
					char magicD3[20];
					strcpy(magicD3, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 400, 0, magicD3);

					conversion = currentMag[3].getName();
					char magicD4[20];
					strcpy(magicD4, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 430, 0, magicD4);

				}
				else
				{
					battleControl->render();
					battleControl->selectRender(arrow);
				}
				if (player->getDefend())
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 300, 0, "You braced yourself for the next attack!");
				}
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if ((*iter)->getAlive() == true)
					{
						(*iter)->render();
					}
				}
				if (battleLock == true)
				{
					if (battleControl->getSelect() == 3)
					{
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 20, 60, 0, "You cannot run away");
					}
				}
				if (options[FRAMECOUNTER])
				{
					al_draw_textf(font18, al_map_rgb(255, 255, 0), 5, 5, 0, " FPS: %i", gameFPS);
				}
				al_draw_textf(font18, al_map_rgb(255, 255, 0), 5, 20, 0, " Time: %is ", battleSecond % 60);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 320, 340, 0, "Hime   HP %i/%i", player->getHealth(), player->getTotalHealth());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 310, 360, 0, "       MP %i", player->getMP());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 310, 380, 0, "       Energy %i/%i", player->getEnergy(), player->getTotalEnergy());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 150, 180, 0, " HP %i/%i", boss->getHealth(), boss->getTotalHealth());

				if (battleControl->getSelect() == 1)
				{
					if (!currentMag[magicControl->getSelect()].getUsed() && keys[SPACE])
					{
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 300, 0, "There is no Spell in this Slot!");
					}
				}
			}	
			else if (state == BOSS2)
			{
				bossScreen2->render();
				player->setX((WIDTH * 3) / 4);
				player->setY(HEIGHT / 2);
				if (player->getInsufficient() == true)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 280, 0, "Not enough Energy!");
				}
				if (battleControl->getSelect() == 0 && keys[SPACE] == true)
				{
					battleControl->render();
					battleControl->selectRender(arrow);
				}
				else if (battleControl->getSelect() == 1 && keys[SPACE] == true)
				{
					magicControl->render();
					magicControl->selectRender(arrow);

					string conversion = currentMag[0].getName();
					char magicD1[20];
					strcpy(magicD1, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 345, 0, magicD1);

					conversion = currentMag[1].getName();
					char magicD2[20];
					strcpy(magicD2, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 370, 0, magicD2);

					conversion = currentMag[2].getName();
					char magicD3[20];
					strcpy(magicD3, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 400, 0, magicD3);

					conversion = currentMag[3].getName();
					char magicD4[20];
					strcpy(magicD4, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 430, 0, magicD4);

				}
				else
				{
					battleControl->render();
					battleControl->selectRender(arrow);
				}
				if (player->getDefend())
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 300, 0, "You braced yourself for the next attack!");
				}
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if ((*iter)->getAlive() == true)
					{
						(*iter)->render();
					}
				}
				if (battleLock == true)
				{
					if (battleControl->getSelect() == 3)
					{
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 20, 60, 0, "You cannot run away");
					}
				}
				if (options[FRAMECOUNTER])
				{
					al_draw_textf(font18, al_map_rgb(255, 255, 0), 5, 5, 0, " FPS: %i", gameFPS);
				}
				al_draw_textf(font18, al_map_rgb(255, 255, 0), 5, 20, 0, " Time: %is ", battleSecond % 60);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 320, 340, 0, "Hime   HP %i/%i", player->getHealth(), player->getTotalHealth());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 310, 360, 0, "       MP %i", player->getMP());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 310, 380, 0, "       Energy %i/%i", player->getEnergy(), player->getTotalEnergy());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 150, 180, 0, " HP %i/%i", boss2->getHealth(), boss2->getTotalHealth());
				al_draw_textf(font48, al_map_rgb(255, 255, 255), 265, 20, 0, "%i.%i", 59 - gameControl, (((((3600 - al_get_timer_count(gameTimer)) * 100) / 60)) % 100));

				if (battleControl->getSelect() == 1)
				{
					if (!currentMag[magicControl->getSelect()].getUsed() && keys[SPACE])
					{
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 300, 0, "There is no Spell in this Slot!");
					}
				}
			}
			else if (state == BOSS3)
			{
				bossScreen->render();
				boss3->bodyRender();
				bossHead->setAlive(true);
				arms->setAlive(true);
				player->setX((WIDTH * 3) / 4);
				player->setY(HEIGHT / 2);
				if (player->getInsufficient() == true)
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 280, 0, "Not enough Energy!");
				}
				if (battleControl->getSelect() == 0 && keys[SPACE] == true)
				{
					battleControl->render();
					battleControl->selectRender(arrow);
				}
				else if (battleControl->getSelect() == 1 && keys[SPACE] == true)
				{
					magicControl->render();
					magicControl->selectRender(arrow);

					string conversion = currentMag[0].getName();
					char magicD1[20];
					strcpy(magicD1, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 345, 0, magicD1);

					conversion = currentMag[1].getName();
					char magicD2[20];
					strcpy(magicD2, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 370, 0, magicD2);

					conversion = currentMag[2].getName();
					char magicD3[20];
					strcpy(magicD3, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 400, 0, magicD3);

					conversion = currentMag[3].getName();
					char magicD4[20];
					strcpy(magicD4, conversion.c_str());
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 130, 430, 0, magicD4);

				}
				else
				{
					battleControl->render();
					battleControl->selectRender(arrow);
				}
				if (player->getDefend())
				{
					al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 300, 0, "You braced yourself for the next attack!");
				}
				for (iter = objects.begin(); iter != objects.end(); ++iter)
				{
					if ((*iter)->getAlive() == true)
					{
						(*iter)->render();
					}
				}
				if (battleLock == true)
				{
					if (battleControl->getSelect() == 3)
					{
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 20, 60, 0, "You cannot run away");
					}
				}
				if (options[FRAMECOUNTER])
				{
					al_draw_textf(font18, al_map_rgb(255, 255, 0), 5, 5, 0, " FPS: %i", gameFPS);
				}
				al_draw_textf(font18, al_map_rgb(255, 255, 0), 5, 20, 0, " Time: %is ", battleSecond % 60);
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 320, 340, 0, "Hime   HP %i/%i", player->getHealth(), player->getTotalHealth());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 310, 360, 0, "       MP %i", player->getMP());
				al_draw_textf(font24, al_map_rgb(255, 255, 255), 310, 380, 0, "       Energy %i/%i", player->getEnergy(), player->getTotalEnergy());
				al_draw_textf(font18, al_map_rgb(255, 255, 255), 150, 100, 0, " HP %i/%i", boss3->getHealth(), boss3->getTotalHealth());

				if (battleControl->getSelect() == 1)
				{
					if (!currentMag[magicControl->getSelect()].getUsed() && keys[SPACE])
					{
						al_draw_textf(font24, al_map_rgb(255, 255, 255), 50, 300, 0, "There is no Spell in this Slot!");
					}
				}
			}
			else if (state == VICTORY)
			{
				victoryScreen->render();
				if (lastState == BATTLE)
				{
					al_draw_textf(font32, al_map_rgb(0, 0, 0), 200, 160, 0, "%i", enemy->getMoneyValue());
					al_draw_textf(font32, al_map_rgb(0, 0, 0), 200, 105, 0, "%i", enemy->getExpValue());
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 420, 425, 0, "%i", player->getMoney());
				}
				else if (lastState == BATTLE2)
				{
					al_draw_textf(font32, al_map_rgb(0, 0, 0), 200, 160, 0, "%i", enemy->getMoneyValue());
					al_draw_textf(font32, al_map_rgb(0, 0, 0), 200, 105, 0, "%i", enemy->getExpValue());
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 420, 425, 0, "%i", player->getMoney());
				}
				else if (lastState == BOSS)
				{
					al_draw_textf(font32, al_map_rgb(0, 0, 0), 200, 160, 0, "%i", boss->getMoneyValue());
					al_draw_textf(font32, al_map_rgb(0, 0, 0), 200, 105, 0, "%i", boss->getExpValue());
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 420, 425, 0, "%i", player->getMoney());
				}
				else if (lastState == BOSS2)
				{
					al_draw_textf(font32, al_map_rgb(0, 0, 0), 200, 160, 0, "%i", boss2->getMoneyValue());
					al_draw_textf(font32, al_map_rgb(0, 0, 0), 200, 105, 0, "%i", boss2->getExpValue());
					al_draw_textf(font32, al_map_rgb(255, 255, 255), 420, 425, 0, "%i", player->getMoney());
				}
			}
			else if (state == LOST)
			{
				lostScreen->render();
				lostScreen->selectRender(arrow);
			}
			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}


	//Iterator that Destorys objects
	for (iter = objects.begin(); iter != objects.end();)
	{
		(*iter)->destroy();
		delete (*iter);
		iter = objects.erase(iter);
	}

	delete titleScreen1;
	delete titleScreen2;
	delete battleScreen;
	delete menuScreen;
	delete lostScreen;
	delete battleControl;
	delete statusScreen;
	delete optionScreen;
	delete loadScreen;
	delete saveScreen;
	delete magicScreen;
	delete equipmentScreen;
	delete itemScreen;
	delete victoryScreen;
	delete magicControl;
	delete magicSlots;
	delete worldScreen;
	delete shopMenu;
	delete shopControl;
	delete buysellEquipment;
	delete buysellItem;
	delete gambleScreen;
	delete buy;
	delete sell;


	


	//Destroy Project Objects
	al_destroy_bitmap(playerImage);
	al_destroy_bitmap(enemyImage);
	al_destroy_bitmap(bgImage);
	al_destroy_bitmap(animationImage);
	al_destroy_bitmap(battleImage);
	al_destroy_bitmap(titleImage1);
	al_destroy_bitmap(titleImage2);
	al_destroy_bitmap(lostImage);
	al_destroy_bitmap(menuImage);
	al_destroy_bitmap(statusImage);
	al_destroy_bitmap(optionImage);
	al_destroy_bitmap(battleCommand);
	al_destroy_bitmap(magicCommand);
	al_destroy_bitmap(saveImage);
	al_destroy_bitmap(loadImage);
	al_destroy_bitmap(magicImage);
	al_destroy_bitmap(optionOff);
	al_destroy_bitmap(optionOn);
	al_destroy_bitmap(arrow);
	al_destroy_bitmap(equipmentImage);
	al_destroy_bitmap(itemImage);
	al_destroy_bitmap(victoryImage);
	al_destroy_bitmap(slashImage);
	al_destroy_bitmap(fireImage);
	al_destroy_bitmap(iceImage);
	al_destroy_bitmap(lightningImage);
	al_destroy_bitmap(windImage);
	al_destroy_bitmap(defenseImage);
	al_destroy_bitmap(slotImage);
	al_destroy_bitmap(worldImage);
	al_destroy_bitmap(shopImage);
	al_destroy_bitmap(shopControlImage);
	al_destroy_bitmap(playerArrow);
	al_destroy_bitmap(buysellEquipmentImage);
	al_destroy_bitmap(buysellItemImage);
	al_destroy_bitmap(gambleImage);

	al_destroy_sample(song);
	al_destroy_sample(selectSound);
	al_destroy_sample(battleSong);
	al_destroy_sample(themeSong);
	al_destroy_sample(gameOver);
	al_destroy_sample(victorySong);
	al_destroy_sample(saveloadSong);
	al_destroy_sample(attackSound);
	al_destroy_sample(fleeSound);
	al_destroy_sample(fireSound);
	al_destroy_sample(iceSound);
	al_destroy_sample(thunderSound);
	al_destroy_sample(worldSound);
	al_destroy_sample(shopSound);

	al_destroy_sample_instance(songInstance);
	al_destroy_sample_instance(changeInstance);
	al_destroy_sample_instance(selectInstance);
	al_destroy_sample_instance(battleInstance);
	al_destroy_sample_instance(themeInstance);
	al_destroy_sample_instance(gameOverInstance);
	al_destroy_sample_instance(victoryInstance);
	al_destroy_sample_instance(saveloadInstance);
	al_destroy_sample_instance(attackInstance);
	al_destroy_sample_instance(fleeInstance);
	al_destroy_sample_instance(fireInstance);
	al_destroy_sample_instance(thunderInstance);
	al_destroy_sample_instance(iceInstance);
	al_destroy_sample_instance(worldInstance);
	al_destroy_sample_instance(shopInstance);
	al_destroy_sample_instance(menuInstance);

	//Shell Object Destroy
	al_destroy_font(font18);
	al_destroy_font(font24);
	al_destroy_font(font32);
	al_destroy_font(font48);

	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_timer(battleTimer);
	al_destroy_timer(gameTimer);
	al_destroy_display(display);

	return 0;
}

void changeState(int &state, int newState)
{
	if (state == TITLE2)
	{
		bg2->setAlive(false);
		player->init();
	}
	else if (state == PLAYING)
	{
		for (iter = objects.begin(); iter != objects.end(); iter++)
		{
			if (((*iter)->getID() != PLAYER) && ((*iter)->getID() != MISC))
			{
				(*iter)->setAlive(false);
			}
		}
	}
	else if (state == LEVEL2)
	{
		for (iter = objects.begin(); iter != objects.end(); iter++)
		{
			if (((*iter)->getID() != PLAYER) && ((*iter)->getID() != MISC))
			{
				(*iter)->setAlive(false);
			}
		}
	}
	else if (state == BATTLE)
	{
		player->setEnergy();
		player->setAttacking(false);
		al_stop_timer(battleTimer);
		al_set_timer_count(battleTimer, 0);
		for (iter = objects.begin(); iter != objects.end(); iter++)
		{
			if (((*iter)->getID() != PLAYER) && ((*iter)->getID() != MISC))
			{
				(*iter)->setAlive(false);
			}
		}
		al_stop_sample_instance(battleInstance);
	}
	else if (state == BATTLE2)
	{
		player->setEnergy();
		player->setAttacking(false);
		al_stop_timer(battleTimer);
		bg2->setAlive(true);
		al_set_timer_count(battleTimer, 0);
		for (iter = objects.begin(); iter != objects.end(); iter++)
		{
			if (((*iter)->getID() != PLAYER) && ((*iter)->getID() != MISC))
			{
				(*iter)->setAlive(false);
			}
		}
		al_stop_sample_instance(battleInstance);
	}
	else if (state == BOSS)
	{
		player->setEnergy();
		player->setAttacking(false);
		al_stop_timer(battleTimer);
		boss->setY(boss->getY() + 50);
		boss->setX(100);
		bosspos = 200;
		al_set_timer_count(battleTimer, 0);
		al_stop_sample_instance(squidbossInstance);
		al_stop_sample_instance(squidwalkInstance);
		for (iter = objects.begin(); iter != objects.end(); iter++)
		{
			if (((*iter)->getID() != PLAYER) && ((*iter)->getID() != MISC))
			{
				(*iter)->setAlive(false);
			}
		}
		al_stop_sample_instance(squidbossInstance);
	}
	else if (state == BOSS2)
	{
		player->setEnergy();
		player->setAttacking(false);
		al_stop_timer(battleTimer);
		boss2->setY(boss2->getY() + 50);
		boss2->setX(100);
		bosspos2 = 400;
		al_set_timer_count(battleTimer, 0);
		for (iter = objects.begin(); iter != objects.end(); iter++)
		{
			if (((*iter)->getID() != PLAYER) && ((*iter)->getID() != MISC))
			{
				(*iter)->setAlive(false);
			}
		}
		al_stop_sample_instance(dragonInstance);
	}
	else if (state == BOSS3)
	{
		player->setEnergy();
		player->setAttacking(false);
		al_stop_timer(battleTimer);
		boss3->setY(boss3->getY() + 50);
		boss3->setX(300);
		al_set_timer_count(battleTimer, 0);
		for (iter = objects.begin(); iter != objects.end(); iter++)
		{
			if (((*iter)->getID() != PLAYER) && ((*iter)->getID() != MISC))
			{
				(*iter)->setAlive(false);
			}
		}
		al_stop_sample_instance(dragonInstance);
	}
	else if (state == LOST)
	{
		al_set_timer_count(gameTimer, 0);
		bg->setAlive(true);
		al_stop_sample_instance(gameOverInstance);
	}
	else if (state == VICTORY)
	{
		al_stop_sample_instance(victoryInstance);
	}
	else if(state == STATUS)
	{
	}
	else if (state == SAVE)
	{
		al_stop_sample_instance(saveloadInstance);
	}
	else if (state == LOAD1)
	{
		player->setHp(player->getTotalHealth());
		player->setMp((player->getTotalMP()));
		al_stop_sample_instance(saveloadInstance);
	}
	else if (state == LOAD2)
	{
		player->setHp(player->getTotalHealth());
		player->setMp((player->getTotalMP()));
		al_stop_sample_instance(saveloadInstance);
	}
	else if (state == SHOP)
	{
	}
	else if (state == WORLD)
	{
		al_start_timer(elapsed);
	}

	lastState = state;
	state = newState;


	if (state == TITLE1)
	{
		gameControl = 0;
		if (options[MUSIC])
		{
			al_play_sample_instance(themeInstance);
		}
	}
	else if (state == CONVERSATION)
	{
		al_stop_sample_instance(themeInstance);
	}
	else if (state == WORLD)
	{
		al_stop_sample_instance(shopInstance);
		al_stop_sample_instance(themeInstance);
		al_stop_sample_instance(songInstance);
		al_stop_sample_instance(leveltwoInstance);
		al_set_timer_count(gameTimer, 0);
		if (options[MUSIC])
		{
			al_play_sample_instance(worldInstance);
		}
	}
	else if (state == PLAYING)
	{
		stage = 1;
		al_stop_sample_instance(worldInstance);
		al_start_timer(gameTimer);
		bg->setAlive(true);
		bg2->setAlive(false);
		al_stop_sample_instance(themeInstance);
		levelState = PLAYING;
		if (options[MUSIC])
		{
			al_play_sample_instance(songInstance);
		}
	}
	else if (state == LEVEL2)
	{
		al_stop_sample_instance(worldInstance);
		al_start_timer(gameTimer);
		bg->setAlive(false);
		bg2->setAlive(true);
		stage = 2;
		levelState = LEVEL2;
		al_stop_sample_instance(themeInstance);
		if (options[MUSIC])
		{
			al_play_sample_instance(leveltwoInstance);
		}
	}
	else if (state == SHOP)
	{
		al_stop_sample_instance(worldInstance);
		if (options[MUSIC])
		{
			al_play_sample_instance(shopInstance);
		}
	}
	else if (state == MAIN)
	{
		if (options[MUSIC])
		{
			if (levelState == PLAYING)
			{
				al_play_sample_instance(songInstance);
			}
			else if (levelState == LEVEL2)
			{
				al_play_sample_instance(leveltwoInstance);
			}
		}
	}
	else if (state == VICTORY)
	{
		if (options[MUSIC])
		{
			al_play_sample_instance(victoryInstance);
		}
	}
	else if (state == BATTLE)
	{
		keys[SPACE] = false;
		player->setBattle();
		al_start_timer(battleTimer);
		randomEncounter = 0;
		al_stop_sample_instance(songInstance);
		enemy->init();
		bg->setAlive(false);
		if (options[MUSIC])
		{
			al_play_sample_instance(battleInstance);
		}
	}
	else if (state == BATTLE2)
	{
		keys[SPACE] = false;
		player->setBattle();
		al_start_timer(battleTimer);
		randomEncounter = 0;
		al_stop_sample_instance(leveltwoInstance);
		enemy->init();
		bg2->setAlive(false);
		if (options[MUSIC])
		{
			al_play_sample_instance(battleInstance);
		}
	}
	else if (state == BOSS)
	{
		keys[SPACE] = false;
		player->setBattle();
		al_stop_timer(gameTimer);
		al_start_timer(battleTimer);
		randomEncounter = 0;
		al_stop_sample_instance(songInstance);
		boss->init(2000, 50, 30, 20, 2000, 700, 2000, 11, 12, 96, 96);
		boss->setY(boss->getY() + 50);
		boss->setX(100);
		bosspos = 200;
		bg->setAlive(false);
		walkback = false;
		if (options[MUSIC])
		{
			al_play_sample_instance(squidbossInstance);
		}
	}
	else if (state == BOSS2)
	{
		keys[SPACE] = false;
		player->setBattle();
		al_start_timer(battleTimer);
		randomEncounter = 0;
		al_stop_sample_instance(leveltwoInstance);
		boss2->init(1000, 75, 45, 50, 500, 700, 1000, 11, 12, 96, 96);
		boss2->setY(boss2->getY() + 50);
		boss2->setX(100);
		bosspos2 = 400;
		bg2->setAlive(false);
		walkback2 = false;
		if (options[MUSIC])
		{
			al_play_sample_instance(dragonInstance);
		}
	}
	else if (state == BOSS3)
	{
		keys[SPACE] = false;
		player->setBattle();
		al_start_timer(battleTimer);
		al_stop_timer(gameTimer);
		randomEncounter = 0;
		arms->setAlive(false);
		bossHead->setAlive(false);
		boss3->init(4000, 75, 99, 50, 2000, 1700, 4000, 1, 6, 240, 170);
		boss3->setAnimationColumns(0);
		boss3->setMaxFrame(0);
		boss3->setCurrentFrame(0);
		boss3->setX(300);
		boss3->setY(300);
		bg2->setAlive(false);
	}
	else if (state == BOSSTALK)
	{
		keys[SPACE] = false;
		player->setBattle();
		randomEncounter = 0;
		al_stop_sample_instance(leveltwoInstance);
		bg2->setAlive(false);
		bossConversation = 0;
		if (options[MUSIC])
		{
			al_play_sample_instance(lastBossInstance);
		}
	}
	else if (state == CREDIT)
	{
		credits->setY(0);
		bossHead->setAlive(false);
		arms->setAlive(false);
		al_stop_timer(gameTimer);
		al_stop_sample_instance(dragonInstance);
		conversationEnd = 0;
		if (options[MUSIC])
		{
			al_play_sample_instance(themeInstance);
		}
	}
	else if (state == LOST)
	{
		if (lastState == MAIN)
		{
			al_stop_sample_instance(songInstance);
		}
		else if (lastState == PLAYING)
		{
			al_stop_sample_instance(songInstance);
		}
		else if (lastState == BATTLE)
		{
			al_stop_sample_instance(battleInstance);
		}
		else if (lastState == BOSS)
		{
			al_stop_sample_instance(squidbossInstance);
		}
		else if (lastState == BOSS2)
		{
			al_stop_sample_instance(dragonInstance);
		}
		else if (lastState == BOSS3)
		{
			al_stop_sample_instance(lastBossInstance);
		}
		if (options[MUSIC])
		{
			al_play_sample_instance(gameOverInstance);
		}
	}
	else if (state == WORLD_MAIN)
	{
		if (options[MUSIC])
		{
			al_play_sample_instance(worldInstance);
		}
	}
	else if (state == STATUS)
	{
	}
	else if (state == SAVE)
	{
		checkload(0);
		checkload(1);
		checkload(2);
		if (lastState == WORLD_MAIN)
		{
			al_stop_sample_instance(worldInstance);
		}
		else if (lastState == MAIN)
		{
			al_stop_sample_instance(songInstance);
		}
		if (options[MUSIC])
		{
			al_play_sample_instance(saveloadInstance);
		}
	}
	else if (state == LOAD1)
	{
		checkload(0);
		checkload(1);
		checkload(2);
		al_stop_sample_instance(themeInstance);
		if (options[MUSIC])
		{
			al_play_sample_instance(saveloadInstance);
		}
	}
	else if (state == LOAD2)
	{
		checkload(0);
		checkload(1);
		checkload(2);
		if (lastState == WORLD_MAIN)
		{
			al_stop_sample_instance(worldInstance);
		}
		else if (lastState == MAIN)
		{
			al_stop_sample_instance(songInstance);
			al_stop_sample_instance(leveltwoInstance);
		}
		if (options[MUSIC])
		{
			al_play_sample_instance(saveloadInstance);
		}
	}
}

void save(int save)
{
	if (save == 0)
	{
		ofstream file("save1.txt");
		file << player->getLevel() << endl << player->getMoney()
			<< endl << player->getExp() << endl << player->getTotalExp() << endl << elapsedControl << endl << stage << endl
			<< magic1 << endl << magic2 << endl << magic3 << endl << magic4 << endl << inv.getMasterNumber(0) << endl << inv.getQuantity(0)
			<< endl << inv.getMasterNumber(1) << endl << inv.getQuantity(1) << endl << inv.getMasterNumber(2) << endl << inv.getQuantity(2)
			<< endl << inv.getMasterNumber(3) << endl << inv.getQuantity(3) << endl << inv.getMasterNumber(4) << endl << inv.getQuantity(4)
			<< endl << inv.getMasterNumber(5) << endl << inv.getQuantity(5) << endl << inv.getMasterNumber(6) << endl << inv.getQuantity(6)
			<< endl << inv.getMasterNumber(7) << endl << inv.getQuantity(7) << endl << inv.getMasterNumber(8) << endl << inv.getQuantity(8)
			<< endl << inv.getMasterNumber(9) << endl << inv.getQuantity(9) << endl << inv.getMasterNumber(10) << endl << inv.getQuantity(10)
			<< endl << inv.getMasterNumber(11) << endl << inv.getQuantity(11) << endl << inv.getMasterNumber(12) << endl << inv.getQuantity(12)
			<< endl << inv.getMasterNumber(13) << endl << inv.getQuantity(13) << endl << inv.getMasterNumber(14) << endl << inv.getQuantity(14)
			<< endl << inv.getMasterNumber(15) << endl << inv.getQuantity(15) << endl << inv.playerFromMaster(player, 0)
			<< endl << inv.playerFromMaster(player, 1) << endl << inv.playerFromMaster(player, 2) << endl << inv.playerFromMaster(player, 3) << endl
			<< player->getNextLevel();

	}
	else if (save == 1)
	{
		ofstream file("save2.txt");
		file << player->getLevel() << endl << player->getMoney()
			<< endl << player->getExp() << endl << player->getTotalExp() << endl << elapsedControl << endl << stage << endl
			<< magic1 << endl << magic2 << endl << magic3 << endl << magic4 << endl << inv.getMasterNumber(0) << endl << inv.getQuantity(0)
			<< endl << inv.getMasterNumber(1) << endl << inv.getQuantity(1) << endl << inv.getMasterNumber(2) << endl << inv.getQuantity(2)
			<< endl << inv.getMasterNumber(3) << endl << inv.getQuantity(3) << endl << inv.getMasterNumber(4) << endl << inv.getQuantity(4)
			<< endl << inv.getMasterNumber(5) << endl << inv.getQuantity(5) << endl << inv.getMasterNumber(6) << endl << inv.getQuantity(6)
			<< endl << inv.getMasterNumber(7) << endl << inv.getQuantity(7) << endl << inv.getMasterNumber(8) << endl << inv.getQuantity(8)
			<< endl << inv.getMasterNumber(9) << endl << inv.getQuantity(9) << endl << inv.getMasterNumber(10) << endl << inv.getQuantity(10)
			<< endl << inv.getMasterNumber(11) << endl << inv.getQuantity(11) << endl << inv.getMasterNumber(12) << endl << inv.getQuantity(12)
			<< endl << inv.getMasterNumber(13) << endl << inv.getQuantity(13) << endl << inv.getMasterNumber(14) << endl << inv.getQuantity(14)
			<< endl << inv.getMasterNumber(15) << endl << inv.getQuantity(15) << endl << inv.playerFromMaster(player, 0)
			<< endl << inv.playerFromMaster(player, 1) << endl << inv.playerFromMaster(player, 2) << endl << inv.playerFromMaster(player, 3)
			<< endl << player->getNextLevel();;

	}
	else if (save == 2)
	{
		ofstream file("save3.txt");
		file << player->getLevel() << endl << player->getMoney()
			<< endl << player->getExp() << endl << player->getTotalExp() << endl << elapsedControl << endl << stage << endl
			<< magic1 << endl << magic2 << endl << magic3 << endl << magic4 << endl << inv.getMasterNumber(0) << endl << inv.getQuantity(0)
			<< endl << inv.getMasterNumber(1) << endl << inv.getQuantity(1) << endl << inv.getMasterNumber(2) << endl << inv.getQuantity(2)
			<< endl << inv.getMasterNumber(3) << endl << inv.getQuantity(3) << endl << inv.getMasterNumber(4) << endl << inv.getQuantity(4)
			<< endl << inv.getMasterNumber(5) << endl << inv.getQuantity(5) << endl << inv.getMasterNumber(6) << endl << inv.getQuantity(6)
			<< endl << inv.getMasterNumber(7) << endl << inv.getQuantity(7) << endl << inv.getMasterNumber(8) << endl << inv.getQuantity(8)
			<< endl << inv.getMasterNumber(9) << endl << inv.getQuantity(9) << endl << inv.getMasterNumber(10) << endl << inv.getQuantity(10)
			<< endl << inv.getMasterNumber(11) << endl << inv.getQuantity(11) << endl << inv.getMasterNumber(12) << endl << inv.getQuantity(12)
			<< endl << inv.getMasterNumber(13) << endl << inv.getQuantity(13) << endl << inv.getMasterNumber(14) << endl << inv.getQuantity(14)
			<< endl << inv.getMasterNumber(15) << endl << inv.getQuantity(15) << endl << inv.playerFromMaster(player, 0)
			<< endl << inv.playerFromMaster(player, 1) << endl << inv.playerFromMaster(player, 2) << endl << inv.playerFromMaster(player, 3)
			<< endl << player->getNextLevel();
	}
}

void load(int input)
{
	if (input == 0)
	{
		ifstream infile("save1.txt");
		if (infile.is_open())
		{
			string line;
			vector<int> v;
			while (getline(infile, line))
			{
				istringstream iss(line);
				int n;

				while (iss >> n)
				{
					v.push_back(n);
				}
			}
			player->setLevel(v[0]);
			player->setGold(v[1]);
			player->setExp(v[2]);
			player->setTotalExp(v[3]);
			totalGameTime1 = (v[4]);
			al_set_timer_count(elapsed, totalGameTime1 * 60);
			stage = (v[5]);
			currentMag[0] = master[v[6]];
			currentMag[0].setUsed(true);
			currentMag[1] = master[v[7]];
			currentMag[1].setUsed(true);
			currentMag[2] = master[v[8]];
			currentMag[2].setUsed(true);
			currentMag[3] = master[v[9]];
			currentMag[3].setUsed(true);
			inv.addFromMaster(0, v[10], v[11]);
			inv.addFromMaster(1, v[12], v[13]);
			inv.addFromMaster(2, v[14], v[15]);
			inv.addFromMaster(3, v[16], v[17]);
			inv.addFromMaster(4, v[18], v[19]);
			inv.addFromMaster(5, v[20], v[21]);
			inv.addFromMaster(6, v[22], v[23]);
			inv.addFromMaster(7, v[24], v[25]);
			inv.addFromMaster(8, v[26], v[27]);
			inv.addFromMaster(9, v[28], v[29]);
			inv.addFromMaster(10, v[30], v[31]);
			inv.addFromMaster(11, v[32], v[33]);
			inv.addFromMaster(12, v[34], v[35]);
			inv.addFromMaster(13, v[36], v[37]);
			inv.addFromMaster(14, v[38], v[39]);
			inv.addFromMaster(15, v[40], v[41]);
			inv.equipFromMaster(player, v[42]);
			inv.equipFromMaster(player, v[43]);
			inv.equipFromMaster(player, v[44]);
			inv.equipFromMaster(player, v[45]);
			player->setNextLevel(v[46]);

			if (stage == 2)
			{
				level2 = true;
			}
			else
			{
				level2 = false;
			}
			infile.close();
		}
	}
	else if (input == 1)
	{
		ifstream infile("save2.txt");
		string line;
		vector<int> v;
		while (getline(infile, line))
		{
			istringstream iss(line);
			int n;

			while (iss >> n)
			{
				v.push_back(n);
			}
		}
		player->setLevel(v[0]);
		player->setGold(v[1]);
		player->setExp(v[2]);
		player->setTotalExp(v[3]);
		totalGameTime2 = (v[4]);
		al_set_timer_count(elapsed, totalGameTime2 * 60);
		stage = (v[5]);
		currentMag[0] = master[v[6]];
		currentMag[0].setUsed(true);
		currentMag[1] = master[v[7]];
		currentMag[1].setUsed(true);
		currentMag[2] = master[v[8]];
		currentMag[2].setUsed(true);
		currentMag[3] = master[v[9]];
		currentMag[3].setUsed(true);
		inv.addFromMaster(0, v[10], v[11]);
		inv.addFromMaster(1, v[12], v[13]);
		inv.addFromMaster(2, v[14], v[15]);
		inv.addFromMaster(3, v[16], v[17]);
		inv.addFromMaster(4, v[18], v[19]);
		inv.addFromMaster(5, v[20], v[21]);
		inv.addFromMaster(6, v[22], v[23]);
		inv.addFromMaster(7, v[24], v[25]);
		inv.addFromMaster(8, v[26], v[27]);
		inv.addFromMaster(9, v[28], v[29]);
		inv.addFromMaster(10, v[30], v[31]);
		inv.addFromMaster(11, v[32], v[33]);
		inv.addFromMaster(12, v[34], v[35]);
		inv.addFromMaster(13, v[36], v[37]);
		inv.addFromMaster(14, v[38], v[39]);
		inv.addFromMaster(15, v[40], v[41]);
		inv.equipFromMaster(player, v[42]);
		inv.equipFromMaster(player, v[43]);
		inv.equipFromMaster(player, v[44]);
		inv.equipFromMaster(player, v[45]);
		player->setNextLevel(v[46]);

		if (stage == 2)
		{
			level2 = true;
		}
		else
		{
			level2 = false;
		}
		infile.close();
	}
	else if (input == 2)
	{
		ifstream infile("save3.txt");
		string line;
		vector<int> v;

		while (getline(infile, line))
		{
			istringstream iss(line);
			int n;

			while (iss >> n)
			{
				v.push_back(n);
			}
		}
		player->setLevel(v[0]);
		player->setGold(v[1]);
		player->setExp(v[2]);
		player->setTotalExp(v[3]);
		totalGameTime3 = (v[4]);
		al_set_timer_count(elapsed, totalGameTime3 * 60.0);
		stage = (v[5]);
		currentMag[0] = master[v[6]];
		currentMag[0].setUsed(true);
		currentMag[1] = master[v[7]];
		currentMag[1].setUsed(true);
		currentMag[2] = master[v[8]];
		currentMag[2].setUsed(true);
		currentMag[3] = master[v[9]];
		currentMag[3].setUsed(true);
		inv.addFromMaster(0, v[10], v[11]);
		inv.addFromMaster(1, v[12], v[13]);
		inv.addFromMaster(2, v[14], v[15]);
		inv.addFromMaster(3, v[16], v[17]);
		inv.addFromMaster(4, v[18], v[19]);
		inv.addFromMaster(5, v[20], v[21]);
		inv.addFromMaster(6, v[22], v[23]);
		inv.addFromMaster(7, v[24], v[25]);
		inv.addFromMaster(8, v[26], v[27]);
		inv.addFromMaster(9, v[28], v[29]);
		inv.addFromMaster(10, v[30], v[31]);
		inv.addFromMaster(11, v[32], v[33]);
		inv.addFromMaster(12, v[34], v[35]);
		inv.addFromMaster(13, v[36], v[37]);
		inv.addFromMaster(14, v[38], v[39]);
		inv.addFromMaster(15, v[40], v[41]);
		inv.equipFromMaster(player, v[42]);
		inv.equipFromMaster(player, v[43]);
		inv.equipFromMaster(player, v[44]);
		inv.equipFromMaster(player, v[45]);
		player->setNextLevel(v[46]);

		if (stage == 2)
		{
			level2 = true;
		}
		else
		{
			level2 = false;
		}
		infile.close();
	}
}
void checkload(int input)
{
	if (input == 0)
	{
		ifstream infile("save1.txt");

		if (infile.is_open())
		{
			string line;
			vector<int> v;
			while (getline(infile, line))
			{
				istringstream iss(line);
				int n;

				while (iss >> n)
				{
					v.push_back(n);
				}
			}
			totalGameTime1 = (v[4]);
			stage1 = (v[5]);
			infile.close();
			loadfile1 = true;
		}
		else
		{
			loadfile1 = false;
		}
	}
	else if (input == 1)
	{
		ifstream infile("save2.txt");
		if (infile.is_open())
		{
			string line;
			vector<int> v;
			while (getline(infile, line))
			{
				istringstream iss(line);
				int n;

				while (iss >> n)
				{
					v.push_back(n);
				}
			}
			totalGameTime2 = (v[4]);
			stage2 = (v[5]);
			infile.close();
			loadfile2 = true;
		}
		else
		{
			loadfile2 = false;
		}
	}
	else if (input == 2)
	{
		ifstream infile("save3.txt");
		if (infile.is_open())
		{
			loadfile3 = true;
			string line;
			vector<int> v;
			while (getline(infile, line))
			{
				istringstream iss(line);
				int n;

				while (iss >> n)
				{
					v.push_back(n);
				}
			}
			totalGameTime3 = (v[4]);
			stage3 = (v[5]);
			infile.close();
		}
		else
		{
			loadfile3 = false;
		}
	}
}