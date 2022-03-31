//#include "game_music_notes.h"

#define PSG_FREQ(f)	(PSG_MAIN_CLOCK/(f*32))


    // make_octave 1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023 ; Oct -3  0
    // make_octave 1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023,1023 ; Oct -2  1
    // make_octave 1023,1023,1023,1023,1023,1023,1023,1023,1023,1008,951,898   ; Oct -1  2
    // make_octave 847,800,755,713,673,635,599,566,534,504,476,449             ; Oct 0   3
    // make_octave 424,400,377,356,336,317,300,283,267,252,238,224             ; Oct 1	 4
    // make_octave 212,200,189,178,168,159,150,141,133,126,119,112             ; Oct 2   5
    // make_octave 106,100,94,89,84,79,75,71,67,63,59,56                       ; Oct 3   6
    // make_octave 53,50,47,45,42,40,37,35,33,31,30,28                         ; Oct 4   7
    // make_octave 26,25,24,22,21,20,19,18,17,16,15,14                         ; Oct 5   8
    // make_octave 13,12,12,11,11,10,9,9,8,8,7,7                               ; Oct 6   9
    // make_octave 7,6,6,6,5,5,5,4,4,4,4,4                                     ; Oct 7  10
    // make_octave 3,3,3,3,3,2,2,2,2,2,2,2                                     ; Oct 8  11

// 362, 322, 304, 270, 304, 322, 362, 383, 362, 241, 270, 304, 320, 362, 362

const int TITLE_MUSIC[] = {
	
	PSG_FREQ(309), 30,
	PSG_FREQ(347), 15,
	PSG_FREQ(367), 24,
	
	PSG_FREQ(413), 6, 
	PSG_FREQ(367), 15,
	PSG_FREQ(347), 15,
	PSG_FREQ(309), 15,
	PSG_FREQ(292), 15,
	PSG_FREQ(309), 45, 
	
	PSG_FREQ(463), 30,
	PSG_FREQ(413), 15,
	PSG_FREQ(367), 24,
	PSG_FREQ(349), 6,
	PSG_FREQ(309), 15,		
	PSG_FREQ(309), 60,
	0,0
};

const int BOSS_MUSIC[] = {
	PSG_FREQ(110), 30, 
	PSG_FREQ(123), 10, 
	PSG_FREQ(130), 20, 
	PSG_FREQ(110), 20, 
	PSG_FREQ(208), 4, 
	PSG_FREQ(220), 4, 
	PSG_FREQ(208), 4, 
	PSG_FREQ(220), 4, 
	PSG_FREQ(208), 4, 
	PSG_FREQ(220), 4, 
	PSG_FREQ(208), 4, 
	PSG_FREQ(220), 4, 
	PSG_FREQ(208), 4, 
	PSG_FREQ(220), 4, 
	PSG_FREQ(208), 4, 
	//PSG_FREQ(220), 4, 
	//PSG_FREQ(208), 4, 
	PSG_FREQ(220), 45, 
	0,0
};


int playDelay(int wait)
{
	int stopped = 0;
	
	u16 sta = sys_frames16();
	
	//wait /= 4;
	
	do 
	{ 
		NEXT_FRAME;
		//if (kbd_test()) return 1;
	
	} while ((sys_frames16() - sta) < wait);

	return stopped;
}


#define PSG_STEREO_VOL	9

void playMusic(int* music, int stereo)
{
	int offset = 0;
	int freq  = 0;
	int delay = 0;

	int oldFreq = 0;
	
	
	//PRINT_INT((u16)music);
//	psg_set_freqdiv(0,0);
//	psg_set_freqdiv(1,0);
//	psg_set_volume(0,0);
//	psg_set_volume(1,0);
	
	psg_mute();

	psg_set_volume(0,14);
	psg_set_volume(1, 0);
	
	while (1)
	{
		freq = music[offset];
		
		if (freq == 0) break;
		
		delay = music[offset+1];

		//PRINT("\n"); //PRINT_INT(freq); 
		//console_write_hex8(freq >> 8);
		//console_write_hex8(freq & 255);


		if (stereo)
		{
			psg_set_freqdiv(0,freq);

			if (oldFreq > 0)
			{
				psg_set_volume(1, PSG_STEREO_VOL);
				psg_set_freqdiv(1,oldFreq);
			}
			else
			{
				psg_set_volume(1, 0);
				psg_set_freqdiv(1,0x3FF);
			}


		}
		else
		{
			psg_set_freqdiv(0,freq);
			psg_set_volume(0,14);
		}


		
		
		oldFreq = freq;


		if (playDelay(delay)) break;

		
		offset += 2;
	}

	psg_mute();
}




void playTitleMusic()
{
	playMusic(TITLE_MUSIC, 1);
	//playMusic(BOSS_MUSIC, 0);
}


void playBossMusic()
{
	playMusic(BOSS_MUSIC, 0);
}
