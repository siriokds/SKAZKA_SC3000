void game_forest_enemy(u8 clear_screen);

void ENEMY_BRIGAND() 	 { STRCPY(sENEMY,"RAZBOYNIK"); 		HPLO = 10; AX = 1; GLDO =  5; XPO =  2; }
void ENEMY_HUNGRY_WOLF() { STRCPY(sENEMY,"HUNGRY WOLF");	HPLO = 10; AX = 1; GLDO =  5; XPO =  2; }
void ENEMY_KIKIMORA()	 { STRCPY(sENEMY,"KIKIMORA"); 		HPLO = 25; AX = 2; GLDO = 10; XPO =  4; }
void ENEMY_LESHY()		 { STRCPY(sENEMY,"LESHY");			HPLO = 25; AX = 2; GLDO = 10; XPO =  4; }
void ENEMY_BABA_YAGA()	 { STRCPY(sENEMY,"BABA YAGA");		HPLO = 35; AX = 3; GLDO = 15; XPO = 10; }


void game_forest_enemy_victory()
{
	NEXT_FRAME;
	CLS;
	CURSOR(14,1);PRINT("VICTORY!");
	CURSOR(9,10);PRINT("YOU GOT GOLD: ");PRINT_INT(GLDO);
	CURSOR(10,11);INPUT_WAIT_NUM;
	
	gameStatus = STAT_MAIN_SCREEN;
}



void game_forest_enemy_run()
{
	int O = RND8(10);
	if (O < 5)
	{
		gameStatus = STAT_MAIN_SCREEN;
	}
	else
	{
		CURRHP--;
		
		NEXT_FRAME;
		CLS;
		CURSOR(0,11);PRINT("YOU COULDNT RUN AWAY");
		game_forest_enemy(0);

	}
}

void game_forest_enemy_fight()
{
	int DAMAGE_YOU = 0;
	int DAMAGE_ENEMY = 0;

	int F = RND8(10);
	int ATKLO = 0;

	if (F == 0) DAMAGE_ENEMY = 0;
	else if (F >= 1 && F < 4) { DAMAGE_ENEMY = 1 + ATK;		HPLO -= DAMAGE_ENEMY; }
	else if (F >= 4 && F < 7) { DAMAGE_ENEMY = 2 + ATK;		HPLO -= DAMAGE_ENEMY; }
	else if (F >= 7 && F < 9) { DAMAGE_ENEMY = 3 + ATK;		HPLO -= DAMAGE_ENEMY; }
	else if (F == 9) 		  { DAMAGE_ENEMY = 4 + ATK;		HPLO -= DAMAGE_ENEMY; }
	
	if (HPLO <= 0) { CURRXP += XPO; GOLD += GLDO; gameStatus = STAT_FOREST_ENEMY_VICTORY; return; }
	
	F = RND8(10);
	if (F == 0) DAMAGE_YOU = 0;
	else if (F >= 1 && F < 4) { DAMAGE_YOU = 1 + ATKLO; 	CURRHP = CURRHP - DAMAGE_YOU + ARM; }
	else if (F >= 4 && F < 7) { DAMAGE_YOU = 2 + ATKLO; 	CURRHP = CURRHP - DAMAGE_YOU + ARM; }
	else if (F >= 7 && F < 9) { DAMAGE_YOU = 3 + AX; 		CURRHP = CURRHP - DAMAGE_YOU + ARM; }
	else if (F == 9) 		  { DAMAGE_YOU = 4 + AX;		CURRHP = CURRHP - DAMAGE_YOU + ARM; }

	CURSOR(0,11);PRINT("DAMAGE TO YOUR OPPONENT: ");PRINT_INT(DAMAGE_ENEMY);

	DAMAGE_YOU -= ARM;
	if (DAMAGE_YOU < 0) DAMAGE_YOU = 0;
	
	CURSOR(0,12);PRINT("DAMAGE TO YOU: ");PRINT_INT(DAMAGE_YOU);
	BEEP_LOW;
	
	CURSOR(0,13);INPUT_WAIT_NUM;
	
	gameStatus = STAT_FOREST_ENEMY;
}




void game_forest_enemy(u8 clear_screen)
{
	if (CURRHP < 1) { gameStatus = STAT_DEAD_SCREEN; return; }
	
	if (clear_screen)
	{
		NEXT_FRAME;
		CLS;
	}
	
	CURSOR(0,0);PRINT("YOU ARE IN THE FOREST");
	CURSOR(0,1);PRINT("YOU ENCOUNTERED: ");PRINT(sENEMY);
	CURSOR(0,5);PRINT("WHAT WILL YOU DO?");
	CURSOR(0,6);PRINT("1) RUN");
	CURSOR(0,7);PRINT("2) FIGHT");
	CURSOR(0,22);PRINT("YOUR HP - ");PRINT_INT(CURRHP);
	CURSOR(17,22);PRINT("ENEMY HP - ");PRINT_INT(HPLO);
	CURSOR(6,19);PRINT("\xBC");
	CURSOR(5,20);PRINT("\x8E\x92)");
	CURSOR(6,21);PRINT("^");
	CURSOR(24,19);PRINT("@");
	CURSOR(23,20);PRINT("-\x8E-");
	CURSOR(24,21);PRINT("^");
	
	BEEP;
	
	int code = INPUT_WAIT_NUM;
	
	switch(code)
	{
		case 1:
			gameStatus = STAT_FOREST_ENEMY_RUN;
			//game_forest_enemy_run();
			break;

		case 2:
			gameStatus = STAT_FOREST_ENEMY_FIGHT;
			//game_forest_enemy_fight();
			break;
	}
}


void game_forest_patrol()
{
	if (CURRHP < 1) { gameStatus = STAT_DEAD_SCREEN; return; }
	
	NEXT_FRAME;
	CLS;
	
	switch (X)
	{
		case 1: //IF X=1 THEN GOTO L2200
			{
				int E = RND8(10);
				if (E < 5)
				{
					// GOTO BRIGAND
					ENEMY_BRIGAND();
				}
				else
				{
					// GOTO HUNGRY_WOLF
					ENEMY_HUNGRY_WOLF();
				}

				gameStatus = STAT_FOREST_ENEMY;
			}
			break;

		case 2: //IF X=2 THEN GOTO L2400
			{
				int E = (int)RND8(10);
				switch(E)
				{
					case 3:		//GOTO KIKIMORA
					case 4:
					case 5:
						ENEMY_KIKIMORA();
						break;

					case 6:		//GOTO HUNGRY_WOLF
					case 7:
						ENEMY_HUNGRY_WOLF();
						break;

					case 8:		//GOTO LESHY
					case 9:
						ENEMY_LESHY();
						break;

					// case 0:		// GOTO BRIGAND
					// case 1:
					// case 2:
					default:
						ENEMY_BRIGAND();
						break;
				}
				gameStatus = STAT_FOREST_ENEMY;
			}
			break;


		case 3:	//IF X=3 THEN GOTO L2600
			{
				int E = (int)RND8(10);
				switch(E)
				{

					case 1:		// GOTO HUNGRY_WOLF
						ENEMY_HUNGRY_WOLF();
						break;
						
					case 2:		// GOTO KIKIMORA
					case 3:		
					case 4:
						ENEMY_KIKIMORA();
						break;
						
					case 5:		// GOTO LESHY
					case 6:		
					case 7:
						ENEMY_LESHY();
						break;

					case 8:		// GOTO BABA_YAGA
					case 9:
						ENEMY_BABA_YAGA();
						break;

					//case 0:		// GOTO BRIGAND
					default:
						ENEMY_BRIGAND();
						break;
				}

				gameStatus = STAT_FOREST_ENEMY;
			}

			break;
	}
		
	
	
	
}