        bits    64

; aid for calling assembly functions from C programs
;
%macro  save_all        0
        push    eax
        push    ebx
        push    ecx
        push    edx
        push    esi
        push    edi
        push    ebp
%endmacro

%macro  restore_all     0
        pop     ebp
        pop     edi
        pop     esi
        pop     edx
        pop     ecx
        pop     ebx
        pop     eax
%endmacro

; make the global variables from the C module visible to the Asm module
;
;EXTERN  g_numbers1_count

; make the functions of the Asm module visible to the C module
;
;GLOBAL asm_example2


; Discuss what this program is doing and why the result is
; a list of numbers from 1 to 10
;
;asm_example2:
;        save_all
;        lea     esi, [g_numbers1]
;        lea     edi, [g_output]
;        mov     ecx, [g_numbers1_count]
;.again:
;        mov     eax, [esi]
;        mov     [edi], eax
;        add     esi, 4
;        add     edi, 4
;        dec     ecx
;        jnz     .again
;        restore_all
;        ret

