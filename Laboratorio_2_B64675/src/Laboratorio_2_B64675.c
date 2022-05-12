// LABORATORIO 2 
// Decripcion: 
// Autor: Luis Fernando Mora Delgado
// Referencias para la elaboracion del laboratorio: 1: https://www.gadgetronicx.com/attiny85-timer-tutorial-generating-time-delay-interrupts/ 2: https://www.gadgetronicx.com/attiny85-external-pin-change-interrupt/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
void Delay_tiempos();
//_______ESTADOS DE LA FMS________
#define LDPV 0         // Luz de paso de vehiculos.
#define LDVD 2         // Luz de vehıculos detenidos.
#define LDPP 1         // Luz de paso de peatones.
#define LDPD 3         // Luz de peatones detenidos
#define LDP  4         // Luz de parpadeo

//ESTADOS DE TIEMPO    // Estos estados van a definir que tipo de tiempo debe estarse contando
#define T_Vehiculos  5       // Tiempo Minimo de Paso de Vehiculos
#define T_Peatones   6       // Tiempo de parpadeo

//VARIABLES GLOBALES
int Pulso_botones =0; // Se inicializa el valor de pulso botones en 0
int Contador_ciclos=0;// Se encarga de contar los ciclos
int estado=0; //Se inicializa el estado LDPV por defecto
int estado_t=5; //Se inicializa como TminPV
int sec=0; // Contador de segundos

//INTERRUPCION BOTON
ISR (INT0_vect)
{
Pulso_botones=1; //Esta interrupcion me pone el valor de Pulso_botones=1
}
//INTERRUPCION DE TIEMPO PARA MANEJAR DELAY (Basado en link de referencia)
ISR (TIMER0_OVF_vect)      //Interrupt vector for Timer0
{
  Delay_tiempos();
}
//###########################################################################
//############################FUNCIONES######################################
//###########################################################################

//###########################################################################
//Esta funcion se encarga de ir contando ciclos de reloj para asi tener un conteo de los segundos, esta funcion se basa en la configuracion de los relojes que se hicieron basados en
//los links de referencia, en el cual se menciona que el tiempo que le tomara al timer0 incrementarse de 0 a 255 sera de 15.81ms por lo tanto cada 63 que repita esta accion se tendra
//aproximadamente 1s, ya que; 63x15.81ms=993.03ms que es aproximadamente 1 segundo
void Delay_tiempos(){
    if (Contador_ciclos==63) //waiting for 63 because to get 1 sec delay
  {
    Contador_ciclos=0; //making intr_count=0 to repeat the count
    ++sec;
  }
  else  Contador_ciclos++; //incrementing contador upto 63

}
//###########################################################################
//Esta funcion configura los relojes e habilita las interrupciones generales y timer0, ademas de hacer el prescaling de TCCR0B
void Timer_config() 
{
 TCCR0A = 0x00; //Modo normal
 TCCR0B = 0x00; 
 TCCR0B |= (1 << CS00) | (1 << CS02); //Prescaling a 1024 
 sei();// Interrupción global
 TCNT0 = 0;
 TIMSK |= (1 << TOIE0); //Habilitando la interrupción del timer0 
}
//###########################################################################
//CONFIGURACION DE INTERRUPCION EXTERNA: Basado en link de referencia
void interruptor_externo()  
{
  DDRB |= (1 << PB3)|(1 << PB2)|(1 << PB1) |(1 << PB0); //Valores de salida
  GIMSK |= (1<<INT0);     // habilitando el INT0 (interrupción externa)
  MCUCR |= (1<<ISC01)|(1<<ISC10)|(1<<ISC11); // Configurando como flanco  
}
//###########################################################################
//Esta funcion se encarga de llamar a las funciones que permiten configurar las interrupciones usadas en el laboratorio
void Setup_INTERRUPCIONES(){
    interruptor_externo();
    Timer_config();
}
//###########################################################################
void Semaforos_FMS(){
  switch (estado){
  //CASO LUZ DE PASO DE VEHICULOS  
  case LDPV:
      estado_t=T_Vehiculos; //Estado que permite hacer parpadear las luces luego de pasar del estado LDPV
      PORTB = (0<<PB3)|(1<<PB2)|(1<<PB1)|(0<<PB0); //Luz de paso vehicular se enciende y Luz Roja de peatones tambien 
      if(Pulso_botones==0) {
        estado=LDPV;
      }
      else if(Pulso_botones==1){
            if(sec>=10){
              Contador_ciclos=0; //Reinicio de contador
              sec=0;
              estado=LDP;
            }
            } 
            break;
  // CASO LUZ DE PARPADEO          
  case LDP:
    if(sec<3){
        if(estado_t==T_Vehiculos){
            if(Contador_ciclos==30||Contador_ciclos==90||Contador_ciclos==150) PORTB = (0<<PB3)|(0<<PB2)|(1<<PB1)|(0<<PB0);
            else if(Contador_ciclos==60||Contador_ciclos==120) PORTB = (0<<PB3)|(1<<PB2)|(1<<PB1)|(0<<PB0);
        }
        if(estado_t==T_Peatones){
            if(Contador_ciclos==30||Contador_ciclos==90||Contador_ciclos==150) PORTB = (1<<PB3)|(0<<PB2)|(0<<PB1)|(0<<PB0);
            else if(Contador_ciclos==60||Contador_ciclos==120) PORTB = (1<<PB3)|(0<<PB2)|(0<<PB1)|(1<<PB0);
        }
    
    }
    else {
        if(estado_t==T_Vehiculos){
            sec=0;
            Contador_ciclos=0;
            estado= LDVD;
        }
        if(estado_t==T_Peatones){
            sec=0;
            Contador_ciclos=0;
            estado= LDPD;
        }
        
    }
    break;
  //CASO LUZ DE VEHICULOS DETENIDOS          
  case LDVD:
    PORTB = (1<<PB3)|(0<<PB2)|(1<<PB1)|(0<<PB0);
    if(sec>=1){
    estado= LDPP;
    Contador_ciclos=0;
    sec=0;
    }
    else estado=LDVD;
    break; 
  //CASO LUZ DE PEATONES PASE
  case LDPP:
    PORTB =(1<<PB3)|(0<<PB2)|(0<<PB1)|(1<<PB0);
    estado_t=T_Peatones;
    if(sec>=10){
      estado= LDP;
      Contador_ciclos=0;
      sec=0;  
    }
    else estado=LDPP;
    break;
  //CASO LUZ DE PEATONES DETENIDOS
  case LDPD:
    PORTB = (1<<PB1)|(1<<PB3);
    if(sec>=1){
    estado= LDPV;
    Contador_ciclos=0;
    sec=0;
    Pulso_botones=0; //Se regresa el valor del boton a 0
    }
    else estado=LDPD;
    break; 
  }
}

int main(void)
{
  PORTB &= (0<<PB0) | (0<<PB1)| (0<<PB2)|(0<<PB3); // Se entablecen los pines B0,B1,B2 y B3 como salidas
  estado = LDPV; // Se inicializa en estado paso de vehiculos
  Pulso_botones = 0; //Se inicializa el valor del pulso de los botones en 0
  Setup_INTERRUPCIONES();
  while (1) {
    Semaforos_FMS(); //Maquina de estados de los semaforos empieza a funcionar
  }
  return 0;
}