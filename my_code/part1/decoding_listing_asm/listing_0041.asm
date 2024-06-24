; ========================================================================
;
; (C) Copyright 2023 by Molly Rocket, Inc., All Rights Reserved.
;
; This software is provided 'as-is', without any express or implied
; warranty. In no event will the authors be held liable for any damages
; arising from the use of this software.
;
; Please see https://computerenhance.com for further information
;
; ========================================================================

; ========================================================================
; LISTING 41
; ======================================================================

;                76543210  76|543|210 76543210 76543210 76543210 76543210
; rm to/from rm: 000000dw mod|reg|rm  disp-lo  disp-hi
;    imdt to rm: 100000sw mod|000|rm  disp-lo  disp-hi  data     data if s: w=01
;   imdt to acc: 0000010w data        data if w=1

bits 16                             
add bx, [bx+si]                     ; 0x1803
add bx, [bp]                        ; 0x5e03 00
add si, 2                           ; 0xc683 02, 1100 0110 1000 0011
add bp, 2                           ; 0xc583 02
add cx, 8                           ; 0xc183 08
add bx, [bp + 0]                    ; 0x5e03 00
add cx, [bx + 2]                    ; 0x4f03 02
add bh, [bp + si + 4]               ; 0x7a02 04
add di, [bp + di + 6]               ; 0x7b03 06
add [bx+si], bx                     ; 0x1801
add [bp], bx                        ; 0x5e01 00
add [bp + 0], bx                    ; 0x5e01 00
add [bx + 2], cx                    ; 0x4f01 02
add [bp + si + 4], bh               ; 0x7a00 04
add [bp + di + 6], di               ; 0x7b01 06
add byte [bx], 34                   ; 0x0780 22
add word [bp + si + 1000], 29       ; 0x8283 03e8 1d
add word [bp + si + 1000], 350      ; 0x8281 03e8 015e
add ax, [bp]                        ; 0x4603 00
add al, [bx + si]                   ; 0x0002
add ax, bx                          ; 0xd801
add al, ah                          ; 0xe000
add ax, 1000                        ; 0xe805 03
add al, -30                         ; 0xe204
add al, 9                           ; 0x0904

sub bx, [bx+si]
sub bx, [bp]
sub si, 2
sub bp, 2
sub cx, 8
sub bx, [bp + 0]
sub cx, [bx + 2]
sub bh, [bp + si + 4]
sub di, [bp + di + 6]
sub [bx+si], bx
sub [bp], bx
sub [bp + 0], bx
sub [bx + 2], cx
sub [bp + si + 4], bh
sub [bp + di + 6], di
sub byte [bx], 34
sub word [bx + di], 29
sub word [bp + si + 1000], 350
sub ax, [bp]
sub al, [bx + si]
sub ax, bx
sub al, ah
sub ax, 1000
sub al, -30
sub al, 9

cmp bx, [bx+si]
cmp bx, [bp]
cmp si, 2
cmp bp, 2
cmp cx, 8
cmp bx, [bp + 0]
cmp cx, [bx + 2]
cmp bh, [bp + si + 4]
cmp di, [bp + di + 6]
cmp [bx+si], bx
cmp [bp], bx
cmp [bp + 0], bx
cmp [bx + 2], cx
cmp [bp + si + 4], bh
cmp [bp + di + 6], di
cmp byte [bx], 34
cmp word [4834], 29             ; 0x3e83 12e2 001d      mod|reg|rm   op     |s|w  
cmp word [4834], 350            ; 0x3e81 12e2 015e      00 |111|110  1000 00|0|1  0001 0010 1110 0010
cmp ax, [bp]
cmp al, [bx + si]
cmp ax, bx
cmp al, ah
cmp ax, 1000
cmp al, -30
cmp al, 9

test_label0:
jnz test_label1
jnz test_label0
test_label1:
jnz test_label0
jnz test_label1

label:
je label
jl label
jle label
jb label
jbe label
jp label
jo label
js label
jne label
jnl label
jg label
jnb label
ja label
jnp label
jno label
jns label
loop label
loopz label
loopnz label
jcxz label