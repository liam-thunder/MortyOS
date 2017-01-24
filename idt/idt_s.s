[GLOBAL idtFlush]
idtFlush:
	mov eax, [esp+4]
	lidt [eax]
	ret
.end:

; ISR Part
%macro ISR_NOERRCODE 1
[GLOBAL isr%1]
isr%1:
	cli
	push byte 0
	push byte %1
	jmp isr_common_stub
%endmacro

%macro ISR_ERRCODE 1
[GLOBAL isr%1]
isr%1:
	cli
	push byte %1
	jmp isr_common_stub
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

ISR_NOERRCODE 255

[GLOBAL isr_common_stub]
[EXTERN isrHandler]

; interrupt service routine
isr_common_stub:
	; push edi,esi,ebp,esp,ebx,edx,ecx,eax
	pusha
	mov ax, ds
	; save the data segment descriptor
	push eax

	; load the kernel data segment descriptor
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; save the pointer to registers_t struct
	push esp 
	call isrHandler
	; clean up the pushed argument
	add esp, 4

	; reload the original data segment descriptor
	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	; pop edi,esi,ebp,esp,ebx,edx,ecx,eax
	popa
	; clean up the pushed error code and isr number
	add esp, 8
	iret
.end:

; IRQ Part
%macro IRQ 2
[GLOBAL irq%1]
irq%1:
	cli
	push byte 0
	push byte %2
	jmp irq_common_stub
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

[GLOBAL irq_common_stub]
[EXTERN irqHandler]
irq_common_stub:
	pusha

	mov ax, ds
	push eax

	mov ax, 0x10 
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    push esp
    call irqHandler
    add esp, 4

    pop ebx         
    mov ds, bx
    mov es, bx
    mov fs, bx
    mov gs, bx
    mov ss, bx

    popa
    add esp, 8
    iret
 .end: