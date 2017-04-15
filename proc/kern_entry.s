[GLOBAL kernel_thread_entry]
[EXTERN do_exit]
kernel_thread_entry:
    push edx
    call ebx

    push eax
    call do_exit