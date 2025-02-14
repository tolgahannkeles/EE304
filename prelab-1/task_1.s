	THUMB
	AREA	DATA, ALIGN=4 
				
	AREA	|.text|, CODE, READONLY, ALIGN=2
	EXPORT  main
		   
main	
	MOV R0, #2		; R0 = 2
	MOV R1, #3		; R1 = 3
	ADD R2, R0, R1 	; R2 = R0 + R1

loop B loop		; an infinite loop

	ALIGN	  
	END