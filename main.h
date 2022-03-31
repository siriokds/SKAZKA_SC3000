#ifndef _MAIN_H__
#define _MAIN_H__

const char* s = "MAIN";

typedef unsigned char 	u8;
typedef unsigned short 	u16;

#define VAR_STRING_SIZE	64

#define ASCII_BS 0x08
#define ASCII_FF 0x0C		// Form FEED
#define ASCII_CR 0x0D
#define ASCII_LF 0x0A
#define ASCII_SP 0x20

#define DI  				__asm di __endasm
#define EI   				__asm ei __endasm
#define DISABLE_INTERRUPTS	DI
#define ENABLE_INTERRUPTS	EI
#define HALT   				__asm halt __endasm
#define NEXT_FRAME			HALT


extern void sys_wait(u16) __sdcccall(1);
extern u16	sys_rand16() __sdcccall(1);
extern u16  sys_frames16() __sdcccall(1);
extern u8   kbd_test() __sdcccall(1);
extern void kbd_clearbuffer() __sdcccall(1);
extern void ppi_reset() __sdcccall(1);
extern void psg_reset() __sdcccall(1);
extern void psg_mute() __sdcccall(1);
extern void psg_beep() __sdcccall(1);
extern void psg_beep_low() __sdcccall(1);
extern void vdp_setup_registers() __sdcccall(1);

extern void console_cursor(u8 x, u8 y) __sdcccall(1);
extern void console_writechar(char) __sdcccall(1);
extern void console_writetext(char*) __sdcccall(1);
extern void console_write_hex8(u8) __sdcccall(1);
extern void console_write_int16(u16) __sdcccall(1);
extern void console_write_int8(u8) __sdcccall(1);
//extern char console_readkey();
//extern char console_waitkey();
extern char kbd_wait_char() __sdcccall(1);

#define vdp_init_textmode			vdp_setup_registers
#define	delay_ms(ms)				sys_wait(((ms)*65092)/1000)
#define	console_cr()				console_writechar(0x0D)
#define	console_spc()				console_writechar(32)

#define	console_write(s)			console_writetext(s)
#define	console_print(s)			console_writetext(s)
#define	console_printat(x,y,s)		console_cursor(x,y);console_writetext(s)
#define console_input_char			kbd_wait_char


#define CLS							console_writechar(ASCII_FF)
#define CURSOR(x,y)					console_cursor(x+2,y)
#define PRINT(s)					console_writetext(s)
#define PRINT_INT(n)				console_write_int8(n);
#define PRINTAT(x,y,s)				console_printat(s)
#define INPUT_CHAR					console_input_char()
#define INPUT_WAIT					INPUT_CHAR
#define INPUT_WAIT_CHAR(c)			console_waitfor_char(c)
#define INPUT_WAIT_NUM				console_waitfor_number()
#define INPUT_WAIT_CR				INPUT_WAIT_CHAR(ASCII_CR)
#define BEEP						psg_beep()
#define BEEP_LOW					psg_beep_low()
#define SOUND(c,f,v)				psg_set_freq(c,f);psg_set_volume(c,v)


#define PSG_LATCH_CMD     	0b10000000
#define PSG_DATA_CMD      	0b00000000
#define PSG_TYPE_VOL		0b00010000

#define PSG_MAIN_CLOCK 		(3580000)
#define PSG_MAIN_CLOCK_DIV 	(PSG_MAIN_CLOCK/32)


#define PSG_CHANNEL0      0b00000000  // Tone 1
#define PSG_CHANNEL1      0b00100000  // Tone 2
#define PSG_CHANNEL2      0b01000000  // Tone 3
#define PSG_CHANNEL3      0b01100000  // White Noise

   // PSG:
   //                Input clock (Hz) (3580000)
   // Freq (Hz) = ----------------------------------
   //                2 x (register value) x (divider (16))
   
   
static volatile __sfr __at 0x7f psg_port;


	void psg_set_freqdiv(int channel, u16 freq_div)
	{
		psg_port = PSG_LATCH_CMD | (channel << 5) | (freq_div & 0b1111);
		
//		if (freq_div > 0b1111)
        psg_port = PSG_DATA_CMD                   | (freq_div >> 4);
	}

	void psg_set_volume(int channel, u8 volume)
	{
		psg_port = PSG_LATCH_CMD | (channel << 5) | PSG_TYPE_VOL | (15-(volume & 0b1111));
	}


	u16 psg_freq_to_reg(u16 freq)
	{
	   return freq > 0 ? (PSG_MAIN_CLOCK / (freq * 32)) : 0;
	}

	void psg_set_freq(int channel, u16 freq)
	{
		u16 freqDiv = psg_freq_to_reg(freq);
		if (freqDiv > 0x3FF) freqDiv = 0x3FF;
		
		psg_set_freqdiv(channel, freqDiv );
	}

	
	void framesDelay(int numFrames)
	{
		u16 sta = sys_frames16();
		
		do 
		{ 
			NEXT_FRAME;

		} while ((sys_frames16() - sta) < numFrames);
	}


//	u8 RND8(u8 base)
//	{
//		return (((u8)sys_rand16())%(base));
//	}
	
	u8 RND8(u8 base)
	{
		return (((u8)sys_frames16())%(base));
	}

	void STRCPY(char *d, char *s)
	{
		while (*s)
		{
			*d++ = *s++;
		}
		
		*d = 0;
	}

	int console_waitfor_number()
	{
		int v = 0;
		
		while(1)
		{
			char a = console_input_char();
			if (a >= 0x30 && a <= 0x39)
			{
				v = a - 0x30;
				break;
			}
		};
		
		return v;
		 
	}


	void console_waitfor_char(char c)
	{
		char a = 0;
		do
		{
			a = console_input_char();
		} while (a != c);
	}

#endif // #define _MAIN_H__