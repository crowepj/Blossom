global _start
_start:
	push rbp


	call printf
	mov RDI,1
	call exit
	pop rbp


extern printf
extern exit