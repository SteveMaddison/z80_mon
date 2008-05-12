;
;
;

;-------------------------------
;  global function identifiers
;-------------------------------

; link to main() in main.c
	.globl _main

; prototypes declared in 'util.h'
	.globl _outb			; i/o port output
	.globl _inb				; i/o port input

; prototypes declared 'util.h'
	.globl _get_sp			; get stack pointer


;-----------------
;  function code
;-----------------
	.area _CODE
_boot::
; These are the very first instructions executed when the machine is
; powered up or reset.
	di				; disable interrupts
	jp      _main	; jump to the main() function, in src/main.c (not a "call" as
    				; main() is void and we don't want to come back here...)


;----------------------------------------------------
;  void outb( BYTE port, BYTE byte );
;
;  outputs <byte> to i/o port <port>
;----------------------------------------------------
_outb::
	push	bc
	ld		hl, #0x0004
	add		hl, sp
	ld		c, (hl)
	inc		hl
	ld		b, (hl)
	out		(c), b
	pop		bc
	ret


;----------------------------------------------------
; BYTE inb( BYTE port );
;
; returns byte on i/o port <port>
;----------------------------------------------------
_inb::
	push	bc
	ld		hl, #0x0004
	add		hl, sp
	ld		c,(hl)
	in		l,(c)
	ld		h, #0x00
	pop		bc
	ret

;----------------------------------------------------
; size_t* get_sp( void );
;
; returns the stack pointer
;----------------------------------------------------
_get_sp::
	ld		hl, #0x0000
	add		hl, sp
	ret
