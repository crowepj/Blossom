global _start
_start:
	push rbp
	mov rbp, rsp
	mov RDI,SC0
	call printf
	mov RDI,1
	call exit
	pop rbp
	ret
SC0:db "Test",0
extern printf
extern exit
