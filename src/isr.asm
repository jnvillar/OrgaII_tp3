; ** por compatibilidad se omiten tildes **
; ==============================================================================
; TRABAJO PRACTICO 3 - System Programming - ORGANIZACION DE COMPUTADOR II - FCEN
; ==============================================================================
; definicion de rutinas de atencion de interrupciones

%include "imprimir.mac"

BITS 32

sched_tarea_offset:     dd 0x00
sched_tarea_selector:   dw 0x00

;; PIC
extern fin_intr_pic1

;; Sched
extern sched_atender_tick
extern sched_tarea_actual

extern print
extern game_atender_tick
extern fin_intr_pic1
extern screen_actualizar_reloj_global
extern imprim
extern game_perro_actual
extern game_perro_mover
extern game_perro_cavar
extern game_perro_olfatear
extern game_atender_tick
extern sched_atender_tick
extern sched_proxima_a_ejecutar
extern game_syscall_manejar
extern sched_saltar_idle
extern esPerro
extern frenado
extern sched_saltar_idle
extern habilitar_pic


;;
;; Definición de MACROS
;; -------------------------------------------------------------------------- ;;

guardar: dw 0


exception0 db     'Divide Error', 0
exception1 db     'RESERVED', 0
exception2 db     'N MI Interrupt', 0
exception3 db     'Breakpoint', 0
exception4 db     'Overflow', 0
exception5 db     'BOUND Range Exceeded', 0
exception6 db     'Invalid Opcode', 0
exception7 db     'Device Not Available', 0
exception8 db     'Double Fault', 0
exception9 db     'Coprocessor Segment Overrun', 0
exception10 db     'Invalid TSS', 0
exception11 db     'Segment Not Present', 0
exception12 db     'Stack-Segment Fault', 0
exception13 db     'General Protection', 0
exception14 db     'Page Fault', 0
exception15 db     'RESERVED', 0
exception16 db     'Floating-Point Error', 0
exception17 db     'Alignment Check', 0
exception18 db     'Machine Check', 0
exception19 db     'SIMD Floating-Point Exception', 0


exception32 db     0
exception33 db     0


%macro ISR 1
global _isr%1

_isr%1:
	;xchg bx, bx
    
    push exception%1
    call esPerro
    add esp, 4

    

    call sched_saltar_idle
    jmp 0x68:0

        

%endmacro

;;
;; Datos
;; -------------------------------------------------------------------------- ;;
; Scheduler

;;
;; Rutina de atención de las EXCEPCIONES
;; 

ISR 0
ISR 1
ISR 2
ISR 3
ISR 4
ISR 5
ISR 6
ISR 7
ISR 8
ISR 9
ISR 10
ISR 11
ISR 12
ISR 13
ISR 14
ISR 15
ISR 16
ISR 17
ISR 18
ISR 19

;;--------------------------------------------------------------------------


;;
;; Rutina de atención del RELOJ
;; 

global _isr32
_isr32:

    pushad
    call fin_intr_pic1

    call frenado
    cmp eax, 1
    je .juegoEstaFrenado

    call sched_tarea_actual
    push eax    
    call game_atender_tick            ; llamo a atender tick con el perro anterior
    call sched_atender_tick

    shl ax, 3    
    str cx
    cmp ax,cx
    je .fin


    ;xchg bx, bx
    mov [sched_tarea_selector], ax
    jmp far [sched_tarea_offset]
    

    .fin:
       call screen_actualizar_reloj_global
    
    pop eax
    popad
        iret




    .juegoEstaFrenado:
        popad
        iret


;;------------------------------------------------------------------------- 

;;
;; Rutina de atención del TECLADO
;; 

global _isr33
_isr33:
    pushad
    call fin_intr_pic1
    in al, 0x60
    push eax
    call imprim
    add esp, 4
    popad
    ;xchg bx, bx
    iret

 


;;------------------------------------------------------------------------ 

;;
;; Rutinas de atención de las SYSCALLS
;;----------------------------------------------------------------------- ;;

;global _isr70
;_isr70:
;    pushad
;    push ecx
;    push eax
;	call game_syscall_manejar
;	add esp, 4
;	add esp, 4
;
;    mov [guardar], eax
;
 ;   popad
;
 ;   mov eax, [guardar]
;
 ;   
 ;   jmp 0x68:0          ; CORREGIR ESTO
;
 ;   iret


global _isr70
_isr70:
    pushad
    push eax
    push ecx
    call sched_tarea_actual
    cmp eax, 0
    je .saltarIdle
    pop ecx
    pop eax

    push ecx
    push eax
    call game_syscall_manejar
    add esp, 4
    add esp, 4

    mov [guardar], eax

    popad

    mov eax, [guardar]
    call sched_saltar_idle          ; CORREGIR
    .saltarIdle:
        jmp 0x68:0          

    iret


