void	game_talk_villagers()
{
	int H = RND8(10);
	
	NEXT_FRAME;
	CLS;
	
	NEXT_FRAME;
	switch (H)
	{
		case 0:
			GOLD -= 5;
			if (GOLD <= 0) GOLD = 0;
			PRINT("YOU LOST SOME MONEY... ");
			break;
		
		case 1:
			GOLD += 5;
			PRINT("YOU FOUND SOME MONEY!");
			break;

		case 2:
			PRINT("DID YOU KNOW THAT YOU CAN ONLY HAVE   ONE LIFE POTION AT A TIME?");
			break;

		case 3:
			PRINT("DID YOU KNOW THAT A TEGILAY IS MADE OFLEATHER, AND A KOLCHUGA IS MADE OF    STEEL?");
			break;

		case 4:
			PRINT("TRULY THE AXE IS THE GREATEST WEAPON  OF OUR PEOPLE!");
			break;

		case 5:
			PRINT("SOMETIMES ITS WISE TO RUN AWAY");
			break;

		case 6:
			PRINT("YOU BETTER BE KNOWN AS A HERO TO      CHALLANGE KOSCHEY");
			break;

		case 7:
			PRINT("DID YOU KNOW THAT WOLFS ONLY ATTACK   WHEN THEY ARE HUNGRY?");
			break;

		case 8:
			PRINT("SOME PEOPLE SAY THEY SAW BABA YAGA IN THE FOREST, BUT I DONT BELIVE THEM");
			break;

		case 9:
			PRINT("CYKA, BLYAT!");
			break;
	}
	
	BEEP_LOW;
	PRINT("\n\nPRESS ANY KEY");
	INPUT_CHAR;
	
	gameStatus = STAT_MAIN_SCREEN;
}