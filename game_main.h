
void game_main_screen(int Q)
{
	NEXT_FRAME;
	CLS;

	NEXT_FRAME;
	if (Q==3)
	{ 
		CURSOR(13,19);
		PRINT("HP IS REPLENISHED");
	}
	else
	{
		//P = 0;
	}
	
	CURSOR(0,0);
	PRINT("\x86\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x88\n");
	PRINT("\x80STATISTICS\x80\n");
	PRINT("\x80HP    /   \x80\n");
	PRINT("\x80XP    /   \x80\n");
	PRINT("\x80GOLD      \x80\n");
	PRINT("\x80 YOU HAVE \x80\n");
	PRINT("\x80          \x80\n");
	PRINT("\x80          \x80\n");
	PRINT("\x80          \x80\n");
	PRINT("\x80          \x80\n");
	PRINT("\x87\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x89");
	CURSOR(0,12);PRINT("     \x8A\x81\x8C");
	CURSOR(0,13);PRINT("     \x80 \x80");
	CURSOR(0,14);PRINT("   \x8E \x8B\x83\x8D");
	CURSOR(0,15);PRINT("   \x80 \x86\x82\x88\x8A\x81\x8C");
	CURSOR(0,16);PRINT("   \xF8\x81\x84 \x85\x84 \x80");
	CURSOR(0,17);PRINT("     \x87\x83\x89\x8B\x81\x8D");
	CURSOR(0,18);PRINT("     \x93 \x94");
	CURSOR(2,21);PRINT("YOUR LVL");
	CURSOR(0,19);PRINT("    \x93   \x94");
	
	
	     if (CURRXP <= 11) { X=1; M=10; Y=11; }
	else if (CURRXP <= 34) { X=2; M=20; Y=34; }
	else if (CURRXP <= 60) { X=3; M=30; Y=60; }
	else 				   { X=3; M=30; Y=99; CURSOR(3,22);PRINT("(HERO)"); }

	CURSOR(10,21);PRINT_INT(X);	//REM CUR LVL


	NEXT_FRAME;
	CURSOR(16,1);PRINT("OLD VILLAGE");
	CURSOR(13,3);PRINT("      ");
	CURSOR(13,4);PRINT("  \xE1 \xE4\xE9\xE9   \xE0   \xE8");
	CURSOR(13,5);PRINT("\x95\xE5\xE5\xE5\xE5\xE5\x96  \x95\xE5\x96 \xE8\x90\xE8");
	CURSOR(13,6);PRINT("\xE5\xE5\xE5\xE5\xE5\xE5\xE5  \xE5\xE5\xE5 \x90\x90\x90");
	CURSOR(13,7);PRINT("\xE5\x92\xE5\x92\xE5\x92\xE5  \xE5\x92\xE5 \x90\x90\x90");
	CURSOR(13,8);PRINT("\xE5\xE5\xE5\xE5\xE5\xE5\xE5\xFD \xE5\xE5\xE5\xFD \xE6 ");
	CURSOR(13,10);PRINT("WHAT SHALL WE DO?");
	CURSOR(13,12);PRINT("1) FOREST PATROL");
	CURSOR(13,13);PRINT("2) VISIT THE SHOP");
	CURSOR(13,14);PRINT("3) REST");
	CURSOR(13,15);PRINT("4) TALK TO VILLAGERS");
	CURSOR(13,16);PRINT("5) FIGHT THE KOSCHEY");
	CURSOR(13,18);PRINT("6) MENU");
	
	CURSOR(8,2);PRINT_INT(M);
	CURSOR(8,3);PRINT_INT(Y);
	if (STARTHP == 1) { CURRHP=10; STARTHP=0; }
	CURSOR(3,3);PRINT_INT(CURRXP);
	CURSOR(3,2);PRINT_INT(CURRHP);
	CURSOR(6,4);PRINT_INT(GOLD);
	CURSOR(1,6);PRINT(sWEAPON);
	CURSOR(1,7);PRINT(sARMOR);
	CURSOR(1,8);PRINT(sMISC);
	CURSOR(13,17);
	
	BEEP;
	
	int code = INPUT_WAIT_NUM;
	switch(code)
	{
		case 1:
			gameStatus = STAT_FOREST_PATROL;
			break;
		case 2:
			gameStatus = STAT_SHOP_SCREEN;
			break;
		case 3:
			//REST_HERO:
			CURRHP = M;
			CURSOR(3,2);PRINT_INT(CURRHP);
			gameStatus = STAT_MAIN_SCREEN;
			break;
		case 4:
			gameStatus = STAT_TALK_VILLAGERS;
			break;
		case 5:
			gameStatus = STAT_KOSHEY;
			break;

		case 6:
			gameStatus = STAT_GAMEMENU;
			break;
	}
	
}
