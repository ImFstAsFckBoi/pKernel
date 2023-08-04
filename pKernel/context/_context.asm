SECTION .data
ness:
    db "PK Fire!", 0   
SECTION	.text
    GLOBAL pk_ctx_switch
    GLOBAL pk_init_ctx
    GLOBAL pk_process_loader
    ALIGN 16
    BITS 32

    extern kernel
    extern pk_exit_handler
    extern printf
    extern fsync
    extern time_msecs

    %define ADDR_WIDTH  4

    %define K_CUR_OFF   12

    %include "pcb_offsets.asm"

    %define STATUS_NEW      0
    %define STATUS_RUNNING  1
    %define STATUS_DONE     2



%macro __pk_store_ctx 0
    ; Requires ECX = kernel.current
    
    mov [ECX + SP_OFF], ESP
    mov [ECX + BP_OFF], EBP
    mov [ECX + AX_OFF], EAX
    mov [ECX + BX_OFF], EBX
    mov [ECX + DX_OFF], EDX

%endmacro

%macro __pk_load_ctx 0
    ; Requires ECX = process to load

    mov ESP, [ECX + SP_OFF]
    mov EBP, [ECX + BP_OFF]
    mov EAX, [ECX + AX_OFF]
    mov EBX, [ECX + BX_OFF]
    mov EDX, [ECX + DX_OFF]

    ; mov ECX, [ECX + CX_OFF]
    ; ECX is only cross contenxt register
    ; will be junk for inital proc but will
    ; be saved on stack by yielding proc
%endmacro

%macro start_cpu_timer 0
    ; Require ECX = current process
    pusha
    push ECX
    call time_msecs
    pop ECX
    mov [ECX + START_TIME_OFF], EAX
    popa
%endmacro

%macro end_cpu_timer 0
    ; Require ECX = current process
    pusha
    push ECX
    call time_msecs
    pop ECX
    mov EBX, [ECX + START_TIME_OFF]
    sub EAX, EBX
    add [ECX + CPU_TIME_OFF], EAX
    popa
%endmacro

pk_ctx_switch:
    push EBP
    mov EBP, ESP
    push ECX

    mov ECX, DWORD [kernel + K_CUR_OFF]   ; ECX = kernel.current
    
    end_cpu_timer
    
    __pk_store_ctx                  ; Store current process registers
    
    mov DWORD [ECX + IP_OFF], .exit
    mov ECX, DWORD [EBP + 2*ADDR_WIDTH]        ; ECX = &process
    mov [kernel + K_CUR_OFF], ECX   ; kernel.current = &process
    jmp [ECX + IP_OFF]

.exit:
    mov ECX, DWORD [kernel + K_CUR_OFF]   ; kernel.current = &process
    __pk_load_ctx
    
    start_cpu_timer

    pop ECX
    mov ESP, EBP    ; pop all local variables
    pop EBP         ; restore EBP reg'
    ret             ;  return



%define arg1_off ADDR_WIDTH
pk_process_loader:

    mov ECX, DWORD [kernel + K_CUR_OFF]   ; ECX = kernel.current
    mov DWORD [ECX + STATUS_OFF], STATUS_RUNNING

    __pk_load_ctx
    
    start_cpu_timer

    ; Replace func entry function (arg1) with pk_exit_handler address
    ; Will look like return address to entry fucntion
    ; This fucntion has no return address since it will never return
    ; and is jumped to, not called
    ;
    ; Stack layout now
    ; EBP off |  -16 |  -12 |   -8 |   -4 |    0 |
    ;         +------+------+------+------+------+
    ;         | ???? | func | argc | argv | ???? |
    ;         +------+------+------+------+------+
    ;                   ^ESP
    ;                                       ^EBP
    ;
    ; Stack layout when entered entry function
    ; EBP off |    0 |    4 |    8 |   12 |   16 |
    ;         +------+------+------+------+------+
    ;         |  EBP |  ret | argc | argv | ???? |
    ;         +------+------+------+------+------+
    ;           ^ESP
    ;           ^EBP

    mov ECX, [ESP]
    mov DWORD [ESP], pk_exit_handler

    jmp ECX









































GLOBAL pk_fire
pk_fire:
    push ness
    call printf
    push 1
    call fsync
    jmp pk_fire
