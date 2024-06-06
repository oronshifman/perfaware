; ========================================================================
; LISTING 39
; ========================================================================

bits 16

; Register-to-register
mov si, bx                          ; works 16bit
mov dh, al                          ; works 16bit

; 8-bit immediate-to-register
mov cl, 12                          ; works 16bit
mov ch, -12                         ; works 16bit

; 16-bit immediate-to-register
mov dx, 3948                        ; works 24bit
mov dx, -3948                       ; works 24bit

; Source address calculation
mov al, [bx + si]                   ; works 16bit   0x008a
mov bx, [bp + di]                   ; works 16bit   0x1b8b
mov dx, [bp]                        ; works 24bit   0x568b 00

; Source address calculation plus 8-bit displacement
mov ah, [bx + si + 4]               ; works 24bit   0x608a 04

; Source address calculation plus 16-bit displacement
mov al, [bx + si + 4999]            ; works 32bit   0x808a 1387

; Dest address calculation
mov [bx + di], cx                   ; works 16bit   0x0989
mov [bp + si], cl                   ; works 16bit   0x0a88
mov [bp], ch                        ; works 24bit   0x6e88 00

; mov immidiate to rm
; Explicit sizes 
mov [bp + di], byte 7               ; works 24bit   0x0300 07
mov [di + 901], word 347            ; works 48bit   0x85c7 0385 015b