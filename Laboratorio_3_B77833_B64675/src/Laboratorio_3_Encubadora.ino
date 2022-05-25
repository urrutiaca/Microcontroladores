//Librerias utulizadas
#include <Adafruit_PCD8544.h>
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdio.h>

//Declaracion de pines
//LEDS
const int Luz_Roja=2;
const int Luz_Azul=3;
// Sensor de Humedad
const int Sensor_Humedad= A5;
//Termistor
const int Pin_termistor=A4;
int Vo=0;
const float R1=10000;
float logRT, RT, T;
float c1 = 1.009249522e-03, c2 = 2.378405444e-04, c3 = 2.019202697e-07;
//PID
const int Punto_op=A3; //Punto de operación

//Comunicacion con la PC
const int comunicacion=13;

// Configuracion de pantalla PCD8544
// Informacion extraída de: https://www.electrosoftcloud.com/pantalla-lcd-nokia-5110-con-arduino-parte-2/
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 5, 6, 4, 8);


float Temperatura_Termistor(){ // Informacion extraída de: https://www.circuitbasics.com/arduino-thermistor-temperature-sensor-tutorial/ 
 Vo= analogRead(Pin_termistor);
 RT = R1 * (1023.0 / (float)Vo - 1.0);
 logRT = log(RT);
 T = (1.0 / (c1 + c2*logRT + c3*logRT*logRT*logRT));
 T = T - 273.15; // Temperatura en Celsius
 return T;
}

void Leds_Alarma(){
 if(T>42.0){  //Se enciende luz roja si Temperatura>42 grados celsius
 	digitalWrite(Luz_Roja, HIGH);
	digitalWrite(Luz_Azul, LOW);

	}
 else if(T<32.0){ //Se enciende luz azul si Temperatura<32 grados celsius
	digitalWrite(Luz_Azul, HIGH);
	digitalWrite(Luz_Roja, LOW);

	}
 else{ //Si la temperatura esta dentro del rango de operacion no se enciende ningun leds
	digitalWrite(Luz_Azul, LOW);
	digitalWrite(Luz_Roja, LOW);

	}
}

void setup() {
  Serial.begin(9600); // Inicio del puerto serial
 // Configuracion de pines como salida
  pinMode(Luz_Roja,OUTPUT);
  pinMode(Luz_Azul,OUTPUT);
  pinMode(comunicacion, INPUT);
 // Setup de la pantalla PCD8544
  display.begin();
  display.setContrast(50); // Contraste de la pantalla
  display.display(); //
  delay(500);
  display.clearDisplay();   // Limpia la pantalla 
  display.setTextSize(1);  // Tamano de fuente
  display.setTextColor(BLACK); //Color de texto
}


void loop() { //Loop
	 //Porcentaje de Humedad
	
	Leds_Alarma();
	display.print("Temp: \n");
	display.print(Temperatura_Termistor()); //Temperatura Termistor
	 display.print("C"); 
	 display.print("\n");
	 display.print("Humedad:");
	 display.print(analogRead(Sensor_Humedad)/10.23); //Porcentaje de Humedad simulado con la fuente de voltaje de 5 V.
	 display.print("%"); 
	 display.print("\n");
	 display.print("P Op:");
	 display.print(analogRead(Punto_op)/10.23); //Punto de operacion PID. 
	 display.print("\n");
      display.display();
	 delay(500);
	 display.clearDisplay();
	if(digitalRead(comunicacion)==LOW){ //Valores en el puerto serial
		Serial.print("--------------------- \n");
		Serial.print("TEMPERATURA:");
    		Serial.print(T);
    		Serial.println("C");
    		Serial.print("HUMEDAD:");
    		Serial.print(analogRead(Sensor_Humedad)/10.23);
    		Serial.println("%");
	
	}
  
}




