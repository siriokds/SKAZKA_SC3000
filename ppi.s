;
; SC - PPI (8255)
;
; Port $DC: 			PA key data
; Bit7: KEY PA7 (input)
; Bit6: KEY PA6 (input)
; Bit5: KEY PA5 (input)
; Bit4: KEY PA4 (input)
; Bit3: KEY PA3 (input)
; Bit2: KEY PA2 (input)
; Bit1: KEY PA1 (input)
; Bit0: KEY PA0 (input)
;
; Port $DD: 			PB key data, CMT, serial printer, cartridge terminal
;  Bit7: CMT LOAD (input)
;  Bit6: SPR BUSY (input)
;  Bit5: SPR / FAULT (input)
;  Bit4: EXT / CONT (input)
;  Bit3: KEY PB3 (input)
;  Bit2: KEY PB2 (input)
;  Bit1: KEY PB1 (input)
;  Bit0: KEY PB0 (input)
;
; Port $DE: 			PC key select, CMT, serial printer
;  Bit7: SPR / FEED (output)
;  Bit6: SPR / RESET (output)
;  Bit5: SPR DATA (output)
;  Bit 4: CMT SAVE (output)
;  Bit3: CMT MOTOR (output) 1 = On, 0 = Off (not implemented)
;  Bit2: KEY SEL P2 (output)
;  Bit1: KEY SEL P1 (output)
;  Bit0: KEY SEL P0 (output)
;
; Port $DF: 			8255 CW



.module ppi

sc_ppi_a                       = 0xdc
sc_ppi_b                       = 0xdd
sc_ppi_c                       = 0xde
sc_ppi_control                 = 0xdf



	.area _CODE
.globl	ppi_read_key_buffer, keyboard_input_buffer

_ppi_reset::
		push	 af
		
		ld     a, #0b10010010                        ; initialise SC PPI: set I/O to mode 0, A+B in, C out
		out    (sc_ppi_control),a

		pop			af
		ret




;#######################################
; Read key buffer
;#######################################
; fills 9 bytes from keyboard_input_buffer
; with key state information
; address                    bits
;                            7      6      5      4      3      2      1      0
; keyboard_input_buffer+0    I      K      ,  (eng dier) Z      A      Q      1
; keyboard_input_buffer+1    O      L      .    (space)  X      S      W      2
; keyboard_input_buffer+2    P      ;      /  (home/clr) C      D      E      3
; keyboard_input_buffer+3    @      :     (pi) (ins/del) V      F      R      4
; keyboard_input_buffer+4    [      ]    (down)          B      G      T      5
; keyboard_input_buffer+5          (cr)  (left)          N      H      Y      6
; keyboard_input_buffer+6          (up)  (right)         M      J      U      7
; keyboard_input_buffer+7    8      9      0      -      ^     (yen) (graph)(break)
; keyboard_input_buffer+8                              (shift)(ctrl) (func)
; blanks should be ignored.

ppi_read_key_buffer::
;--------------------------------------------------
    push   af
    push   bc
    push   de
    push   hl
      in     a,(sc_ppi_c)                        ; set keyboard raster to 0
      and    #0xF8
      ld     c,a

      ld     b,#6                                 ; counter: first 6 rasters only
      ld     hl, #keyboard_input_buffer
rkb_1a:    
		  call   raw_read_from_keyboard
      ld     (hl),a                              ; save low 8 bits in RAM
      rrc    e                                   ; save PB0 bits in d - PB1/2/3 are nothing for 1st 5 rasters
      rl     d                                   ; high bits correspond to low rasters: ??890-^(yen)
      inc    c                                   ; move to next keyboard raster
      inc    hl
      djnz  rkb_1a                                  ; repeat for all rasters

      ld     b,e                                 ; bit 2 is FUNC, others are nothing
      call   raw_read_from_keyboard              ; get last raster
      ld     (hl),a                              ; save in RAM
      sla    d                                   ; move d (high bits) up by 2
      sla    d
      ld     a,e                                 ; and put GRAPH and BREAK in the gap
      and    #0x03                           		 ; so it's 890-^(yen)(graph)(break)
      add    a,d
      inc    hl                                  ; stick the lot in the next slot in RAM
      ld     (hl),a
      ld     a,e
      and    #0x0C															 ; CTRL and SHIFT are still unsaved
      ld     e,a                                 ; put them in e
      ld     a,b                                 ; get the FUNC key (?????(func)??)
      rrca
      and    #0x02                                 ; convert to 000000(func)0
      or     e                                   ; merge with 0000(shift)(ctrl)00
      ld     (keyboard_input_buffer+8),a         ; save in the last byte of the buffer
		pop    hl
		pop    de
		pop    bc
		pop    af
		ret
		
		
;#######################################
; Read from keyboard
;#######################################
; Parameters:
; c = SC3000 PPI C value
; Returns:
; ea = scan code (high bits are other SC3000 statuses)
raw_read_from_keyboard:
    ld     a,c                                   ; set keyboard raster with c
    out    (sc_ppi_c),a
    call   pause_for_ppi                         ; wait before reading back
    in     a,(sc_ppi_b)
    cpl                                          ; read high bits into e, invert so they are active high
    ld     e,a
    in     a,(sc_ppi_a)                          ; similar for low bits
    cpl
    ret

pause_for_ppi:
    nop
    nop
    ret
