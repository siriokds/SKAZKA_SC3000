
SP  = 0x20
BS  = 0x08
LF  = 0x0A
FF  = 0x0C
CR  = 0x0D

 
;-------------------------------------------------------
KEYB_READ_ASCII::
;-------------------------------------------------------
	ld	bc, #0x0800
	ld 	de, #keyboard_input_buffer
r_key_loop:    
	ld		a,(de)
	or		a
	jr		nz, r_key_found
	inc		de
	inc		c
	djnz	r_key_loop
	
	xor	a                          	; found nothing -> return
	jr	r_key_end


r_key_found:
	ld      h, #0x00				; else: c = offset of data found
	ld      l, c
	add     hl,hl
	add     hl,hl
	add     hl,hl                  	; multiply by 8
	ld      b, a
	ld      c, #0x00

r_key_loop_2:    
	rrc	b                         	; how many trailing zero bits there are in b, result in c
	jr	c, r_key_add_res   	      	; eg. %01010000 = 4, 0b01010100 = 2
	inc	c
	jr	r_key_loop_2

r_key_add_res:    
	ld     	b, #0x00				; add result to hl
	add    	hl,bc

	ld	a,(keyboard_input_buffer+8)         	; check if shift/ctrl key was pressed

	ld	bc, #table_ctrl_chars
	bit	2,a
	jr	nz, r_key_add_retval

	bit	3,a
	ld	bc, #table_shifted_chars
	jr	z, r_key_add_retval
	ld	bc, #table_unshifted_chars

r_key_add_retval:    
	add    hl,bc                               		; do the lookup and return the byte found
    ld     a,(hl)


r_key_end:
	; A = CHAR, HL = OLD | NEW
	
	ld	hl, (keyboard_last_char)
	ld	h, l
	ld	l, a
	ld	(keyboard_last_char), hl
	sub	h
	ld	(keyboard_diff_char), a

	ret



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



; 0x0B, CTRL+K ???

table_ctrl_chars:
.db   0x00, 0x11, 0x01, 0x1A,   0x00, 0x00, 0x0B, 0x09
.db   0x00, 0x17, 0x13, 0x18,   0x00, 0x00, 0x0C, 0x0F
.db   0x00, 0x05, 0x04, 0x03,   0x00, 0x00, 0x00, 0x10
.db   0x00, 0x12, 0x06, 0x16,   0x00, 0x00, 0x00, 0x00

.db   0x00, 0x14, 0x07, 0x02,   0x00, 0x00, 0x1D, 0x1B
.db   0x00, 0x19, BS, 	0x0E,   0x00, 0x00, 0x00, 0x00
.db   0x00, 0x05, LF,   CR,     0x00, 0x00, 0x00, 0x00
.db   0x00, 0x00, 0x1C, 0x1E,   0x1F, 0x00, 0x00, 0x00

table_shifted_chars:
.db "1", "Q", "A", "Z",  0 , ",", "K", "I"
.db "2", "W", "S", "X",  SP, ".", "L", "O"
.db "3", "E", "D", "C",  0 , "/", ";", "P"
.db "4", "R", "F", "V",  BS,  0 , ":", "@"

.db "5", "T", "G", "B",  0 ,0x1F, "]", "["
.db "6", "Y", "H", "N",  0 ,0x1D,  CR,  0
.db "7", "U", "J", "M",  0 ,0x1C,0x1E,  0
.db  0 ,  0 , '\', "^", "-", "0", "9", "8"

table_unshifted_chars:
.db  "!", "q", "a", "z",  0 , "<", "k", "i"
.db "\"", "w", "s", "x",  SP, ">", "l", "o"
.db  "#", "e", "d", "c",  0 , "?", "+", "p"
.db  "$", "r", "f", "v",  BS , "_", "*", "`"		

.db  "%", "t", "g", "b",  0 ,  0 , "}", "{"
.db  "&", "y", "h", "n",  0 ,  0 ,  CR,  0
.db  "'", "u", "j", "m",  0 ,  0 ,  0 ,  0
.db   0 ,  0 , "|", "~", "=",  0 , ")", "("



;    Dec	Hex	ASCII										Key
;  0	00	NUL (null)									ctrl @
;  1	01	SOH (start of heading)			ctrl A
;  2	02	STX (start of text)					ctrl B
;  3	03	ETX (end of text)						ctrl C
;  4	04	EOT (end of transmission)		ctrl D
;  5	05	ENQ (enquiry)								ctrl E
;  6	06	ACK (acknowledge)						ctrl F
;  7	07	BEL (bell)									ctrl G

;  8	08	BS  (backspace)							ctrl H
;  9	09	HT  (horizontal tab)				ctrl I
; 10	0A	LF  (line feed)							ctrl J
; 11	0B	VT  (vertical tab)					ctrl K
; 12	0C	FF  (form feed)							ctrl L
; 13	0D	CR  (carriage return)				ctrl M
; 14	0E	SO  (shift out)							ctrl N
; 15	0F	SI  (shift in)							ctrl O

; 16	10	DLE (data link escape)			ctrl P
; 17	11	DC1 (device control 1)			ctrl Q
; 18	12	DC2 (device control 2)			ctrl R
; 19	13	DC3 (device control 3)			ctrl S
; 20	14	DC4 (device control 4)			ctrl T
; 21	15	NAK (negative acknowledge)	ctrl U
; 22	16	SYN (synchronous idle)			ctrl V
; 23	17	ETB (end of transm. block)	ctrl W

; 24	18	CAN (cancel)								ctrl X
; 25	19	EM  (end of medium)					ctrl Y
; 26	1A	SUB (substitute)						ctrl Z
; 27	1B	ESC (escape)								ctrl [
; 28	1C	FS  (file separator)				ctrl \
; 29	1D	GS  (group separator)				ctrl ]
; 30	1E	RS  (record separator)			ctrl ^
; 31	1F	US  (unit separator)				ctrl _
