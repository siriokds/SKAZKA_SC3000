void game_title()
{
	NEXT_FRAME;
	CLS;
	
	NEXT_FRAME;
	CURSOR(0,1);PRINT("\x8A\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x8C");

	for (int i=2; i <= 19; i++)
	{
		CURSOR(0,i);PRINT("\x80");
		CURSOR(37,i);PRINT("\x80");
	}
	
	CURSOR(0,20);PRINT("\x8B\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x81\x8D");
	CURSOR(5,4);PRINT("\x90\x90\x90 \x90  \x90  \x90\x90  \x90\x90\x90 \x90  \x90  \x90\x90");
	CURSOR(5,5);PRINT("\x90   \x90 \x90  \x90  \x90   \x90 \x90 \x90  \x90  \x90");
	CURSOR(5,6);PRINT("\x90\x90\x90 \x90\x90   \x90\x90\x90\x90  \x90  \x90\x90   \x90\x90\x90\x90");
	CURSOR(5,7);PRINT("  \x90 \x90 \x90  \x90  \x90 \x90   \x90 \x90  \x90  \x90");
	CURSOR(5,8);PRINT("\x90\x90\x90 \x90  \x90 \x90  \x90 \x90\x90\x90 \x90  \x90 \x90  \x90");

	CURSOR(8,13);PRINT("SLAVIC RPG ADVENTURE");
	CURSOR(3,18);PRINT("ONLY FOR SEGA COMPUTERS SC-3000/H");
	
	if (gameFirstTime)
	{
		gameFirstTime = 0;
		framesDelay(30);

		//for (int x = 0; x < 2; x++) { for (int i = 0; i < 32000; i++) {} }

		playTitleMusic();
		kbd_clearbuffer();
	}
	
	CURSOR(6,21);PRINT("PRESS ANY KEY TO CONTINUE");
	INPUT_WAIT;
	gameStatus = STAT_INSTRUCTIONS;

}


void game_instructions()
{
	NEXT_FRAME;
	CLS;

	NEXT_FRAME;
	CURSOR(0,1);PRINT("YOU ARE A YOUNG HERO WHO WISHES TO RIDTHE LAND FROM A TERRIFYING CREATURE\nKNOWN AS KOSCHEY THE IMMORTAL.");
	CURSOR(0,8);PRINT("RIGHT NOW ALL YOU HAVE IS YOUR DAGGER\nAND YOUR WHITS.");
	CURSOR(0,15);PRINT("YOU ENTER A VILLAGE TO BEGIN YOUR\nADVENTURE.");
	BEEP;
	CURSOR(6,22);PRINT("PRESS ANY KEY TO CONTINUE");

	INPUT_WAIT;
	gameStatus = STAT_MAIN_SCREEN;
}

