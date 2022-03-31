#define GAME_STRING_SIZE	32

char sWEAPON[GAME_STRING_SIZE];
char sARMOR[GAME_STRING_SIZE];
char sMISC[GAME_STRING_SIZE];
char sENEMY[GAME_STRING_SIZE];
char sSAY[GAME_STRING_SIZE];

int  gameFirstTime;
u8	 gameStatus;
int  GOLD;
int	 ATK;
int  ARM;
int  UP;
int  CURRXP;
int  CURRHP;
int  M, X, Y;	
int  STARTHP;

// ENEMY
int  HPLO;
int  AX;
int  GLDO;
int  XPO;
int  Q;

	
enum gameStatusEnum
{
	STAT_TITLE = 1,
	STAT_INSTRUCTIONS,
	STAT_GAMEMENU,
	STAT_DEAD_SCREEN,
	STAT_ENDGAME_SCREEN,
	STAT_MAIN_SCREEN,
	STAT_FOREST_PATROL,
	STAT_FOREST_ENEMY,
	STAT_FOREST_ENEMY_RUN,
	STAT_FOREST_ENEMY_FIGHT,
	STAT_FOREST_ENEMY_VICTORY,
	STAT_SHOP_SCREEN,
	STAT_SHOP_INPUT,
	STAT_TALK_VILLAGERS,
	STAT_KOSHEY,
	STAT_KOSHEY_FINALBATTLE
};


void game_setup()
{
	gameStatus = STAT_TITLE;
	gameFirstTime = 1;
}


void game_initvars()
{
	gameStatus = STAT_TITLE;
	
	sWEAPON[0] = 0;
	sARMOR[0] = 0;
	sMISC[0] = 0;
	sENEMY[0] = 0;
	sSAY[0] = 0;
	
	GOLD = 10;
	ATK = 0;
	ARM = 0;
	UP = 0;
	CURRXP = 0;
	CURRHP = 10;
	X=1; M=10; Y=11;
}


#include "game_music.h"
#include "game_misc.h"
#include "game_title.h"
#include "game_shop.h"
#include "game_main.h"
#include "game_forest.h"
#include "game_talk_villagers.h"
#include "game_koshey.h"



void game_loop() 
{
	switch (gameStatus)
	{
		case STAT_TITLE:
			{
				game_initvars();
				game_title();
			}
			break;

		case STAT_INSTRUCTIONS: 	game_instructions(); 	break;
		case STAT_GAMEMENU:			game_menu();		 	break;
		case STAT_DEAD_SCREEN:		game_dead_screen();		break;
		case STAT_ENDGAME_SCREEN: 	game_endgame_screen();	break;


		case STAT_MAIN_SCREEN:
			game_main_screen(0);
			break;

		case STAT_FOREST_PATROL:
			game_forest_patrol();
			break;

		case STAT_FOREST_ENEMY:
			game_forest_enemy(1);
			break;

		case STAT_FOREST_ENEMY_RUN:
			game_forest_enemy_run();
			break;

		case STAT_FOREST_ENEMY_FIGHT:
			game_forest_enemy_fight();
			break;

		case STAT_FOREST_ENEMY_VICTORY:
			game_forest_enemy_victory();
			break;

		case STAT_SHOP_SCREEN:
			game_shop();
			break;

		case STAT_SHOP_INPUT:
			game_shop_input();
			break;
			
		case STAT_TALK_VILLAGERS:
			game_talk_villagers();
			break;
			
		case STAT_KOSHEY:
			game_koshey();
			break;
			
		case STAT_KOSHEY_FINALBATTLE:
			game_koshey_finalbattle();
			break;

	}
	
}

