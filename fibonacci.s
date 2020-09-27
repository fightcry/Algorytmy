.intel_syntax noprefix

.global main
.text

main:
mov eax, [esp+8]
mov eax, [eax+4]

push eax
call atoi
add esp, 4

push eax
call f
add esp, 4

mov ebx, offset format
push eax
push ebx
call printf
add esp,8

xor eax, eax
ret

f:
    push ebp
    mov  ebp, esp
    sub  esp, 4

    mov  eax, [ebp+8]
    cmp  eax, 2
    jb   end

    dec  eax
    push eax
    call f

    mov  [ebp-4], eax
    dec  dword ptr [esp]
    call f
    add  esp,4
    add  eax, [ebp-4]
end:
    mov  esp, ebp
    pop  ebp
    ret

.data

format: .asciz "Wynik: %i\n"
