; kernel code segment selector (IDX(1) TI(0) PRL(0))
SEL_KCODE   EQU 0x8
; kernel data segment selector (IDX(2) TI(0) PRL(0))
SEL_KDATA   EQU 0x10

[GLOBAL gdt_flush]
gdt_flush:
    ; load argument into eax
    mov eax, [esp+4]  
    ; load gdt
    lgdt [eax]      

    ; load kdata segment sel
    mov ax, SEL_KDATA      
    mov ds, ax        
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ; jump to kcode segment sel
    ; use far jump to change cs
    jmp SEL_KCODE:.flush   
              
.flush:
    ret