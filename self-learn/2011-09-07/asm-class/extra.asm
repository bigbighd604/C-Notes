
; aid for calling assembly functions from C programs
;
%macro  save_all        0
        push    ebx
        push    esi
        push    edi
        push    ebp
%endmacro

%macro  restore_all     0
        pop     ebp
        pop     edi
        pop     esi
        pop     ebx
%endmacro

; make the global variables from the C module visible to the Asm module
;
EXTERN  g_numbers1_count
EXTERN  g_numbers2_count
EXTERN  g_numbers1
EXTERN  g_numbers2
EXTERN  g_output
EXTERN  g_input1
EXTERN  g_input2
EXTERN  g_total
EXTERN  g_output_count
EXTERN  g_fibonacci_number
EXTERN  g_factorial_number
EXTERN  g_gcd_number1
EXTERN  g_gcd_number2

; make the functions of the Asm module visible to the C module
;
GLOBAL asm_example1
GLOBAL asm_example2
GLOBAL asm_sum_counter
GLOBAL asm_sum_numbers
GLOBAL asm_add_sub_numbers
GLOBAL asm_array_numbers
GLOBAL asm_fibonacci_number
GLOBAL asm_factorial_number
GLOBAL asm_gcd_number


; Discuss what this program is doing and why the result is 512
;
asm_example1:
        save_all
        mov     eax, 1
        mov     ecx, 9
.again:
        add     eax,eax
        dec     ecx
        jnz     .again
        mov     [g_total], eax
        restore_all
        ret



; Discuss what this program is doing and why the result is
; a list of numbers from 1 to 10
;
asm_example2:
        save_all
        lea     esi, [g_numbers1]
        lea     edi, [g_output]
        mov     ecx, [g_numbers1_count]
.again:
        mov     eax, [esi]
        mov     [edi], eax
        add     esi, 4
        add     edi, 4
        dec     ecx
        jnz     .again
        restore_all
        ret



; Write a program to count down to zero,
; and keep a sum of the values the counter is doing.
; The initial value of the counter is in g_input1.
; Put the result in g_total.
;
asm_sum_counter:
        save_all

;
; write your program here
;
	mov eax, 0
	mov ecx, [g_input1]
.again:
	add eax, ecx
	dec ecx
	jnz .again
	mov [g_total], eax
	restore_all
	ret



; Write a program here to sum all the numbers in the g_numbers1 array.
; Put the result in g_total.
;
asm_sum_numbers:
        save_all
;
; write your program here
;
; eax = temporary sum
; ebx = pointer to array position
; ecx = number of array elements
	mov eax, 0
	lea ebx, [g_numbers1]
	mov ecx, [g_numbers1_count]
.again:
	add eax, [ebx]
	add ebx, 4
	dec ecx
	jnz .again
	mov [g_total], eax
        restore_all
        ret



; Write a progam here to sum all the numbers in the g_numbers1 array
; and substract all numbers from the g_numbers2 array.
; Put the result in g_total.
;
asm_add_sub_numbers:
        save_all
;
; write your program here
;
	mov eax, 0
	mov ebx, 0
	lea ecx, [g_numbers1]
	lea edx, [g_numbers2]
	mov esi, [g_numbers1_count]
	mov edi, [g_numbers2_count]

.number1:
	add eax, [ecx]
	add ecx, 4
	dec esi
	jnz .number1

.number2:
	add ebx, [edx]
	add edx, 4
	dec edi
	jnz .number2

	sub eax, ebx
	mov [g_total], eax
        restore_all
        ret



; Write a progam here to substract each number in g_numbers2
; from the corresponding one in g_numbers1, and store the results
; in g_output.
;
asm_array_numbers:
        save_all
;
; write your program here
;
; eax = address pointer for g_numbers1 array
; ebx = address pointer for g_numbers2 array
; edx = address pointer for g_output

	mov ebx, 0
	mov ecx, [g_numbers1_count]

.again:
	mov eax, [g_numbers1 + ebx]
	sub eax, [g_numbers2 + ebx]
	mov [g_output + ebx], eax
	add ebx, 4
	dec ecx
	jnz .again
	
        restore_all
        ret


asm_fibonacci_number:
	save_all

	mov eax, 0
	mov ebx, 1
	mov [g_output], eax
	mov [g_output + 4], ebx
	mov ecx, [g_fibonacci_number]
	sub ecx, 1
	mov esi, 8

.again:
	mov edx, ebx
	add ebx, eax
	mov eax, edx
	mov [g_output + esi], ebx
	add esi, 4
	dec ecx
	jnz .again
	restore_all
	ret


asm_factorial_number:
	save_all
; use imul to multiply
; eax stores the result of each multiply

	mov eax, 1
	mov ebx, [g_factorial_number]

.again:
	imul eax, ebx
	dec ebx
	cmp ebx, 1
	jne .again
	mov [g_total], eax

	restore_all
	ret


asm_gcd_number:
	save_all

	mov eax, [g_gcd_number1]
	mov ebx, [g_gcd_number2]

.again:
	cmp eax, 0
	je .finish
	cmp ebx, 0
	je .finish
	cmp eax, ebx
	jl .less
	sub eax, ebx
	jmp .again

.less:
	sub ebx, eax
	jmp .again

.finish:
	add eax, ebx
	mov [g_total], eax
	restore_all
	ret
