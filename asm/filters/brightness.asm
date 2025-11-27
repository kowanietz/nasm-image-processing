global brightness_image

section .text

brightness_image:
    push rbx
    push r12
    push r13

    movsxd r12, r8d     ; brightness parameter (extend to 64 bit)

    mov r8, rdi         ; pointer to *pixels
    mov r9, rsi         ; width
    mov r10, rdx        ; height
    mov r11, rcx        ; components (RGB/RGBA so 3/4)(

    mov rax, r9
    imul rax, r10
    imul rax, r11
    mov r13, rax

    xor rbx, rbx        ; loop index

.loop:
    cmp rbx, r13
    jae .done

    movzx rax, byte [r8 + rbx]  ; Load pixel and zero-extend to 64-bit in one go

    add rax, r12        ; rax = pixel + brightness (signed)


    test rax, rax       ; check if negative
    js .underflow       ; jump if flasg ist set

    cmp rax, 255        ; check if >255
    jg .overflow

    mov [r8 + rbx], al

    inc rbx
    jmp .loop

.underflow: ; set negative value to 0
    mov byte [r8 + rbx], 0
    inc rbx
    jmp .loop

.overflow:  ; set overflowing value to 255
    mov byte [r8 + rbx], 255
    inc rbx
    jmp .loop

.done:
    pop r13
    pop r12
    pop rbx
    ret
