;
; Boot code for the ROM monitor
;
; These are the very first instructions executed when the machine is
; powered up or reset.
;

	.globl _main	; link to main() in main.c
	.area _CODE
_boot::
	di		; disable interrupts
	
	push	iy	; if we got here by means of a reset, push all registers
	push	ix	; to the stack in case we want to examine them later.
	push	hl
	push	de
	push	bc
	push	af
			
	jp      _main	; jump to the main() function, in main.c (not a "call" as
    			; main() is void and we don't want to come back here...)
