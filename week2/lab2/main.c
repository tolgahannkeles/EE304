#include "stm32f10x.h"
#include<stdio.h>
#include<math.h>

void wait(){for(int i=0;i<0x010000;i++){}}

int main (void) {
	
	//------------------------------
	//      init the micro controller
	//------------------------------
	//enable the GPIO clock for port GPIOA
	RCC->APB2ENR |= 0x0004; // ...000100
	//Or we could use a more readable way:
	//RCC->APB2ENR |= 1<<2; // This ORs the 2nd bit (Note that bit numbers start from 0)
	
	//Let's set PA03 pin as output
	//We need to modify GPIOA_CRL register. To make it output with max speed 2Mhz:
	//Mode3[1:0] should be 10, CR3[1:0] might be 00, which is Push/Pull output.
	//Therefore, GPIOA_CRL[15:12] bits should be 0b0010=0x2 (Reference manual)
	//Note that we cannot use OR. We exacly want 0b0010.
	//Reset value of GPIOA is 0x4444 4444. Each 4 corresponds a pair of (Mode,CR)
	//So, the value to be loaded into the reg is 0x4444 2444
	GPIOA->CRL = 0x22222222;	//Leave every pin in reset mode except pin3. It is 
														//set to PushPull output at max 2Mhz speed.
	
	GPIOA->CRH = 0x22222222;
	//Or for a more readable but slow way we could do:
	//GPIOA->CRL = (GPIOA->CRL & ~(0xFF<<12)) | 2<<12;
	//now we are all set to go


	//forever do...
	for(;;){
		
		int i=1;
		int j= 65536 / 2;
		while(i<= 128 && j > 128){
			GPIOA->ODR = i | j;
			i *= 2;
			j /= 2;
			wait();
		}
		while(i>=1 && j <= 65536){
			i /= 2;
			j *= 2;
			GPIOA->ODR = i | j;
			wait();
		}
		
		

	}//for	

	
	return 0;
}
