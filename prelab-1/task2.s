    THUMB
    AREA    DATA, ALIGN=4 

    AREA    |.text|, CODE, READONLY, ALIGN=2
    EXPORT  main

main    
    NOP                 ; No operation

    MOV R0, #0          ; i = 0
    MOV R1, #2         ; n = 20
    MOV R2, #0          ; sum = 0

for
    CMP R0, R1          ; i < n ? (n = R1)
    BGE exitfor         ; Eger i >= n ise çik
    ADD R2, R2, R0      ; sum = sum + i
    ADD R0, #1          ; i++
    B for               ; Tekrar kontrol et

exitfor
    NOP                 ; Döngüden çikis

loop
    B loop              ; Sonsuz döngü

    ALIGN
    END
