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
mov al, [bx + si]                   ; works 16bit
mov bx, [bp + di]                   ; works 16bit
mov dx, [bp]                        ; works 24bit

; Source address calculation plus 8-bit displacement
mov ah, [bx + si + 4]               ; works 24bit

; Source address calculation plus 16-bit displacement
mov al, [bx + si + 4999]            ; works 32bit

; Dest address calculation
mov [bx + di], cx                   ; works
mov [bp + si], cl                   ; works
mov [bp], ch                        ; works

; mov immidiate to rm
; Explicit sizes 
mov [bp + di], byte 7               ; works 24bit
mov [di + 901], word 347            ; works 48bit