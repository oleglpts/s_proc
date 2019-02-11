section	.text
	BITS 64
global	main
main:
	xor	eax, eax
	xor	ebx, ebx
	xor	ecx, ecx
	xor	edx, edx
	jmp	short string
code:	
	pop	rcx
	mov	bl, 1
	mov	dl, 14
	mov	al, 4
	int	0x80
	mov	eax, 1
        xor     ebx, ebx
	int	0x80
string:	call	code
	db	"Hello, world!", 0x0a
