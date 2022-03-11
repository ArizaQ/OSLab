
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               syscall.asm
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                     Forrest Yu, 2005
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

%include "sconst.inc"

_NR_get_ticks       equ 0 ; 要跟 global.c 中 sys_call_table 的定义相对应！
_NR_print_str      	equ 1 ; 
_NR_sleep_milli      	equ 2 ; 
_NR_p      	equ 3 ; 
_NR_v      	equ 4 ; 
INT_VECTOR_SYS_CALL equ 0x90

; 导出符号
global	get_ticks
global	print_str
global	sleep_milli
global	P
global	V

bits 32
[section .text]

; ====================================================================
;                              get_ticks
; ====================================================================
get_ticks:
	mov	eax, _NR_get_ticks
	int	INT_VECTOR_SYS_CALL
	ret

print_str:
	mov	eax, _NR_print_str  
	mov ebx,[esp+4]
	int	INT_VECTOR_SYS_CALL
	ret
sleep_milli:
	mov	eax,_NR_sleep_milli
	mov ebx,[esp+4]
	int	INT_VECTOR_SYS_CALL
	ret
P:
	mov eax,_NR_p
	mov ebx,[esp+4];
	int INT_VECTOR_SYS_CALL
	ret

V:
	mov eax,_NR_v
	mov ebx,[esp+4]
	int INT_VECTOR_SYS_CALL
	ret

