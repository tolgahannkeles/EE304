    THUMB
    AREA    DATA, ALIGN=4 

    AREA    |.text|, CODE, READONLY, ALIGN=2
    EXPORT  main

main
	MOV R1, #6 ; R1 -> N
	MOV R2, #0 ; R2 -> Sum
	MOV R3, #0 ; R3 -> i
	
for
	CMP R1, #0
	BEQ exit
	ADD R2, R2, R1
	SUB R1, R1, #1
	B for
		
exit
	
	ALIGN
	END
		