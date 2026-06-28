    bits 64

    section .data

    global count_ones
    global fibb
    global replace_char

    extern data_x
    extern count
    extern result_num
    extern iterations
    extern text
    extern from
    extern replace_to

    section .text


count_ones:
    mov eax, [data_x]
    xor ecx, ecx

.opakovani:
    test eax, eax
    jz .hotovo
    
    shr eax, 1
    jnc .opakovani
    inc ecx

.preskocit:
    jmp .opakovani

.hotovo:
    mov [count], ecx
    ret

fibb:
    mov ecx, [iterations]
    xor eax, eax
    mov edx, 1

    test ecx, ecx
    jle .fibb_hotovo

.fibb_opakovani:
    dec ecx

    mov r8d, eax 
    add r8d, edx 

    mov eax, edx
    mov edx, r8d

    test ecx, ecx
    jnz .fibb_opakovani

.fibb_hotovo:
    mov [result_num], edx
    ret

replace_char:
    mov rdi, text
    mov al, [from]
    mov bl, [replace_to]

.char_opakovani:
    mov cl, [rdi]
    test cl, cl
    jz .char_hotovo

    cmp cl, al 
    jne .char_dalsi

    mov [rdi], bl

.char_dalsi:
    inc rdi
    jmp .char_opakovani

.char_hotovo:
    ret
