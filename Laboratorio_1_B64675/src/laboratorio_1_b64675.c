//Luis Fernando Mora Delgado, B64675 
//Laboratorio 1: Introduccion a microcontroladores y manejo de GPIOS
//Este programa permite simular un dado simplificado utilizando leds, un boton, y el microcontrolador PIC12f675
#include <pic14/pic12f675.h>
typedef unsigned int word ;
word __at 0x2007 __CONFIG = (_WDTE_OFF & _WDT_OFF & _MCLRE_OFF );
void delay (unsigned int tiempo);
 
void main(void)
{

    TRISIO = 0b00000000; //Poner todos los pines como salidas menos GP3
    GPIO = 0x00; //Poner pines en bajo
    
    int random=0b101; //Valor inicial del numero aleatorio
    int newbit=0b000; //Valor inicial de newbit=0, necesario para el cambio del valor de random.
    unsigned int time = 100;
 
    //Loop forever
    while ( 1 )
    {
		//Generacion de numeros aleatorios utilizando el metodo "linear-feedback shift register" 
		newbit=(random ^ (random >> 1)) & 1;
		random= (random >> 1) | (newbit << 2);
		if (GP3)
		{
			if (random==0b001) // 1
			{
				GP0 = ~GP0;	
				delay(time);
				GPIO = 0x00;
			}
			else if (random==0b010) // 2
			{
				GP1 = ~GP1;
				delay(time);
				GPIO = 0x00;
			}
			else if (random==0b011) // 3
			{
				GP2 = ~GP2;
				delay(time);
				GPIO = 0x00;
			}
			else if (random==0b100) // 4
			{
				GP0 = ~GP0;
				GP2 = ~GP2;
				delay(time);
				GPIO = 0x00;
			}
			else if (random==0b101) // 5
			{
				GP1 = ~GP1;
				GP2 = ~GP2;
				delay(time);
				GPIO = 0x00;
			}
			else if (random==0b110) // 6
			{
				GP0 = ~GP0;
				GP1 = ~GP1;
				GP2 = ~GP2;
				delay(time);
				GPIO = 0x00;
			}
				
		}
		
	    
	}
    
 




}
void delay(unsigned int tiempo)
{
	unsigned int i;
	unsigned int j;

	for(i=0;i<tiempo;i++)
	  for(j=0;j<1275;j++);
}
