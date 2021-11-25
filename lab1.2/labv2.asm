section  .data ;Data segment
userMsg db 'Please input x and y : ',10 ;Ask the user to enter a number
lenUserMsg equ $-userMsg ;The length of the message
dispMsg db 'You have entered: '
lenDispMsg equ $-dispMsg 
zero db '0'
s1: db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
s2: db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
a: db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
b: db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
sum: db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
mulRes: dd 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
finalSum: db '0',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
finalMulRes: db '0',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
inputStr db 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
signA: db 0
signB db 0
addOF db 0
newLine db 10
minusSign db '-'
section .bss;Uninitialized data
len1: resb 1
len2: resb 1
addMaxLen: resb 1
mulMaxLen: resb 1
section .text   ;Code Segment
global main:
main:
input:
   mov eax, 4
   mov ebx, 1
   mov ecx, userMsg
   mov edx, lenUserMsg
   int 80h
;    ;Read and store the user input
;    mov eax, 3
;    mov ebx, 2
;    mov ecx, s1  
;    mov edx, 50   ;5 bytes (numeric, 1 for sign) of that information
;    int 80h
;     ;Read and store the user input
;    mov eax, 3
;    mov ebx, 2
;    mov ecx, s2  
;    mov edx, 50   ;5 bytes (numeric, 1 for sign) of that information
;    int 80h
call _inputWithSign

; 获取输入的长度
; mov     eax, s1                         ; 0407 _ 8D. 84 24, 0000011E
; mov     dword [esp], eax                         ; 040E _ 89. 04 24
; call    _getStringLen                           ; 0411 _ E8, FFFFFF9F
; mov     byte [len1], al                   ; 0416 _ 89. 84 24, 0000015C
; mov     eax, s2                         ; 0407 _ 8D. 84 24, 0000011E
; mov     dword [esp], eax                         ; 040E _ 89. 04 24
; call    _getStringLen                           ; 0411 _ E8, FFFFFF9F
; mov     byte [len2], al                   ; 0416 _ 89. 84 24, 0000015C




; reverseCharArrayToIntArray(s1, len1, a);
    mov     eax, a                         ; 04B3 _ 8D. 84 24, 00000284
    mov     dword [esp+8H], eax                     ; 04BA _ 89. 44 24, 08
    movzx     eax, byte [len1]                 ; 04BE _ 8B. 84 24, 000003BC
    mov     dword [esp+4H], eax                     ; 04C5 _ 89. 44 24, 04
    mov     eax, s1                       
    mov     dword [esp], eax                        ; 04D0 _ 89. 04 24
    call    _reverseCharArrayToIntArray             ; 04D3 _ E8, FFFFFB28

; reverseCharArrayToIntArray(s2, len2, b);
    mov     eax, b                         ; 04B3 _ 8D. 84 24, 00000284
    mov     dword [esp+8H], eax                     ; 04BA _ 89. 44 24, 08
    movzx     eax, byte [len2]                 ; 04BE _ 8B. 84 24, 000003BC
    mov     dword [esp+4H], eax                     ; 04C5 _ 89. 44 24, 04
    mov     eax, s2                       ; 04C9 _ 8D. 84 24, 0000037E
    mov     dword [esp], eax                        ; 04D0 _ 89. 04 24
    call    _reverseCharArrayToIntArray             ; 04D3 _ E8, FFFFFB28
; add
    movzx eax,byte[signA]
    movzx ebx,byte[signB]
    cmp eax,ebx
    jz as_signAEqualssignB
    ; if(signA!=signB):
    movzx     eax, byte [addMaxLen]                   
    mov     dword [esp+08H], eax                    ; 0593 _ 89. 44 24, 0C
    mov     eax, b                       ; 05A2 _ 8D. 84 24, 000000E8
    mov     dword [esp+4H], eax                     ; 05A9 _ 89. 44 24, 04
    mov     eax, a                       ; 05AD _ 8D. 84 24, 0000011A
    mov     dword [esp], eax                        ; 05B4 _ 89. 04 24
    call    _isSubOverflow
    mov byte[addOF], al
    cmp al,1
    jnz as_addOFNotEqualsOne
        movzx eax, byte[addMaxLen]
        mov     dword [esp+0CH], eax                    ; 0900 _ 89. 44 24, 0C
        mov     eax, sum                        ; 0904 _ 8D. 85, FFFFFEB9
        mov     dword [esp+8H], eax                     ; 090A _ 89. 44 24, 08
        mov     eax,   a                     ; 090E _ 8D. 85, FFFFFF4F
        mov     dword [esp+4H], eax                     ; 0914 _ 89. 44 24, 04
        mov     eax,  b                      ; 0918 _ 8D. 85, FFFFFF1D
        mov     dword [esp], eax                        ; 091E _ 89. 04 24
        call    _sub 
        jmp     as_allFinished
    as_addOFNotEqualsOne:
        movzx eax, byte[addMaxLen]
        mov     dword [esp+0CH], eax                    ; 0900 _ 89. 44 24, 0C
        mov     eax, sum                        ; 0904 _ 8D. 85, FFFFFEB9
        mov     dword [esp+8H], eax                     ; 090A _ 89. 44 24, 08
        mov     eax,   b                     ; 090E _ 8D. 85, FFFFFF4F
        mov     dword [esp+4H], eax                     ; 0914 _ 89. 44 24, 04
        mov     eax,  a                      ; 0918 _ 8D. 85, FFFFFF1D
        mov     dword [esp], eax                        ; 091E _ 89. 04 24
        call    _sub 
        jmp     as_allFinished
    as_signAEqualssignB:
        movzx     eax, byte [addMaxLen]                   
        mov     dword [esp+0CH], eax                    ; 0593 _ 89. 44 24, 0C
        mov     eax, sum                        ; 0597 _ 8D. 84 24, 000000B6
        mov     dword [esp+8H], eax                     ; 059E _ 89. 44 24, 08
        mov     eax, b                       ; 05A2 _ 8D. 84 24, 000000E8
        mov     dword [esp+4H], eax                     ; 05A9 _ 89. 44 24, 04
        mov     eax, a                       ; 05AD _ 8D. 84 24, 0000011A
        mov     dword [esp], eax                        ; 05B4 _ 89. 04 24
        call    _add
as_allFinished:  
; reversePrint
    mov     eax, finalSum                          ; 05BC _ 8D. 44 24, 52
    mov     dword [esp+8H], eax                     ; 05C0 _ 89. 44 24, 08
    movzx     eax, byte[addMaxLen]                 ; 05C4 _ 8B. 84 24, 000001B4
    mov     dword [esp+4H], eax                     ; 05CB _ 89. 44 24, 04
    mov     eax, sum                       ; 05CF _ 8D. 84 24, 000000B6
    mov     dword [esp], eax                        ; 05D6 _ 89. 04 24
    call    _generateFinalSum                       ; 05D9 _ E8, FFFFFAF5
;     jmp printSumWord
; ?_finishPrintSumWord:
; _mul(a,b,mulRes,*len1,*len2,*mulMaxLen);
    movzx     eax, byte [mulMaxLen]                   ; 05DE _ 8B. 84 24, 000001B0
    mov     dword [esp+14H], eax                    ; 05E5 _ 89. 44 24, 14
    movzx     eax, byte [len2]                   ; 05E9 _ 8B. 84 24, 000001B8
    mov     dword [esp+10H], eax                    ; 05F0 _ 89. 44 24, 10
    movzx     eax, byte[len1]                  ; 05F4 _ 8B. 84 24, 000001BC
    mov     dword [esp+0CH], eax                    ; 05FB _ 89. 44 24, 0C
    mov     eax, mulRes                         ; 05FF _ 8D. 84 24, 00000084
    mov     dword [esp+8H], eax                     ; 0606 _ 89. 44 24, 08
    mov     eax, b                         ; 060A _ 8D. 84 24, 000000E8
    mov     dword [esp+4H], eax                     ; 0611 _ 89. 44 24, 04
    mov     eax, a                        ; 0615 _ 8D. 84 24, 0000011A
    mov     dword [esp], eax                        ; 061C _ 89. 04 24
    call    _mul                                    ; 061F _ E8, FFFFFBCF
; reversePrint
    mov     eax, finalMulRes                         ; 0624 _ 8D. 44 24, 20
    mov     dword [esp+8H], eax                     ; 0628 _ 89. 44 24, 08
    movzx     eax, byte[mulMaxLen]                  ; 062C _ 8B. 84 24, 000001B0
    mov     dword [esp+4H], eax                     ; 0633 _ 89. 44 24, 04
    mov     eax, mulRes                       ; 0637 _ 8D. 84 24, 00000084
    mov     dword [esp], eax                        ; 063E _ 89. 04 24
    call    _generateFinalProduct 
;     jmp printMulWord
; ?_finishPrintMulWord:
;     call _finalPrint
    _finishPrintZero:
    call _output
jmp exit

; void input(void){
;     int i = 0;
;     int j = 0;
;     char temp = inputStr[i];
;     while(temp!=' '){
;         s1[i] = temp;
;         i++;
;         temp = inputStr[i];
;     }
;     len1 = i;

;     i++;
;     temp = inputStr[i];
;     while(temp!='\n'){
;         s2[j] = temp;
;         i++;
;         j++;
;         temp=inputStr[i]
;     }
;     len2 = j;
; }


; void input(){
;     int i = 0;
;     int j = 0;
;     char temp = inputStr[i];
    
    
;     if(temp=='-'){
;         *signA = 1;
;         i++;
;         temp = inputStr[i];
;     }

;     while(temp!=' '){
;         s1[j] = temp;
;         i++;
;         j++;
;         temp = inputStr[i];
;     }

;     *len1 = j;

;     i++;
;     j = 0;

;     temp = inputStr[i];

;     if(temp=='-'){
;         *signB = 1;
;         i++;
;         temp = inputStr[i];
;     }

;     while(temp!='\n'){
;         s2[j] = temp;
;         i++;
;         j++;
;         temp = inputStr[i];
;     }
;     *len2 = j;

; }
_inputWithSign:
        push    ebp                                     
        mov     ebp, esp                                
        sub     esp, 16 
        ;Read and store the user input
        mov eax, 3
        mov ebx, 2
        mov ecx, inputStr  
        mov edx, 50   
        int 80h

        mov     dword [ebp-4H], 0                       ; i=0
        mov     dword [ebp-8H], 0                       ; j=0
        mov     edx, dword [ebp-4H]                     ;edx=i
        mov     eax, inputStr                     ; eax=inputStr
        add     eax, edx                                ; eax=inputStr+i
        movzx   eax, byte [eax]                         ; eax=inputStr[i]
        mov     byte [ebp-9H], al                       ; temp=eax
        cmp     byte [ebp-9H], 45                       ; if(temp!='-')
        jne     is2                                  
        mov     eax, signA                    
        mov     byte [eax], 1                         
        add     dword [ebp-4H], 1                       ; i++
        mov     edx, dword [ebp-4H]                     ; edx=i
        mov     eax, inputStr                     ;eax=inputStr
        add     eax, edx                                ; eax=inputStr+i
        movzx   eax, byte [eax]                         ; eax=inputStr[i]
        mov     byte [ebp-9H], al                       ; temp=eax
        jmp     is2                                  ; 

is1:  mov     edx, dword [ebp-8H]                     ; edx=j
        mov     eax, s1                    ; eax=s1
        add     edx, eax                                ; edx=s1+j
        movzx   eax, byte [ebp-9H]                      ; eax=temp
        mov     byte [edx], al                          ; s1[j]=temp
        add     dword [ebp-4H], 1                       ; i++
        add     dword [ebp-8H], 1                       ; j++
        mov     edx, dword [ebp-4H]                     ; edx=i
        mov     eax, inputStr                     ; eax=inputStr
        add     eax, edx                                ; eax=inputStr+i
        movzx   eax, byte [eax]                         ; eax=inputStr[i]
        mov     byte [ebp-9H], al                       ;temp=eax
is2:  cmp     byte [ebp-9H], 32                       ;if(temp!=' ')
        jne     is1                                  ; 
        mov     eax, dword [ebp-8H]                     ; eax=j
        mov     edx, eax                                ; edx=j
        mov     byte [len1], dl                          ; len1=j
        add     dword [ebp-4H], 1                       ; i++
        mov     dword [ebp-8H], 0                       ; j=0
        mov     edx, dword [ebp-4H]                     ; edx=i
        mov     eax, inputStr                     ; eax=inputStr
        add     eax, edx                                ; eax=inputStr+i
        movzx   eax, byte [eax]                         ; eax=inputStr[i]
        mov     byte [ebp-9H], al                       ; temp=eax
        cmp     byte [ebp-9H], 45                       ; if(temp!='-')
        jne     is4                                  ; 0093 _ 75, 3E
        mov     eax, signB                    ; 
        mov     byte [eax], 1                           ; signB=1
        add     dword [ebp-4H], 1                       ; i++
        mov     edx, dword [ebp-4H]                     ; edx=i
        mov     eax, inputStr                    ; eax=inputStr
        add     eax, edx                                ; 
        movzx   eax, byte [eax]                         ; eax=inputStr[i]
        mov     byte [ebp-9H], al                       ; tmp=inputStr[i]
        jmp     is4                                  ; 00AD _ EB, 24

is3:  mov     edx, dword [ebp-8H]                     ; edx=j
        mov     eax, s2                    ; 
        add     edx, eax                                ; 
        movzx   eax, byte [ebp-9H]                      ; eax=temp
        mov     byte [edx], al                          ; s2[j]=temp
        add     dword [ebp-4H], 1                       ; i++
        add     dword [ebp-8H], 1                       ; j++
        mov     edx, dword [ebp-4H]                     ; edx=i
        mov     eax,  inputStr                     ; 
        add     eax, edx                                ; 
        movzx   eax, byte [eax]                         ; eax=inputStr[i]
        mov     byte [ebp-9H], al                       ;temp
is4:  cmp     byte [ebp-9H], 10                       ; if(temp!='/n')
        jne     is3                                   ; 00D7 _ 75, D6
        mov     eax, dword [ebp-8H]                     ; eax=j
        mov     edx, eax                                ; edx=j
        mov     eax, len2                    ; 00DE _ 8B. 45, 18
        mov     byte [eax], dl                          ;len2=j

         ; 得到addMaxLen
        movzx eax,byte [len1]
        movzx ebx,byte [len2]
        cmp eax,ebx
        JB setAddMaxLenB2; eax<ebx
        jmp setAddMaxLenA2
        setAddMaxLenB2: mov eax, ebx
        mov byte [addMaxLen], al
        jmp afterGetAddMaxLen2
        setAddMaxLenA2:
        mov byte [addMaxLen], al
        afterGetAddMaxLen2:
        ; 得到mulMaxLen
        movzx eax,byte [len1]
        movzx ebx,byte [len2]
        add eax,ebx
        mov byte[mulMaxLen], al


        nop                                             ; 00E3 _ 90
        leave                                           ; 00E4 _ C9
        ret                                             ; 00E5 _ C3
_input:
        push    ebp                                     ; 
        mov     ebp, esp                                ; 0001 _ 89. E5
        sub     esp, 16                                 ; 0003 _ 83. EC, 10
        mov     dword [ebp-4H], 0                       ; i=0
        mov     dword [ebp-8H], 0                       ; j=0

        ;Read and store the user input
        mov eax, 3
        mov ebx, 2
        mov ecx, inputStr  
        mov edx, 50   
        int 80h

        mov     edx, inputStr                  ; edx=inputStr
        mov     eax, dword [ebp-4H]                     ;eax=i;
        add     eax, edx                                ; eax=inputStr+i;
        movzx   eax, byte [eax]                         ; eax=inputStr[i];
        mov     byte [ebp-9H], al                       ; temp=inputStr[i];
        jmp     ?i_2                                   ; 0025 _ EB, 15

?i_1:  mov     edx, dword s1                        ; edx=s1
        mov     eax, dword [ebp-4H]                     ; eax=i
        add     edx, eax                                ; edx=s1+i
        movzx   eax, byte [ebp-9H]                      ;eax=temp
        mov     byte [edx], al                          ; s1[i]=temp
        add     dword [ebp-4H], 1                       ; i++
        mov     edx, inputStr                  ; edx=inputStr
        mov     eax, dword [ebp-4H]                     ; eax=i
        add     eax, edx                                ; eax=inputStr+i
        movzx   eax, byte [eax]                         ; eax=inputStr[i]
        mov     byte [ebp-9H], al                       ; temp=inputStr[i]
?i_2:  cmp     byte [ebp-9H], 32                       ; if(temp!=' ')跳转
        jnz     ?i_1                                   ; 
        mov     eax, dword [ebp-4H]                     ; eax=i;
        mov     byte [len1], al                      ; len1=i;
        add     dword [ebp-4H], 1                       ; i++;
        mov     edx, inputStr                  ; edx=inputStr
        mov     eax, dword [ebp-4H]                     ; eax=i
        add     eax, edx                                ; eax=inputStr+i
        movzx   eax, byte [eax]                         ; eax=inputStr[i]
        mov     byte [ebp-9H], al                       ; temp=inputStr[i]
        jmp     ?i_4                                   ; 

?i_3:  mov     edx, s2                                  ;   edx=s2
        mov     eax, dword [ebp-8H]                     ; eax=j
        add     edx, eax                                ; edx=s2+j
        movzx   eax, byte [ebp-9H]                      ; eax=temp
        mov     byte [edx], al                          ; s2[j]=temp
        add     dword [ebp-4H], 1                       ; i++
        add     dword [ebp-8H], 1                       ; j++
        mov     eax, dword [ebp-4H]                     ; eax=i
        mov     edx, inputStr
        add     eax, edx                                ; eax=inputStr+i
        movzx   eax, byte [eax]                         ; eax=inputStr[i]
        mov     byte [ebp-9H], al                       ; temp=inputStr[i]

?i_4:  cmp     byte [ebp-9H], 10                       ; if(temp!='\n') 跳转
        jnz     ?i_3                                   ; 007E _ 75, E1
        mov     eax, dword [ebp-8H]                     ; eax=j
        mov     byte [len2], al                      ; 0083 _ A3, 00000000(d)


        ; 得到addMaxLen
        movzx eax,byte [len1]
        movzx ebx,byte [len2]
        cmp eax,ebx
        JB setAddMaxLenB; eax<ebx
        jmp setAddMaxLenA
        setAddMaxLenB: mov eax, ebx
        mov byte [addMaxLen], al
        jmp afterGetAddMaxLen
        setAddMaxLenA:
        mov byte [addMaxLen], al
        afterGetAddMaxLen:
        ; 得到mulMaxLen
        movzx eax,byte [len1]
        movzx ebx,byte [len2]
        add eax,ebx
        mov byte[mulMaxLen], al

        nop                                             ; 0088 _ 90
        leave                                           ; 0089 _ C9
        ret               

_getStringLen:; Function begin ; BEENTESTED
push    ebp                                     
mov     ebp, esp                                 
sub     esp, 16                                  
mov     dword [ebp-4H], 0                        
jmp     gsl032                                   

gsl031:  
add     dword [ebp-4H], 1                        
gsl032:  
mov     edx, dword [ebp-4H]                      
mov     eax, dword [ebp+8H]                      
add     eax, edx                                 
movzx   eax, byte [eax]                         
cmp   al,10
je gsl322
cmp    al, 0                                  
jne     gsl031                                   
gsl322:  mov     eax, dword [ebp-4H]                     
leave                                           
ret                                            
; _getStringLen End of function

; void reverseCharArrayToIntArray(char *num, int len, char *res)
_reverseCharArrayToIntArray:
        push    ebp                                     ;
        mov     ebp, esp                                ;
        sub     esp, 16                                 ;
        mov     dword [ebp-4H], 0                       ; i: ebp-4; i=0
        jmp     r002                                   ; 

r001:  mov     eax, dword [ebp+0CH]                    ; eax=len
        sub     eax, dword [ebp-4H]                     ; eax=len-i
        sub eax,1
        mov     edx, eax                           ; edx=len-1-i
        mov     eax, dword [ebp+8H]                     ; eax= num//a pointer
        add     eax, edx                                ; eax= num+ len-1-i// 指向最后一个元素
        movzx   eax, byte [eax]                         ; 读取该元素: eax=num[len-1-i]
        mov     ecx, eax
        sub     ecx, 30H                          ; ecx= num[len-1-i]-'0'
        mov     edx, dword [ebp-4H]                     ; edx=i
        mov     eax, dword [ebp+10H]                    ; eax=res
        add     eax, edx                                ; eax=res+i
        mov     edx, ecx                                ; edx= num[len-1-i]
        mov     byte [eax], dl                          ; *eax==res[i] = edx==num[len-1-i]
        add     dword [ebp-4H], 1                       ; i++
r002:  mov     eax, dword [ebp-4H]                     ;eax=i
        cmp     eax, dword [ebp+0CH]                    ; eax vs. len
        jl      r001                                   ; if(eax<len) 跳转
        nop                                             ; 
        leave                                           ; 
        ret                                             ; 
; _reverseCharArrayToIntArray End of function




; void add(char *a, char *b, char *sum, int addMaxLen)
; {
;     int carry = 0;
;     for (int i = 0; i <= addMaxLen; i++)
;     {
;         char tempRes = a[i] + b[i] + carry;
;         if(tempRes>=10){
;             sum[i] = tempRes - 10;
;             carry = 1;
;         }else{
;             sum[i] = tempRes;
;             carry = 0;
;         }
        
;     }
; }
_add:   ; Function begin
        push    ebp                                     ; 
        mov     ebp, esp                                ; 
        sub     esp, 16                                 ; 
        mov     dword [ebp-4H], 0                       ; carry=0;
        mov     dword [ebp-8H], 0                       ; i=0;
        jmp     add015                                   ; 

add012:  mov     edx, dword [ebp-8H]                     ;edx=i
        mov     eax, dword [ebp+8H]                     ; eax=a
        add     eax, edx                                ; eax=a+i
        movzx   eax, byte [eax]                         ; eax=a[i]
        mov     ecx, eax                                ; ecx=a[i]
        mov     edx, dword [ebp-8H]                     ; edx=i
        mov     eax, dword [ebp+0CH]                    ; eax=b
        add     eax, edx                                ; eax=b+i
        movzx   eax, byte [eax]                         ; eax=b[i]
        add     eax, ecx                                ; eax=b[i]+a[i]

        mov     edx, dword [ebp-4H]                     ; edx=carry
        add     eax, edx                                ; eax==b[i]+a[i]+carry
        mov     byte [ebp-9H], al                       ; tempRes
        cmp     byte [ebp-9H], 9                        ; tempRes vs. 9
        jng     add013                                   ; if(tempRes<=9) 跳转
        movzx   eax, byte [ebp-9H]                      ; eax=tempRes
        lea     ecx, [eax-0AH]                          ; ecx=tempRes-10
        mov     edx, dword [ebp-8H]                     ; edx=i
        mov     eax, dword [ebp+10H]                    ; eax=sum
        add     eax, edx                                ; eax=sum+i
        mov     edx, ecx                                ; edx=tempRes-10
        mov     byte [eax], dl                          ; *eax==sum[i]   = tempRes-10
        mov     dword [ebp-4H], 1                       ; carry=1
        jmp     add014               ; 016E _ 83. 45, F8, 01
add013:  mov     edx, dword [ebp-8H]                     ; edx=i
        mov     eax, dword [ebp+10H]                    ; eax=sum
        add     edx, eax                                ; edx=sum+i
        movzx   eax, byte [ebp-9H]                      ; eax=tempRes
        mov     byte [edx], al                          ; sum[i]=tempRes
        mov     dword [ebp-4H], 0                       ; carry=0;
add014:  add     dword [ebp-8H], 1                       ; i++;
add015:  mov     eax, dword [ebp-8H]                     ; eax=i
        cmp     eax, dword [ebp+14H]                    ; eax vs. addMaxLen
        jng     add012                                   ; if(i<=addMaxLen) 跳转
        nop                                             ; 
        leave                                           ; 017B _ C9
        ret                                             ; 017C _ C3
; _add End of function

; void sub(char *a, char *b, char *sum, int addMaxLen)
; {
;     int carry = 0;
;     for (int i = 0; i < addMaxLen; i++)
;     {
;         char tempRes = a[i] - b[i] - carry;
;         if(tempRes<0){
;             sum[i] = tempRes + 10;
;             carry = 1;
;         }else{
;             sum[i] = tempRes;
;             carry = 0;
;         }
        
;     }
; }



; BEEN TESTED
; void reversePrintOnce(char* sum,int maxLen,char* finalSum){
;     int i = maxLen;
;     int j = 0;
;     while (sum[i] == 0)
;         i--;
;     if (i < 0)
;         printf("0\n");
;     else
;     {
;         for (; i >= 0; i--,j++)
;             finalSum[j] = sum[i];
;             //printf("%d", sum[i]);
;         finalSum[j] = 0;
;         printf(finalSum);
;     }

; }
_generateFinalSum:; Function begin
        push    ebp                                     
        mov     ebp, esp                                
        sub     esp, 40                                 
        mov     eax, dword [ebp+0CH]                    ;eax=maxLen
        mov     dword [ebp-0CH], eax                    ; 2thpara: i=maxLen;
        mov     dword [ebp-10H], 0                      ; 3rdpara: j=0;
        jmp     rp011                                   ; 

rp010:  sub     dword [ebp-0CH], 1                      ; i--;
rp011:  cmp     dword [ebp-0CH], 0
        jl rp015                                        ;i<0则输出0
        mov     edx, dword [ebp-0CH]                    ; edx=i
        mov     eax, dword [ebp+8H]                     ; eax=sum
        add     eax, edx                                ; eax= sum+i
        movzx   eax, byte [eax]                         ; eax=sum[i]
        test    al, al                                  ; if(eax==0)
        jz      rp010                                   ; 

        cmp     dword [ebp-0CH], -1                      ; if(i!=-1) 跳转
        jne     rp013                                   ; 
        jmp rp015 ;i<0 输出0
;?_finishPrintZero: jmp     rp014                                   ; 010D _ EB, 39

rp012:  mov     edx, dword [ebp-0CH]                    ; edx=i
        mov     eax, dword [ebp+8H]                     ; eax=sum
        add     eax, edx                                ; eax=sum+i
        mov     ecx, dword [ebp-10H]                    ; ecx=j
        mov     edx, dword [ebp+10H]                    ; edx=finalSum
        add     edx, ecx                                ; edx=finalSum+j
        movzx   eax, byte [eax]                         ; eax=sum[i]
        add eax,48; 转字符
        mov     byte [edx], al                          ; fnalSum[j]=sum[i]
        sub     dword [ebp-0CH], 1                      ; i--;
        add     dword [ebp-10H], 1                      ; j++;
rp013:  cmp     dword [ebp-0CH], -1                      ; i vs.-1
        jne     rp012                                   ; if(i!=-1) 跳转回去
        mov     edx, dword [ebp-10H]                    ; edx=j
        mov     eax, dword [ebp+10H]                    ; eax=finalSum
        add     eax, edx                                ; eax=finalSum+j
        mov     byte [eax], 0                           ; finalSum[j]=0;终止符'\0'
rp015:
rp014:  nop                                             ; 0148 _ 90
        leave                                           ; 0149 _ C9
        ret                                             ; 014A _ C3     

; _generateFinalSum End of function


_generateFinalProduct:; Function begin
        push    ebp                                     ; 00D3 _ 55
        mov     ebp, esp                                ; 00D4 _ 89. E5
        sub     esp, 40                                 ; 00D6 _ 83. EC, 28
        mov     eax, dword [ebp+0CH]                    ;eax=maxLen
        mov     dword [ebp-0CH], eax                    ; 2thpara: i=maxLen;
        mov     dword [ebp-10H], 0                      ; 3rdpara: j=0;
        jmp     rp2011                                   ; 

rp2010:  sub     dword [ebp-0CH], 1                      ; i--;
rp2011:  cmp     dword [ebp-0CH], 0
        jl rp2015
        mov     edx, dword [ebp-0CH]                    ; edx=i
        mov     eax, dword [ebp+8H]                     ; eax=sum
        imul edx,4
        add     eax, edx                                ; eax= sum+i
        movzx   eax, byte [eax]                         ; eax=sum[i]
        test    al, al                                  ; if(eax==0)
        jz      rp2010                                   ; 

        cmp     dword [ebp-0CH], -1                      ; if(i!=-1) 跳转
        jne     rp2013                                   ; 
        jmp rp2015

rp2012:  mov     edx, dword [ebp-0CH]                    ; edx=i
        mov     eax, dword [ebp+8H]                     ; eax=sum
        imul edx,4
        add     eax, edx                                ; eax=sum+i
        mov     ecx, dword [ebp-10H]                    ; ecx=j
        mov     edx, dword [ebp+10H]                    ; edx=finalSum
        add     edx, ecx                                ; edx=finalSum+j
        movzx   eax, byte [eax]                         ; eax=sum[i]
        add eax,48; 转字符
        mov     byte [edx], al                          ; fnalSum[j]=sum[i]
        sub     dword [ebp-0CH], 1                      ; i--;
        add     dword [ebp-10H], 1                      ; j++;
rp2013:  cmp     dword [ebp-0CH], -1                      ; i vs.-1
        jne     rp2012                                   ; if(i!=-1) 跳转回去
        mov     edx, dword [ebp-10H]                    ; edx=j
        mov     eax, dword [ebp+10H]                    ; eax=finalSum
        add     eax, edx                                ; eax=finalSum+j
        mov     byte [eax], 0                           ; finalSum[j]=0;终止符'\0'
rp2015:
rp2014:  nop                                             
        leave                                           
        ret                                       
; _reversePrint End of function
_finalPrint:; Function begin
        push    ebp                                     ; 00D3 _ 55
        mov     ebp, esp                                ; 00D4 _ 89. E5
        sub     esp, 12                                 ; 00D6 _ 83. EC, 28

        mov eax, 4
        mov ebx, 1
        mov ecx, finalSum
        mov edx, 50
        int 80h
        mov eax, 4
        mov ebx, 1
        mov ecx, newLine
        mov edx, 1
        int 80h  
        mov eax, 4
        mov ebx, 1
        mov ecx, finalMulRes
        mov edx, 50
        int 80h 

        nop                                             ; 0148 _ 90
        leave                                           ; 0149 _ C9
        ret                                             ; 014A _ C3


; BEEN TESTED
; void mul(char *a, char *b, char *mulRes, int len1, int len2, int mulMaxLen)
; {
;     for (int i = 0; i < len1; i++)
;         for (int j = 0; j < len2; j++)
;             mulRes[i + j] += a[i] * b[j];
;     for (int i = 0; i < mulMaxLen; i++)
;     {
;         char tempProduct = mulRes[i];
;         if (tempProduct >= 10)
;         {
;             char consult = tempProduct / 10;
;             char remainder = tempProduct % 10;
;             mulRes[i + 1] += consult;
;             mulRes[i] =remainder;
;         }
; ;     }
; }
_mul:   ; Function begin
        push    ebp                                     ; 01F3 _ 55
        mov     ebp, esp                                ; 01F4 _ 89. E5
        push    ebx                                     ; 01F6 _ 53
        sub     esp, 32                                 ; 01F7 _ 83. EC, 10
        mov     dword [ebp-8H], 0                       ; 1st: i=0;
        jmp     mul024                                   ; 0201 _ EB, 61

mul021:  mov     dword [ebp-0CH], 0                      ; j=0
        jmp     mul023                                   ;

; 内层循环 
mul022:  mov     edx, dword [ebp-8H]                     ; edx=i;
        mov     eax, dword [ebp-0CH]                    ; eax=j;
        add     eax, edx                                ; eax=i+j
        imul eax,4
        mov     edx, eax                                ; edx=i+j
        mov     eax, dword [ebp+10H]                    ; eax=mulRes
        add     eax, edx                                ; eax=mulres+i+j
        mov   eax, dword [eax]                         ; eax=mulRes[i+j]
        mov     ecx, eax                                ; ecx=mulRes[i+j]

        mov     edx, dword [ebp-8H]                     ; edx=i
        mov     eax, dword [ebp+8H]                     ; eax=a
        add     eax, edx                                ; eax=a+i
        movzx   eax, byte [eax]                         ; eax=a[i]
        mov     ebx, eax                                ; ebx=a[i]

        mov     edx, dword [ebp-0CH]                    ; edx=j
        mov     eax, dword [ebp+0CH]                    ; eax=b
        add     eax, edx                                ; eax=b+j
        movzx   eax, byte [eax]                         ; eax=b[j]
        mov     edx, eax                                ; edx=b[j]

        mov     eax, ebx                                ; eax=a[i]
        imul    eax, edx                                ; eax=a[i]*b[i]

        add     ecx, eax                                ; ecx += a[i]*b[j]// 即ecx=mulRes[i+j]+a[i]*b[j]
        mov     edx, dword [ebp-8H]                     ; edx=i;
        mov     eax, dword [ebp-0CH]                    ; eax=j;
        add     eax, edx                                ; eax=i+j;
        mov     edx, eax                                ; edx=i+j
        imul edx,4
        mov     eax, dword [ebp+10H]                    ; eax=mulRes
        add     eax, edx                                ; eax=mulRes+i+j
        mov     edx, ecx                                ; edx=所要求的和
        mov     dword [eax], edx                          ; mulRes[i+j]==*eax =edx
        add     dword [ebp-0CH], 1                      ; j++;
mul023:  mov     eax, dword [ebp-0CH]                    ; eax=j
        cmp     eax, dword [ebp+18H]                    ; 
        jl      mul022                                   ; if(j<len2)跳转
        add     dword [ebp-8H], 1                       ; i++;
mul024:  mov     eax, dword [ebp-8H]                     ; eax=i
        cmp     eax, dword [ebp+14H]                    ; i vs. len1'
        jl      mul021                                   ; if(i<len1)跳转
        mov     dword [ebp-10H], 0                      ; else i=0
        jmp     mul027                                   ; 

mul025: mov     edx, dword [ebp-10H]                    ; edx=i//新的局部变量i
        mov     eax, dword [ebp+10H]                    ; eax=mulRes
        imul edx,4
        add     eax, edx                                ; eax=mulRes+i
        mov     eax, dword [eax]                         ; eax=mulRes[i]
        mov     dword [ebp-14H], eax                      ; ebp-20: tempProduct=mulRes[i]
        cmp     dword [ebp-14H], 9                       ; 
        jng     mul026                                   ; if(tempProduct<=9)跳转
        mov   eax, dword [ebp-14H]                     ; eax=tempProduct
        mov ax, word[ebp-14H]
        ;movsx   dx, ax                                  ; dx=tempProduct
        ;mov ax,dx                                       ; ax=tempProduct
        mov dl,10
        div dl
        mov     byte [ebp-15H], al                      ; consult=al
        mov     byte [ebp-16H], ah                      ; remainder=ah
    ; DIV (unsigned divide) 无符号数除法
    ; 格式：DIV SRC
    ; 执行的操作:
    ; 　　字节操作：16位被除数在AX，8位除数为源操作数，结果的8位商在AL中，8位余数在AH中。表示为
    ; 　　（AL）<-(AX)/(SRC) 的商
    ; 　　 (AH) <-(AX)/(SRC) 的余数
    ; 　　字操作：32位被除数放在DX,AX中。其中DX为高位字，16位除数为源操作数，结果的16位端在AX中，16位余数在DX中。表示为
    ; 　　(AX)<-(DX,AX)/(SRC) 的商
    ; 　　(DX)<-(DX,AX)/(SRC) 的余数
    ; 　　双字操作:64位被除数在EDX,EAX中，其中EDX为高位双字，32位除数为源操作数，结果的32位商在EAX中，32位余数在EDX中，表示为
    ; 　　(EAX)<-(EDX,EAX)/(SRC) 的商
    ; 　　(EDX)<-(EDX,EAX)/(SRC) 的余数。
    ; 商和余数均为无符号数。

    ; IDIV 带符号除法指令
    ; 格式：IDIV SRC
    ; 执行的操作：与DIV相同，但操作数必须是带符号的数，商和余数也都是带符号的数，且余数的符号和被除数的符号相同

        mov     eax, dword [ebp-10H]                    ; eax=i
        add     eax,1
        mov     edx, eax                           ; edx=i+1
        imul edx,4
        mov     eax, dword [ebp+10H]                    ; eax=mulRes
        add     eax, edx                                ; eax=mulRes+i+1
        mov   eax, dword [eax]                         ; eax=mulRes[i+1]
        mov     edx, eax                                ; edx=mulRes[i+1]
        movzx   eax, byte [ebp-15H]                     ; eax=consult
        add     eax,edx
        mov     ecx, eax                          ; ecx= consult+mulRes[i+1]
        mov     eax, dword [ebp-10H]                    ; eax=i
        add eax,1
        mov     edx, eax                           ; edx=i+1
        imul edx,4
        mov     eax, dword [ebp+10H]                    ; eax=mulRes
        add     eax, edx                                ; eax=mulRes+i+1
        mov     edx, ecx                                ; edx= ecx
        mov     dword [eax], edx                          ; mulRes[i+1]=consult+mulRes[i+1]

        mov     edx, dword [ebp-10H]                    ; edx=i
        imul edx,4
        mov     eax, dword [ebp+10H]                    ; eax=mulRes
        add     edx, eax                                ; edx=mulRes+i
        movzx   eax, byte [ebp-16H]                     ; eax=remainder
        mov     dword [edx], eax                          ; mulRes[i]=remainder

mul026:  add     dword [ebp-10H], 1                      ; i++;
mul027:  mov     eax, dword [ebp-10H]                    ; eax=i
        cmp     eax, dword [ebp+1CH]                    ; i(i< mulMaxLen)
        jl      mul025                                   ; 
        nop                                             ; 0312 _ 90
        leave
        ret
; _mul End of function


_isSubOverflow:; Function begin
        push    ebp                                     ; 0294 _ 55
        mov     ebp, esp                                ; 0295 _ 89. E5
        sub     esp, 16                                 ; 0297 _ 83. EC, 10
        mov     dword [ebp-4H], 0                       ; carry=0
        mov     dword [ebp-8H], 0                       ; i=0
        jmp     of4                                   ; 

of1:  mov     edx, dword [ebp-8H]                     ; edx=i
        mov     eax, dword [ebp+8H]                     ; a
        add     eax, edx                                ; 02B0 _ 01. D0
        movzx   eax, byte [eax]                         ; 02B2 _ 0F B6. 00
        mov     ecx, eax                                ; ecx=a[i]
        mov     edx, dword [ebp-8H]                     ; i
        mov     eax, dword [ebp+0CH]                    ; b
        add     eax, edx                                ; 
        movzx   eax, byte [eax]                         ; eax=b[i]
        sub     ecx, eax                                ; ecx=a[i]-b[i]
        mov     eax, ecx                                ; eax=a[i]-b[i]

        mov     edx, dword [ebp-4H]                     ; edx=carry
        sub     eax, edx                                ; 
        mov     byte [ebp-9H], al                       ; temp
        cmp     byte [ebp-9H], 0                        ; 
        jl     of2                                   ; 02D2 _ 79, 09
        mov     dword [ebp-4H], 0                       ; carry=1
        jmp     of3                                   ; 02DB _ EB, 07

of2:  mov     dword [ebp-4H], 1                       ; 02DD _ C7. 45, FC, 00000000
of3:  add     dword [ebp-8H], 1                       ; i++
of4:  mov     eax, dword [ebp-8H]                     ; eax=i
        
        cmp     eax, dword [ebp+10H]                    ; if(i<addMaxLen)
        jl      of1                                  ; 
        mov     eax, dword [ebp-4H]                     ; eax=carry
        leave                                           ; 02F3 _ C9
        ret                                             ; 02F4 _ C3
; _isSubOverflow End of function
_sub:   ; Function begin
        push    ebp                                   
        mov     ebp, esp                               
        sub     esp, 16                                 
        mov     dword [ebp-4H], 0                       
        mov     dword [ebp-8H], 0                      
        jmp     sub5                                 

sub2:  mov     edx, dword [ebp-8H]                     ; edx=i
        mov     eax, dword [ebp+8H]                     ; 
        add     eax, edx                                ; 
        movzx   eax, byte [eax]                         ; 
        mov     ecx, eax                                ; 0
        mov     edx, dword [ebp-8H]                     ; 0318 _ 8B. 55, F8
        mov     eax, dword [ebp+0CH]                    ; 031B _ 8B. 45, 0C
        add     eax, edx                                ; 031E _ 01. D0
        movzx   eax, byte [eax]                         ; 0320 _ 0F B6. 00
        sub     ecx, eax                                ; 0323 _ 29. C1
        mov     eax, ecx                                ; 
        
        mov     edx, dword [ebp-4H]                     ; 0327 _ 8B. 55, FC
        sub     eax, edx                                ; 032A _ 29. D0
        mov     byte [ebp-9H], al                       ; 032C _ 88. 45, F7
        cmp     byte [ebp-9H], 0                        ; if(tempRes>0)
        jnl     sub3                                  ; 
        movzx   eax, byte [ebp-9H]                      ; eax=tempRes
        lea     ecx, [eax+0AH]                          ; ecx=tempRes+10
        mov     edx, dword [ebp-8H]                     ; 
        mov     eax, dword [ebp+10H]                    ; eax=sum
        add     eax, edx                                ; 0342 _ 01. D0
        mov     edx, ecx                                ; 0344 _ 89. CA
        mov     byte [eax], dl                          ; sum[i]=tempRes-10
        mov     dword [ebp-4H], 1                       ;carry=1
        jmp     sub4                                   ; 034F _ EB, 15

sub3:  mov     edx, dword [ebp-8H]                     ; 0351 _ 8B. 55, F8
        mov     eax, dword [ebp+10H]                    ; 0354 _ 8B. 45, 10
        add     edx, eax                                ; 0357 _ 01. C2
        movzx   eax, byte [ebp-9H]                      ; 0359 _ 0F B6. 45, F7
        mov     byte [edx], al                          ; 035D _ 88. 02
        mov     dword [ebp-4H], 0                       ; 035F _ C7. 45, FC, 00000000
sub4:  add     dword [ebp-8H], 1                       ; 0366 _ 83. 45, F8, 01
sub5:  mov     eax, dword [ebp+14H]                    ; eax=addMaxLen
        
        cmp     dword [ebp-8H], eax                     ; if(i<addMaxLen)
        jl      sub2                                 ;
        nop
        leave
        ret                                          ;
; _sub End of function

_output:
    push    ebp                                     ; 01F3 _ 55
    mov     ebp, esp                                ; 01F4 _ 89. E5
    push    ebx                                     ; 01F6 _ 53
    sub     esp, 4                                 ; 01F7 _ 83. EC, 10
    call _printAddSign
    mov eax, 4
    mov ebx, 1
    mov edx, 50
    mov ecx,finalSum
    int 80h

    mov eax, 4
    mov ebx, 1
    mov ecx, newLine
    mov edx, 1
    int 80h   
    call _printMulSign
    mov eax, 4
   mov ebx, 1
   mov edx, 50
   mov ecx,finalMulRes
   int 80h  

   nop                                             ; 0312 _ 90
        leave
        ret


_printAddSign:
    push    ebp                                     ; 01F3 _ 55
    mov     ebp, esp                                ; 01F4 _ 89. E5
    push    ebx                                     ; 01F6 _ 53
    sub     esp, 4                                 ; 01F7 _ 83. EC, 10

call _isFinalSumZero
cmp eax,1
je printAddSignFinished


    movzx eax,byte[signA]
    movzx ebx,byte[signB]
    movzx ecx,byte[addOF]
    cmp eax,ebx
    jnz addSignNotEqual
    addSignEqual:
    cmp eax, 1
    jne addSignEqualPositive
    mov eax, 4
    mov ebx, 1
    mov edx, 1
    mov ecx,minusSign
    int 80h
    jmp printAddSignFinished 
    
    addSignNotEqual:
        cmp ecx,0
        jne addSignNotEqual_signb
        movzx     eax,byte[signA]                        ; 05CF _ 8D. 84 24, 000000B6
        mov     dword [esp], eax                        ; 05D6 _ 89. 04 24
        call    _printASign                       ; 05D9 _ E8, FFFFFAF5
        jmp printAddSignFinished
    addSignNotEqual_signb:
        movzx     eax,byte[signB]                        ; 05CF _ 8D. 84 24, 000000B6
        mov     dword [esp], eax                        ; 05D6 _ 89. 04 24
        call    _printASign                       ; 05D9 _ E8, FFFFFAF5
        jmp printAddSignFinished

    addSignEqualPositive:
    printAddSignFinished:
        nop                                             ; 0312 _ 90
        leave
        ret

_printMulSign:
        push    ebp                                     ; 01F3 _ 55
        mov     ebp, esp                                ; 01F4 _ 89. E5
        push    ebx                                     ; 01F6 _ 53
        sub     esp, 4                                 ; 01F7 _ 83. EC, 10

call _isFinalProductZero
cmp eax,1
je printMulSignFinished

        movzx eax,byte[signA]
        movzx ebx,byte[signB]
        cmp eax,ebx
        je printMulSignFinished
        mov eax, 4
        mov ebx, 1
        mov edx, 1
        mov ecx,minusSign
        int 80h

printMulSignFinished:
        nop                                             ; 0312 _ 90
        leave
        ret

_printASign:
    push    ebp                                     ; 01F3 _ 55 
    mov     ebp, esp                                ; 01F4 _ 89. E5
    push    ebx                                     ; 01F6 _ 53
    sub     esp, 4                                 ; 01F7 _ 83. EC, 10
    mov eax,dword[ebp+8H]
    cmp eax,1
    jnz printASignFinished
    mov eax, 4
    mov ebx, 1
    mov edx, 1
    mov ecx,minusSign
    int 80h
    printASignFinished:
    nop                                             ; 0312 _ 90
    leave
    ret
_isFinalSumZero:
; 1表示为0
    push    ebp                                     ; 01F3 _ 55 
    mov     ebp, esp                                ; 01F4 _ 89. E5
    push    ebx                                     ; 01F6 _ 53
    sub     esp, 4                                 ; 01F7 _ 83. EC, 10
    movzx eax,byte[finalSum]
    cmp eax,48
    jne finalSumIsNotZero
    fnalSumIsZero:
    mov eax,1
    jmp isFinalSumZeroFinished
    finalSumIsNotZero:
    mov eax,0
    isFinalSumZeroFinished:
    nop                                             ; 0312 _ 90
    leave
    ret
_isFinalProductZero:
; 1表示为0
        push    ebp                                     ; 01F3 _ 55 
    mov     ebp, esp                                ; 01F4 _ 89. E5
    push    ebx                                     ; 01F6 _ 53
    sub     esp, 4                                 ; 01F7 _ 83. EC, 10
    movzx eax,byte[finalMulRes]
    cmp eax,48
    jne finalProductIsNotZero
    fnalProductIsZero:
    mov eax,1
    jmp isFinalProductZeroFinished
    finalProductIsNotZero:
    mov eax,0
    isFinalProductZeroFinished:
    nop                                             ; 0312 _ 90
    leave
    ret
exit:
   mov eax, 1
   mov ebx, 0
   int 80h

