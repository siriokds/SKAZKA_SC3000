void game_shop()
{
	NEXT_FRAME;
	CLS;

	CURSOR(0,0);PRINT("\x86\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x88");
	CURSOR(0,1);PRINT("\x80      SHOP      \x80");
	CURSOR(0,2);PRINT("\x80                \x80");
	CURSOR(0,3);PRINT("\x80 SWORD    (1)10 \x80");
	CURSOR(0,4);PRINT("\x80 AXE      (2)20 \x80");
	CURSOR(0,5);PRINT("\x80 KOLCHUGA (3)20 \x80");
	CURSOR(0,6);PRINT("\x80 TEGILAY  (4)10 \x80");
	CURSOR(0,7);PRINT("\x80 LIFE     (5)30 \x80");
	CURSOR(0,8);PRINT("\x87\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x89");
    CURSOR(0,10);PRINT("WHAT WILL YOU BUY?");
	CURSOR(0,20);PRINT("YOUR GOLD ");PRINT_INT(GOLD);
	CURSOR(0,21);PRINT("6 FOR EXIT SHOP");
	BEEP;
	gameStatus = STAT_SHOP_INPUT;
}

void game_shop_input()
{
	CURSOR(0,11);
	int code = INPUT_WAIT_NUM;
	
	switch(code)
	{
		case 1:	// BUY_SWORD
			if (GOLD >= 10) { GOLD -= 10; STRCPY(sWEAPON, "SWORD"); ATK = 1; gameStatus = STAT_MAIN_SCREEN; }
			else { CURSOR(0,12);PRINT("NOT ENOUGH GOLD"); }
			break;
		case 2:	// BUY_AXE
			if (GOLD >= 20) { GOLD -= 20; STRCPY(sWEAPON, "AXE"); ATK = 2; gameStatus = STAT_MAIN_SCREEN; }
			else { CURSOR(0,12);PRINT("NOT ENOUGH GOLD"); }
			break;
			
		case 3:	// BUY_KOLCHUGA
			if (GOLD >= 20) { GOLD -= 20; STRCPY(sARMOR, "KOLCHUGA"); ARM = 2; gameStatus = STAT_MAIN_SCREEN; }
			else { CURSOR(0,12);PRINT("NOT ENOUGH GOLD"); }
			break;
		case 4:	// BUY_TEGILAY
			if (GOLD >= 10) { GOLD -= 10; STRCPY(sARMOR, "TEGILAY"); ARM = 1; gameStatus = STAT_MAIN_SCREEN; }
			else { CURSOR(0,12);PRINT("NOT ENOUGH GOLD"); }
			break;
			
		case 5:	// BUY_1UP
			if (GOLD >= 30) { GOLD -= 30; STRCPY(sMISC, "+1 LIFE"); UP = 1; gameStatus = STAT_MAIN_SCREEN; }
			else { CURSOR(0,12);PRINT("NOT ENOUGH GOLD"); }
			break;
		default:
			gameStatus = STAT_MAIN_SCREEN;
			break;
	}
}

