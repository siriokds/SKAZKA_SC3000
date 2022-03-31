#include "main.h"
#include "game.h"


void setup()
{
	vdp_init_textmode();
	game_setup();
}


void main() 
{
	setup();

	while (1) { game_loop(); }
}

