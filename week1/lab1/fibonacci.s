	THUMB
	AREA	DATA, ALIGN=4 
				
	AREA	|.text|, CODE, READONLY, ALIGN=2
	EXPORT  main
		   
main					
		MOV R0, #1
		MOV R1, #1
		MOV R2, #0 ; result
		MOV R10, #0
for		CMP R10, #5
		BGE exitfor		; step 2.2 if false, jump to exit
		ADD R2, R1, R0
		MOV R1, R0
		MOV R0, R2
		ADD R10, #1	 	; step 4 i++
		B for		 	; step 5 go to step 2
exitfor	NOP				; step 6 end of the for loop
loop	B loop		; an infinite loop

	ALIGN	  
	END
