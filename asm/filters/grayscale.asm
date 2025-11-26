global grayscale_image

section .text

grayscale_image:
    push rbx            ; rbx and r12 must be preserved (callee-saved registers)
    push r12

    mov r8, rdi         ; pointer to *pixels
    mov r9, rsi         ; width
    mov r10, rdx        ; height
    mov r11, rcx        ; components (RGB/RGBA so 3/4)(

    mov rax, r9
    imul rax, r10       ; width * height
    imul rax, r11       ; * components (for total file length in bytes)
    mov r12, rax

    xor rbx, rbx        ; loop index = 0

.loop:
    cmp rbx, r12        ; loop condition (index < file length)
    jae .done

    ; load R, G, B into 32-bit regs to avoid 8-bit overflow
    ; "byte" -> only grab 8 bits, extend the rest with zeros via movzx
    movzx eax, byte [r8 + rbx]       ; eax = R
    movzx edx, byte [r8 + rbx + 1]   ; edx = G
    movzx ecx, byte [r8 + rbx + 2]   ; ecx = B

    add eax, edx
    add eax, ecx        ; R+B+G

    ; division by 3 to calculate the average of R, G, B (evil black magic incoming (i stole this code))
    imul eax, 21846     ; multiply by 21846 (about 2^16 / 3)
    shr eax, 16         ; divide by 2^16
    mov dl, al

    ; write gray back to R, G, B
    mov [r8 + rbx], dl
    mov [r8 + rbx + 1], dl
    mov [r8 + rbx + 2], dl

    add rbx, r11 ; i++ 3 for RGB or 4 for RBGA
    jmp .loop

.done:
    pop r12             ; restore callee-saved registers from stack
    pop rbx
    ret
