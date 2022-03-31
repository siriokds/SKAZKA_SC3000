sc_ppi_a		= 0xDC
sc_ppi_b		= 0xDD
sc_ppi_c		= 0xDE


;      Columns
;      PPI Port A                        PPI Port B
; Rows D0  D1  D2  D3  D4  D5  D6  D7    D0  D1  D2  D3
;  -   -------------------------------   ---------------
;  0   '1' 'Q' 'A' 'Z' ED  ',' 'K' 'I'   '8' --- --- ---
;  1   '2' 'W' 'S' 'X' SPC '.' 'L' 'O'   '9' --- --- ---
;  2   '3' 'E' 'D' 'C' HC  '/' ';' 'P'   '0' --- --- ---
;  3   '4' 'R' 'F' 'V' ID  PI  ':' '@'   '-' --- --- ---
;  4   '5' 'T' 'G' 'B' --- DA  ']' '['   '^' --- --- ---
;  5   '6' 'Y' 'H' 'N' --- LA  CR  ---   YEN --- --- FNC
;  6   '7' 'U' 'J' 'M' --- RA  UA  ---   BRK GRP CTL SHF
;  7   1U  1D  1L  1R  1TL 1TR 2U  2D    2L  2R  2TL 2TR


;      Columns
;      PPI Port B                      PPI Port A
; Rows D3  D2  D1  D0    D7  D6  D5  D4  D3  D2  D1  D0
;  -   ----------------  -------------------------------
;  0   --- --- --- '8'   'I' 'K' ',' ENG 'Z' 'A' 'Q' '1'
;  1   --- --- --- '9'   'O' 'L' '.' SPC 'X' 'S' 'W' '2'
;  2   --- --- --- '0'   'P' ';' '/' HOM 'C' 'D' 'E' '3'
;  3   --- --- --- '-'   '@' ':' PI  INS 'V' 'F' 'R' '4'
;  4   --- --- --- '^'   '[' ']' DN  --- 'B' 'G' 'T' '5'
;  5   FNC --- --- YEN   --- CR  LF  --- 'N' 'H' 'Y' '6'
;  6   SHF CTL GRP BRK   --- UP  RT  --- 'M' 'J' 'D' '7'
;  7   2TR 2TL 2R  2L    2DN 2UP 1TR 1TL 1R  1L  1DN 1UP


; ED  = "ENG DIER'S", SPC = (Spacebar), HC  = "HOME CLR", ID  = "INS DEL", PI  = (PI symbol)
; DA  = (Down arrow on keypad), LA  = (Left arrow on keypad), RA  = (Right arrow on keypad), CR  = "CR" (Enter), UA  = (Up arrow on keypad)
; YEN = (Yen symbol), BRK = "BREAK", GRP = "GRAPH", CTL = "CTRL", FNC = "FUNC", SHF = "SHIFT"
; 1U  = Joystick #1 up, 1D  = Joystick #1 down, 1L  = Joystick #1 left, 1R  = Joystick #1 right, 1TL = Joystick #1 left trigger, 1TR = Joystick #1 right trigger
; 2U  = Joystick #2 up, 2D  = Joystick #2 down, 2L  = Joystick #2 left, 2R  = Joystick #2 right, 2TL = Joystick #2 left trigger, 2TR = Joystick #2 right trigger

; Typematic Rate Setting	Enabled
; Typematic Rate (Chars/Sec)	30						
; Typematic Delay (Msec) 		250



; Every keyboard row is 12 bit long and are splitted in 2 ports.
; In order to make the access more friendly, this function transform a 12x7 data matrix (joyst not used) into a 8x9 one. 
; LAST ROW (8) CONTAINS ONLY MODIFIER KEYS.


; address                    bits
;                            7      6      5      4      3      2      1      0
; keyboard_input_buffer+0    I      K      ,  (eng dier) Z      A      Q      1
; keyboard_input_buffer+1    O      L      .    (space)  X      S      W      2
; keyboard_input_buffer+2    P      ;      /  (home/clr) C      D      E      3
; keyboard_input_buffer+3    @      :     (pi) (ins/del) V      F      R      4

; keyboard_input_buffer+4    [      ]    (down)          B      G      T      5
; keyboard_input_buffer+5          (CR)  (left)          N      H      Y      6
; keyboard_input_buffer+6          (up)  (rite)          M      J      U      7
; keyboard_input_buffer+7    8      9      0      -      ^     (yen) (graph)(break)

; keyboard_input_buffer+8                              (shift)(ctrl) (func)
; blanks should be ignored.


;#######################################
; Read key buffer
;#######################################
; fills 9 bytes from keyboard_input_buffer
; with key state information

KEYB_READ_MATRIX::
	in     a,(sc_ppi_c)                     ; set keyboard raster to 0
	and    #0b11111000
	ld     c,a
	
	ld     b, #6                            ; counter: first 6 rasters only
	ld     hl, #keyboard_input_buffer

KEYB_READ_MATRIX_loop:    
	call   KEYB_READ_RAWDATA
	ld     (hl),a                           ; save low 8 bits in RAM
	rrc    e                                ; CF = E & 1; BIT0 => CF;    save PB0 bits in d - PB1/2/3 are nothing for 1st 5 rasters
	rl     d                                ; D = (D << 1) | CF;         high bits correspond to low rasters: ??890-^(yen)
	inc    c                                ; move to next keyboard raster
	inc    hl
	djnz   KEYB_READ_MATRIX_loop            ; repeat for all rasters
	
																								 ; D = 000 & "^-098"
	ld     b,e                              ; B = 00000 FUNC 00         bit 2 is FUNC, others are nothing
	
	call   KEYB_READ_RAWDATA           		; get last raster
	ld     (hl),a                           ; save in RAM
	sla    d                                ; move d (high bits) up by 2
	sla    d
																								 ; D = 0 & "890-^" & 00
	ld     a,e                              ; and put GRAPH and BREAK in the gap
	and    #0b00000011                      ; so its 890-^(yen)(graph)(break)
	add    a,d								; A = 0 & "890-^" & 00
	
	inc    hl                               ; stick the lot in the next slot in RAM
	ld     (hl),a
	ld     a,e
	
	and    #0b00001100                      ; CTRL and SHIFT are still unsaved
	ld     e,a                              ; put them in e
	ld     a,b                              ; get the FUNC key (?????(func)??)
	rrca
	and    #0x02                            ; convert to 000000(func)0
	or     e                                ; merge with 0000(shift)(ctrl)00

	ld     (keyboard_input_buffer+8),a      ; save in the last byte of the buffer
	ret



;#######################################
; Read from keyboard
;#######################################
; Parameters:
; c = SC3000 PPI C value
; Returns:
; ea = scan code (high bits are other SC3000 statuses)
KEYB_READ_RAWDATA:
	ld     a,c                                   ; set keyboard raster with c
	out    (sc_ppi_c),a
	call   pause_for_ppi                         ; wait before reading back
	in     a,(sc_ppi_b)
	cpl                                          ; read high bits into e, invert so they are active high
	and	   #0x0F
	ld     e,a
	in     a,(sc_ppi_a)                          ; similar for low bits
	cpl
	ret

pause_for_ppi:
	nop
	nop
	ret



