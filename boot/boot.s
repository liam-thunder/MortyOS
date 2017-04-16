; -------------------------------------
;  boot.s -- Morty OS Kernel Starts Here
;            by ChenYou 2017/1/3
; -------------------------------------

MBOOT_HEADER_MAGIC  equ    0x1BADB002
MBOOT_PAGE_ALIGN    equ    1 << 0
MBOOT_MEM_INFO      equ    1 << 1
MBOOT_HEADER_FLAGS  equ    MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ    -(MBOOT_HEADER_MAGIC+MBOOT_HEADER_FLAGS)

[BITS 32]
; temp .text section starts here
; Use codes in .init.text to set up the temp paging
section .init.text

dd MBOOT_HEADER_MAGIC
dd MBOOT_HEADER_FLAGS
dd MBOOT_CHECKSUM

[GLOBAL start]
[GLOBAL mboot_ptr_tmp]
; [GLOBAL glb_mboot_ptr]
[EXTERN kernEntry]

start:
    cli
    mov [mboot_ptr_tmp], ebx
    mov esp, STACK_TOP
    add esp, 0FFFFFFF0H
    mov ebp, 0
    ;mov esp, STACK_TOP
    ;mov ebp, 0
    ;and esp, 0FFFFFFF0H
    ;mov [glb_mboot_ptr], ebx
    call kernEntry

;-----------------------
section .init.data
stack:  times 1024 db 0
STACK_TOP equ $-stack-1

mboot_ptr_tmp: dd 0
;-----------------------


;stop:
;   hlp
;   jmp stop
;-----------------------
;section .bss
;stack:
;   resb 32768
;glb_mboot_ptr:
;   resb 4
;STACK_TOP equ $-stack-1
;-----------------------

