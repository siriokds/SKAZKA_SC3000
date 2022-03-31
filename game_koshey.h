void game_koshey()
{
	NEXT_FRAME;
	CLS;
	
	NEXT_FRAME;
	PRINT("THIS IS THE MOST IMPORTANT BATTLE OF  YOUR LIFE.");
	CURSOR(0,3);PRINT("ARE YOU SURE YOU CAN HANDLE IT?");
	CURSOR(0,6);PRINT("1)YES");
	CURSOR(0,7);PRINT("2)NO");
	CURSOR(0,9);
	BEEP;
	int code = INPUT_WAIT_NUM;
	
	switch (code)
	{
		case 1:
			{
				if (CURRXP > 60) { ATK++; ARM++; }

				UP = 0;
				HPLO = 50;
				AX = 4;
				Q = 0;
				
				STRCPY(sSAY, "");
				
				gameStatus = STAT_KOSHEY_FINALBATTLE;
			}
			break;

		case 2:
			gameStatus = STAT_MAIN_SCREEN;
			break;
	}
	
}


void	game_koshey_finalbattle()
{
	if (HPLO   <= 0) { gameStatus = STAT_ENDGAME_SCREEN; return; }
	if (CURRHP <= 0) { gameStatus = STAT_DEAD_SCREEN; return; }
		
	NEXT_FRAME;
	CLS;
	
	NEXT_FRAME;
	CURSOR(0,1);PRINT("YOUR HP:");PRINT_INT(CURRHP);
	CURSOR(23,1);PRINT("KOSCHEYS HP:");PRINT_INT(HPLO);
	
	
	CURSOR(11,1);PRINT("     \x95\x96");
	CURSOR(11,2);PRINT(" \x95\x96  \xE5\xE5  \x95\x96");
	CURSOR(11,3);PRINT(" \xE5\xE5\x96\x95\xE5\xE5\x96\x95\xE5\xE5");
	CURSOR(11,4);PRINT("\x96\xE5\xE5\x96\xE5\x98\x97\xE5\x95\xE5\xE5\x95");
	CURSOR(11,5);PRINT("\xE5\xE5\xE5\xE5\xE5\x96\x95\xE5\xE5\xE5\xE5\xE5");
	CURSOR(11,6);PRINT("\x97\x98\x95 \x90  \x90 \x96\x97\x98");
	CURSOR(11,7);PRINT(" \x98\x97\x96 \x90\x90 \x95\x98\x97");
	CURSOR(11,8);PRINT("\x96\x9F\x97\xF7\x97\x98\x97\x98\xF7\x98\xE4\x95");
	CURSOR(11,9);PRINT("\xE5\x97  \x98  \x97  \x98\xE5");
	CURSOR(11,10);PRINT("\x97\x9F\x97\x90 \xE6\xE6 \x90\x98 \x98");
	CURSOR(11,11);PRINT(" \x97 \x90 \xE1\xE2 \x90 \x98");
	CURSOR(11,12);PRINT("  \x90      \x90");
	CURSOR(11,13);PRINT(" \x95\x90 \xE1\xE6\xE6\xE2 \x90\x96");
	CURSOR(24,13);PRINT(sSAY);
	CURSOR(11,14);PRINT("  \x97\x90    \x90\x98");
	CURSOR(11,15);PRINT("   \x97\x98\x90\x90\x97\x98");
	CURSOR(11,16);PRINT("    \x97\x98\x97\x98");
	CURSOR(1,18);PRINT("SO YOU HAVE COME TO CHALLENGE ME,     FOOLISH MORTAL? VERY WELL.");
	
	if (Q <= 0) 
	{ 
		playBossMusic();
		// BEEP_LOW;
		// BEEP_LOW;
		// BEEP_LOW;
		// BEEP_LOW;
	}

	CURSOR(1,20);PRINT("1) BEG FOR YOUR LIFE");
	CURSOR(1,21);PRINT("2) FIGHT");
	CURSOR(1,22);

	Q = INPUT_WAIT_NUM;
	
	switch(Q)
	{
		case 1:
			STRCPY(sSAY, "HA HA HA");
			CURRHP -= 2;
			break;
			
		case 2:
			{
				int UC = 0;
				int UU = 0;
				int DAMAGE_ENEMY, DAMAGE_HERO;
				
				BEEP_LOW;
				
				switch((int)RND8(10))
				{
					case 0:	UU = 0;	break;
					
					case 1: 
					case 2: 
					case 3: UU = 1;	break;
						
					case 4: 
					case 5: 
					case 6: UU = 2;	break;

					case 7: 
					case 8: UU = 3;	break;

					case 9: UU = 4 + ATK; break;
				}

				DAMAGE_ENEMY = UU + ATK;
				HPLO -= DAMAGE_ENEMY;
				
				switch((int)RND8(10))
				{
					case 0:	UC = 0;	break;
					
					case 1: 
					case 2: 
					case 3: UC = 1;	break;
						
					case 4: 
					case 5: 
					case 6: UC = 2;	break;

					case 7: 
					case 8: UC = 3;	break;

					case 9: UC = 4 + AX; break;
				}
				
				DAMAGE_HERO = UC - ARM + AX;
				CURRHP -= DAMAGE_HERO;
				
				
				if (DAMAGE_HERO > DAMAGE_ENEMY)
				{
					STRCPY(sSAY, "HA HA HA");
				}
				else
				{
					STRCPY(sSAY, "");
				}	
				
			}
			break;
			
		default:
			STRCPY(sSAY, "");
			break;
		
	}



}





