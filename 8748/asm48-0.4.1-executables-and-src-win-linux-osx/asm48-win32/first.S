.org 0x0000

reset:
	mov A, 0xFF
	outl P1, A
	nop
	nop
	mov A, 0x0000
	outl P1, A
	jmp reset
