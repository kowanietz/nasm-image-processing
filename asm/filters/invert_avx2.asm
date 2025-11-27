global invert_image_avx2


; This might be even worse code than the non-SIMD version. Improvements even more welcome.
; Inverts the colors of a BMP byte array

section .text

invert_image_avx2:
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

    vpcmpeqb ymm1, ymm1, ymm1   ; sets all 32 bytes to 0xFF -> 255

    xor rbx, rbx

    mov r13, r12
    and r13, -32 ; round down to nearest multiple of 32 (basically r13-32 butwithout the risk of integer underflow)

.vector_loop:
    cmp rbx, r13
    jae .cleanup

    vmovdqu ymm0, [r8+rbx]      ; loads 32 bytes into vector register
    vpsubb ymm0, ymm1, ymm0     ; 0xFF... vector - pixel value (invert 32 pixels)
    vmovdqu [r8+rbx], ymm0      ; save inverted pixel vector

    add rbx, 32
    jmp .vector_loop

.cleanup:           ; AVX2 uses 32-byte registers, this loop cleans remaining pixels if not divisible by 32
    cmp rbx, r12
    jae .done

    mov al, [r8+rbx]
    movzx rdx, al
    mov rsi, 255
    sub rsi, rdx
    mov [r8+rbx], sil

    inc rbx
    jmp .cleanup

.done:
    pop r12             ; restore callee-saved registers from stack
    pop rbx
    ret
