; kernel data segment selector (IDX(2) TI(0) PRL(0))
SEL_KDATA EQU 0x10

[GLOBAL idt_flush]
idt_flush:
    mov eax, [esp+4]
    lidt [eax]
    ret
.end:

%macro ISR_NOERRCODE 1
[GLOBAL isr%1]
isr%1:
    cli
    push 0
    push %1
    jmp common_stub
%endmacro

%macro ISR_ERRCODE 1
[GLOBAL isr%1]
isr%1:
    cli
    push %1
    jmp common_stub
%endmacro

ISR_NOERRCODE  0
ISR_NOERRCODE  1
ISR_NOERRCODE  2
ISR_NOERRCODE  3
ISR_NOERRCODE  4
ISR_NOERRCODE  5
ISR_NOERRCODE  6
ISR_NOERRCODE  7
ISR_ERRCODE    8
ISR_NOERRCODE  9
ISR_ERRCODE   10
ISR_ERRCODE   11
ISR_ERRCODE   12
ISR_ERRCODE   13
ISR_ERRCODE   14
ISR_NOERRCODE 15
ISR_NOERRCODE 16
ISR_ERRCODE   17
ISR_NOERRCODE 18
ISR_NOERRCODE 19

ISR_NOERRCODE 20
ISR_NOERRCODE 21
ISR_NOERRCODE 22
ISR_NOERRCODE 23
ISR_NOERRCODE 24
ISR_NOERRCODE 25
ISR_NOERRCODE 26
ISR_NOERRCODE 27
ISR_NOERRCODE 28
ISR_NOERRCODE 29
ISR_NOERRCODE 30
ISR_NOERRCODE 31


%macro IRQ 2
[GLOBAL irq%1]
irq%1:
    cli
    push 0
    push %2
    jmp common_stub
%endmacro

IRQ   0,    32
IRQ   1,    33
IRQ   2,    34
IRQ   3,    35
IRQ   4,    36
IRQ   5,    37
IRQ   6,    38
IRQ   7,    39
IRQ   8,    40
IRQ   9,    41
IRQ  10,    42
IRQ  11,    43
IRQ  12,    44
IRQ  13,    45
IRQ  14,    46
IRQ  15,    47

; use push byte 0x80 will overflow
[GLOBAL systemcall]
systemcall:
    cli 
    push 0
    push 0x80
    jmp common_stub

[GLOBAL common_stub]
[EXTERN trap_handler]
common_stub:
    pusha

    push ds
    push es
    push fs
    push gs

    mov ax, SEL_KDATA 
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    push esp
    call trap_handler
    add esp, 4

    jmp common_ret
 .end:


 [GLOBAL common_ret]
common_ret:
    ; pop edi,esi,ebp,esp,ebx,edx,ecx,eax
    pop gs
    pop fs
    pop es
    pop ds
    popa
    ; clean up the pushed error code and isr number
    add esp, 8
    iret
.end: