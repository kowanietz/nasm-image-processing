global invert_image


; This might be bad code. Improvements welcome.
; Inverts the colors of a BMP byte array

section .text

invert_image:
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

    xor rbx, rbx        ; lioop index = 0

.loop:
    cmp rbx, r12        ; loop condition (index < file length)
    jae .done

    mov al, [r8+rbx]    ; load pixel at location "rbx" in pixels array pionter (r8)
    movzx rdx, al
    mov rsi, 255
    sub rsi, rdx        ; 255 - pixel value
    mov [r8+rbx], sil   ; store inverted value back in the original location inb the pixels array

    inc rbx             ; basically i++
    jmp .loop

.done:
    pop r12             ; restore callee-saved registers from stack
    pop rbx
    ret
