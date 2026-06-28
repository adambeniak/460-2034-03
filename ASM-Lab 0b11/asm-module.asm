    bits 64
    section .text

    global faktorialy
    global maska
    global spocitani_jednicek

;; ULOHA 1    

    faktorialy:
        enter 0, 0 
        mov rcx, 0 
    .faktorialy_opakovani:
        cmp rcx, rsi 
        jge .faktorialy_konec 

        mov rax, [rdi + rcx*8] 

        cmp rax, 0 
        jl .nastavit_nulu 
        cmp rax, 1
        jle .faktorialy_ulozit 

        test rax, rax
        jnz .pocitani 
        mov rax, 1 
        jmp .faktorialy_ulozit

    .pocitani:
        mov r8, rax 
        mov rax, 1 

    .nasobeni:
        cmp r8, 1
        jle .faktorialy_ulozit 
        imul rax, r8 
        jo .nastavit_nulu 
        dec r8 
        jmp .nasobeni

    .nastavit_nulu:
        mov rax, 0

    .faktorialy_ulozit:
        mov [rdi + rcx*8], rax 
        inc rcx 
        jmp .faktorialy_opakovani

    .faktorialy_konec:
        leave
        ret

    maska:
        enter 0, 0
        mov eax, 0x03FFFFFF 
        mov rcx, 0 

    .maska_opakovani:
        mov dl, [rdi + rcx] 
        test dl, dl 
        jz .maska_konec 

        cmp dl, 'a' 
        jl .preskocit
        cmp dl, 'z' 
        jg .preskocit

        sub dl, 'a' 
        mov r8d, 1

        push rcx
        mov cl, dl
        shl r8d, cl 
        pop rcx 

        not r8d 
        and eax, r8d 

    .preskocit:
        inc rcx
        jmp .maska_opakovani

    .maska_konec:
        leave
        ret

    spocitani_jednicek:
        enter 0, 0
        mov eax, 0
        mov ecx, 32

    .jednicky_opakovani:
        shr edi, 1
        adc eax, 0
        dec ecx
        jnz .jednicky_opakovani 

        leave
        ret




