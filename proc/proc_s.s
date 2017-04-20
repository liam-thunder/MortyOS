[GLOBAL kernel_thread_entry]
[EXTERN do_exit]
kernel_thread_entry:
    push edx
    call ebx

    push eax
    call do_exit


[GLOBAL switch_to]
switch_to:
    ; eax saved the pointer to from_ctx
    mov eax, [esp+4]
    ; save eip 
    push ebx
    mov ebx, [esp+4]
    mov [eax], ebx
    pop ebx
    
    mov [eax+4], esp
    mov [eax+8], ebp
    mov [eax+12], ecx
    mov [eax+16], edx
    mov [eax+20], esi
    mov [eax+24], edi
    mov [eax+28], ebp

    ; eax saved the pointer to to_ctx
    mov eax, [esp+8]

    mov ebp, [eax+28]
    mov edi, [eax+24]
    mov esi, [eax+20]
    mov edx, [eax+16]
    mov ecx, [eax+12]
    mov ebx, [eax+8]
    mov esp, [eax+4]
    ; load ebi to regs
    push ebx
    push ebx
    mov ebx, [eax]
    mov [esp+4], ebx
    pop ebx

    ret
