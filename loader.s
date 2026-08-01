global loader               ; the entry symbol for ELF
extern kmain                ; declare the C function we want to call

MAGIC_NUMBER equ 0x1BADB002     ; define the magic number constant
FLAGS        equ 0x0            ; multiboot flags
CHECKSUM     equ -MAGIC_NUMBER  ; calculate the checksum

section .text                   
align 4                         
    dd MAGIC_NUMBER             
    dd FLAGS                    
    dd CHECKSUM                 

loader:
    mov esp, kernel_stack + KERNEL_STACK_SIZE   ; setup stack

    ; --- 1. Clear the Framebuffer ---
    mov edi, 0x000B8000                         ; VGA text buffer start
    mov ecx, 2000                               ; 80 columns * 25 rows = 2000 cells
    mov ax, 0x0720                              ; 0x20 = space character, 0x07 = light grey on black

.clear_loop:
    mov [edi], ax                               ; write space to cell
    add edi, 2                                  ; move to next cell (2 bytes per cell)
    dec ecx
    jnz .clear_loop

    ; --- 2. Call the C Kernel ---
    call kmain                                  ; jump into kmain.c

.hang:
    cli                                         ; clear interrupt flag
    hlt                                         ; halt the CPU 
    jmp .hang

KERNEL_STACK_SIZE equ 4096      ; size of stack in bytes

section .bss
align 4                         
kernel_stack:                   
    resb KERNEL_STACK_SIZE