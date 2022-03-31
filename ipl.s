.module ipl
	.globl 	_main
	.globl	_psg_reset, _psg_mute, psg_beep_1, psg_beep_2, psg_beep_3
	.globl	_vdp_reset, _vdp_setup_registers, scrolling_buffer, write_text
	.globl	_ppi_reset
	.globl  keyboard_interrupt
	.globl	_mem_size
	.globl	console_writechar
	.globl  l__INITIALIZER
	.globl  s__INITIALIZED
	.globl  s__INITIALIZER

;--------------------------------------------------------------------------
psg_port			= 0x7F
sc_ppi_control		= 0xDF
vdp_control 		= 0xBF
vdp_data 			= 0xBE

RAMSTART			= 0xC000
RAMSIZE				= 0x400
RAMEND				= (RAMSTART+RAMSIZE-1)
STACK_TOP			= (RAMSTART+RAMSIZE-8)

TIMI				= STACK_TOP


ASC_BS				= 0x08
ASC_FF				= 0x0C
ASC_CR				= 0x0D



;--------------------------------------------------------------------------

	.area _HEADER (ABS)



.org 0x00
    jp     	cold_reset
.org 0x04
    ret
.org 0x08
    ret
.org 0x0C
    ret
.org 0x10
    ret
.org 0x14
    ret
.org 0x18
    ret
.org 0x1C
    ret
.org 0x20
    ret
.org 0x24
    ret
.org 0x28
    ret
.org 0x2C
    ret
.org 0x30
    jp     	warm_reset
.org 0x34
    ret
.org 0x38
    jp		int_handler
.org 0x3C
    ret



.org 0x66
nmi_handler:
    jp		cold_reset		; NMI handler, connected to reset button




.org 0x0080
	.ascii "EDOS"



;-------------------------------------------
cold_reset:
;-------------------------------------------
		di
		im		1
		ld   	sp, #STACK_TOP

;*** RESET PSG
		ld		b, #5
psg_clean:
		ld		a,#0x9F								; PSG: silence all channels
		out		(#psg_port),a
		ld		a,#0xBF
		out		(#psg_port),a
		ld 		a,#0xDF
		out		(#psg_port),a
		ld		a,#0xFF
		out		(#psg_port),a
		djnz	psg_clean


;*** FAST RAM TEST
		ld		hl, #0x55aa
    	push	hl
    	pop		de
    	or		a
    	sbc		hl,de
    	jp		nz, psg_beep_2                      ; Error: Z80/RAM failure


;*** CLEAR RAM
		ld 		HL,#RAMSTART
		ld 		(HL),#0
		ld 		DE,#RAMSTART+1
		ld 		BC,#RAMSIZE-1
		ldir

		ld		a, #0xC9
		ld		(TIMI),a


;*** RESET VDP
		in     	a,(vdp_control)

		xor     a
		out    	(vdp_control),a
		ld     	a,#0x80
		out    	(vdp_control),a

		ld     	a,#0b10000000
		out    	(vdp_control),a
		ld     	a,#0x81
		out    	(vdp_control),a

		in     	a,(vdp_control)
		
; *** CLEAR VRAM ***
		xor		a				; point to vram address 0 to write
		out 	(vdp_control),a
		ld 		a,#0x40
		out 	(vdp_control),a

		ex 		(sp),hl			; wait a while
		ex 		(sp),hl

		xor		a
		ld		bc, #0x4000		; vram size = 16384 bytes
fillLoop:
		out 	(vdp_data),a
		dec 	bc
		ld 		a,b
		or 		c
		jp 		nz, fillLoop
		
		ld		bc, #65092
		call	sys_wait
		
		ld		hl, #TAG
		ld		(hl),#'E'
		inc		hl
		ld		(hl),#'N'
		inc		hl
		ld		(hl),#'D'
		inc		hl
		ld		(hl),#'V'
	
		
		jp		warm_reset


	.area	_CODE 

;-------------------------------------------
warm_reset:
;-------------------------------------------
		di
		im		1
		
		ld		sp, #STACK_TOP
		call	_ppi_reset
		call	_psg_reset
		call	_vdp_reset
		;call	_vdp_setup_registers


		ld		hl, #0x7C32
		ld		(_rng_seed), hl
		
		
	crt0_init_globals:
		ld		bc, #l__INITIALIZER
		ld		a, b
		or		a, c
		jp		z, crt0_skip_globals	
		ld		de, #s__INITIALIZED
		ld		hl, #s__INITIALIZER
		ldir
	crt0_skip_globals:

		
		ld		sp, #STACK_TOP
		call 	_main

_exit::
		halt
		rst 	0x00
1$:
		halt
		jr		1$




	
 ; =====================================================================
 ; I/O wait
 ; 3-58 Mhz = 279,33 ns per T-state
 ; 65092 iterations * 55 states = 3580060 states = 1,000018159 seconds
 ;
 ; BC = wait time (65092 = 1sec)

sys_wait::
    bit     #0,a    	 ; 8
    bit     #0,a    	 ; 8
    bit     #0,a    	 ; 8
    and     a,#255  	 ; 7
    dec     bc      	 ; 6
    ld      a,c     	 ; 4
    or      a,b     	 ; 4
    jp      nz, sys_wait  ; 10, total = 55 states per iteration
	ret

 ; =====================================================================
_sys_wait::
 ; =====================================================================
	push	ix
	ld		ix, #0
	add		ix, sp

	ld		b, 4(ix)
	ld		c, 5(ix)

	call	sys_wait
	
	pop		ix
 
	ret


 ; =====================================================================
_sys_rand16::
 ; =====================================================================
	push	ix
	ld		ix, #0
	add		ix, sp

	ld		b, 4(ix)
	ld		c, 5(ix)

	call	sys_wait
	
	pop		ix
 
	ret
	
; =====================================================================
_sys_frames16::
 ; =====================================================================
	ld		hl, (FRAMES1);
	ret


; =====================================================================
rand16:
; =====================================================================
	push	af
	push	bc
	
    ld		bc, (_rng_seed)
    ld  	hl, #253
    xor		a
    sbc		hl, bc
    sbc		a, b
    sbc		hl, bc
    sbc		a, b
    ld		c, a
    sbc		hl, bc
    jr		nc, rand16end
    inc		hl
rand16end:
    ld		(_rng_seed), hl
	pop		bc
	pop		af
    ret


;-------------------------------------------
int_handler:
;-------------------------------------------
	push    af
	push    bc
	push    de
	push    hl
	push    ix
	push    iy
	
	exx
	ex      af,af'		;'
	
	push    af
	push    bc
	push    de
	push    hl
	push    ix
	push    iy

	in     a,(vdp_control)
	in     a,(vdp_control)


	LD      HL,(FRAMES1)	; Fetch the first two bytes at FRAMES1.
	INC     HL				; Increment lowest two bytes of counter.
	LD      (FRAMES1),HL	; Place back in FRAMES1.
    LD      A,H				; Test if the result was zero.
    OR      L				;            
    JR      NZ, NOINC3		; Forward, if not, to KEY-INT

	LD		HL, #FRAMES3
    INC     (HL)			; otherwise increment FRAMES3 the third byte.
NOINC3:

	call	keyboard_interrupt

int_end:
	pop     iy
	pop     ix
	pop     hl
	pop     de
	pop     bc
	pop     af
	
	ex      af,af'			;'
	exx
	
	pop     iy
	pop     ix
	pop     hl
	pop     de
	pop     bc
	pop     af
	ei
	reti
	
	

;-------------------------------------------
;-------------------------------------------
;-------------------------------------------





	.area	_CODE
	.area	_INITIALIZER
	.area	_HOME
	.area	_GSINIT
	.area	_GSFINAL
	.area	_DATA
	.area	_INITIALIZED
	.area	_BSEG
	.area	_BSS
	.area	_HEAP
	.area	_CODE	



;-------------------------------------------
; SCB AREA
;-------------------------------------------
	.area	_SCB	(ABS)

.org	RAMSTART
KEYBD_BUFFER::					.ds 32		; <<< 256 BYTES ALIGNED!!!
keyboard_input_buffer::     	.ds 32		; 9 bytes .. 0xfc5e - first 7 bytes are low bytes of rasters, 8th is high bits, 9th is remaining control keys
KEYBD_PUTPNT::					.ds 2
KEYBD_GETPNT::					.ds 2
keyboard_last_char::			.ds 2
keyboard_typematic_cnt::		.ds 2
keyboard_diff_char::			.ds 1
keyboard_int_counter::			.ds 1
								
FRAMES1::						.ds 2
FRAMES3::						.ds 1
cursor_x::          			.ds 1    	; $fd00
cursor_y::          			.ds 1    	; $fd01
_write_char_value:: 			.ds 1    	; $fd02 	; char being written to the screen
scrolling_buffer::   			.ds 40		; $fd03 	; 40 bytes 
								.ds 5
vdp_reg0::						.ds 1
vdp_reg1::						.ds 1
_rng_seed::    					.ds 2
_mem_size::    					.ds 2
TAG::    						.ds 4



;-------------------------------------------
;-------------------------------------------
;-------------------------------------------


;	.area _BSS
;	.area _HEAP
;_HEAP_start::

