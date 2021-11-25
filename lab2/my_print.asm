global my_print
    section.text
my_print:
    push ebp
    mov ebp,esp

    mov ecx,[ebp+8h]
    mov edx,[ebp+0CH]
    mov eax, 4
    mov ebx, 1
    ; mov ecx, userMsg
    ; mov edx, lenUserMsg
    int 80h

    nop
    leave
    ret