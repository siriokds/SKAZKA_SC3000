.module psg

psg_port	= 0x7F


	.area _CODE

_psg_reset::
	push	af
	push	bc
	ld	bc,#32                               	; repeat count
psg_1:
	ld	a,#0xff                                 ; pause: 4082 cycles = 1.14ms
psg_2:
	dec	a
	jr	nz, psg_2

   	call	_psg_mute

	xor	a
	dec	bc
	ld	a,b
	or	c
	jr	nz, psg_1                               ; repeat 1000 times - maybe to make it audible on error?

	pop	bc
	pop	af
	ret


_psg_mute::
    ld     a,#0x9f                                 		; PSG: silence all channels
    out    (psg_port),a
    ld     a,#0xbf
    out    (psg_port),a
    ld     a,#0xdf
    out    (psg_port),a
    ld     a,#0xff
    out    (psg_port),a
	ret


;---------------------------------------
psg_beep_1::
;---------------------------------------
    ld     e,#0x01
    jr     psg_error_beep

;---------------------------------------
psg_beep_2::
;---------------------------------------
    ld     e,#0x02
    jr     psg_error_beep

;---------------------------------------
psg_beep_3::
;---------------------------------------
    ld     e,#0x03

    ; fall through


; inputs: e = beep count
; never returns
; beeps are of the approximate form (eg. e=2)
; -__-__________________-__-__________________-__-___________
; 0s            1s            2s             3s            4s
; where 1 char is ~70ms, the beep section is variable length but the long pauses arent

;---------------------------------------
psg_error_beep::
;---------------------------------------
    ld     d,e                                   ; e -> d

psg_beep_loop:
	ld     a,#0b10001111                         ; PSG: ch 0 frequency = %0000111111 = 1775.57Hz
    out    (psg_port),a
    ld     a,#0b00000011
    out    (psg_port),a                               ; PSG: ch 0 volume = 0 = full
    ld     a,#0b10010000
    out    (psg_port),a


    ld     b,#0x3e                                ; sound for 249924 cycles = 69.8ms
stb_2: 
		ld     c,#0xfb
stb_2b:  
		dec    c
    jr     nz, stb_2b
    djnz   stb_2


    ld     a,#0b10011111
    out    (psg_port),a

    ld     b,#0x7d                                 ; silence for 503877 cycles = 141ms
stb_3: 
		ld     c,#0xfb
stb_3b:  
		dec    c
    jr     nz, stb_3b
    djnz	 stb_3


    dec    d                                     ; repeat d times
    jp     nz,psg_beep_loop

    ld     hl,#0x03e8                              ; wait 4044005 cycles = 1130ms
stb_4: 
		ld     c,#0xfb
stb_4b:  
		dec    c
    jr     nz,stb_4b
    dec    hl
    ld     a,h
    or     l
    jr     nz, stb_4
    jp     psg_error_beep                  ; loop forever




psg_beep_wait:
	ld     c,#0xfb
pbwi:  
	dec    c
    jr     nz, pbwi
    djnz   psg_beep_wait
	ret
	
;---------------------------------------
_psg_beep::
;---------------------------------------
	push	bc
	push	de

;    ld      d,#1
;psg_beep_loop:

	ld     a,#0b10001111                         ; PSG: ch 0 frequency = %0000111111 = 1775.57Hz
    out    (psg_port),a
    ld     a,#0b00000111
    out    (psg_port),a                               

    ld     a,#0b10010000						; PSG: ch 0 volume = 0 = full
    out    (psg_port),a

    ld     	b,#0x0f                                ; sound for 249924 cycles = 69.8ms
	call	psg_beep_wait



    ld     a,#0b10011111
    out    (psg_port),a

    ld     b,#0x7d                                 ; silence for 503877 cycles = 141ms
s2tb_3: 
	ld     c,#0xfb
s2tb_3b:  
	dec    c
    jr     	nz, s2tb_3b
    djnz	s2tb_3


;    dec    d                                     ; repeat d times
;    jp     nz,psg_beep_loop

	pop		de
	pop 	bc
	
	ret






;---------------------------------------
_psg_beep_low::
;---------------------------------------
	push	bc
	push	de

;    ld      d,#1
;psg_beep_loop:

	ld     a,#0b10001111                          ; PSG: ch 0 frequency = %0000111111 = 1775.57Hz
    out    (psg_port),a
;    ld     a,#0b00000111
    ld     a,#0b00011111
	
    out    (psg_port),a                               ; PSG: ch 0 volume = 0 = full
    ld     a,#0b10010000
    out    (psg_port),a


;    ld     b,#0x2e                                ; sound for 249924 cycles = 69.8ms
	 ld     b,#0x1e                                ; sound for 249924 cycles = 69.8ms
s3tb_2: 
	ld     c,#0xfb
s3tb_2b:  
	dec    c
    jr     nz, s3tb_2b
    djnz   s3tb_2


    ld     a,#0b10011111
    out    (psg_port),a

    ld     b,#0x7d                                 ; silence for 503877 cycles = 141ms
s3tb_3: 
	ld     c,#0xfb
s3tb_3b:  
	dec    c
    jr     	nz, s3tb_3b
    djnz	s3tb_3



	pop		de
	pop 	bc
	ret





;---------------------------------------
_psg_tone::
;---------------------------------------
	push	bc
	push	de


	ld     a,#0b10001111                         ; PSG: ch 0 frequency = %0000111111 = 1775.57Hz
    out    (psg_port),a
    ld     a,#0b00000011
    out    (psg_port),a                               

	; PSG: ch 0 volume = 0 = full
    ld     a,#0b10010000
    out    (psg_port),a


    ld     b,#0x3e                                ; sound for 249924 cycles = 69.8ms
s4tb_2: 
	ld     c,#0xfb
s4tb_2b:  
	dec    c
    jr     nz, s4tb_2b
    djnz   s4tb_2


	; PSG: ch 0 volume = 15 = zero
    ld     a,#0b10011111
    out    (psg_port),a

	pop		de
	pop 	bc
	ret

