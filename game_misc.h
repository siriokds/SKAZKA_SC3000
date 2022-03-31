void game_menu()
{
	NEXT_FRAME;
	CLS;

	NEXT_FRAME;
	CURSOR(17,8);PRINT("MENU");
	CURSOR(14,10);PRINT("1=EXIT GAME");
	CURSOR(14,11);PRINT("2=RESTART");
	CURSOR(14,12);PRINT("3=CONTINUE");
	BEEP;
	//CURSOR(14,13);INPUTR
	
	
	int code = INPUT_WAIT_NUM;
	switch(code)
	{
		case 1:
			gameFirstTime = 1;
			gameStatus = STAT_TITLE;
			break;

		case 2:
			gameStatus = STAT_TITLE;
			break;

		case 3:
			gameStatus = STAT_MAIN_SCREEN;
			break;
	}
}




void game_dead_screen()
{
	if (UP > 0) { STRCPY(sMISC, ""); UP = 0; CURRHP = M; gameStatus = STAT_TITLE; }
	
	NEXT_FRAME;
	CLS;
	CURSOR(7,8);PRINT("YOU ARE DEAD! GAME OVER.");
	CURSOR(1,20);PRINT("PRESS ANY KEY TO RESTARTING THE GAME");
	CURSOR(17,21);

	INPUT_CHAR;
	gameStatus = STAT_TITLE;
}


void game_endgame_screen()
{
	NEXT_FRAME;
	CLS;

	NEXT_FRAME;
	CURSOR(10,8);PRINT("CONGRATULATIONS!!!");
	CURSOR(3,10);PRINT("YOU HAVE COMPLETED A GREAT GAME");
	CURSOR(0,11);PRINT("AND PROVED THE JUSTICE OF OUR CULTURE.");
	CURSOR(6,12);PRINT("NOW GO AND REST OUR HERO!");
	CURSOR(4,21);PRINT("PRESS ANY KEY TO END THE GAME");
	
	CURSOR(18,22); INPUT_CHAR;
	gameStatus = STAT_TITLE;
}


