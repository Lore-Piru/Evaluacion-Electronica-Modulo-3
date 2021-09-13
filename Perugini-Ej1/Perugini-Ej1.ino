/*
************************************************************************
*Sistema Dual - Tecnicatura en IT
*Primer Año
*Materia: Electrónica General
*Profesor: Matias Hampel
*Alumna: C. Lorena Perugini
*Mail: lorenaperuginikrause@gmail.com
*Empresa: Boehringer Ingelheim
*Ciclo Lectivo 2021
************************************************************************
*Ejercicio número 1
*Consigna:
*Realizar un programa que permita al Arduino encender un led cuando el 
*valor de la entrada analógica A0 esté comprendida entre 1 V y 2 V.
*Aclaración:
*No incluyo los extremos del lapso porque la consigna no dice inclusive.
************************************************************************
*/
/************************Definición de constantes***********************/
//Constantes para entradas y salidas
#define POTENCIOMETRO A0    //Esta será la entrada analógica
#define LED           8     //Esta será la salida digital

//Constantes para el programa

#define min_tension          0//La tensión del arduino va de 0v a 5v
#define max_tension       5000//Pero lo utilizaré en mV
#define min_pote_mvolts   1000//El lapso que se pide en la consigna en mV
#define max_pote_mvolts   2000
#define min_pote_in          0//El lapso de valores que puede tomar el pote
#define max_pote_in       1023

//Constantes por comodidad y costumbre
#define ON                HIGH
#define OFF                LOW

void setup() {
  //Inicialización de entradas y salidas
  pinMode(POTENCIOMETRO, INPUT );
  pinMode(LED          , OUTPUT);
}

void loop() {
  if(poteEntreLapso()){ 
    //Si el potenciómetro está en rango prende el led, si no, apaga el led.
    digitalWrite(LED, ON);
  }else{
    digitalWrite(LED, OFF);
  }
  delay(500);
}
boolean poteEntreLapso(){ 
  //Devuelve un true o false según si está o no en el lapso.
  int valorDelPote = analogRead(POTENCIOMETRO);
  //1023 de lectura corresponde a 5v en una entrada analógica.
  valorDelPote = map(valorDelPote,min_pote_in,max_pote_in,min_tension,max_tension);
  return ((valorDelPote > min_pote_mvolts) && (valorDelPote < max_pote_mvolts));
  //Al tener entre () una expresión booleana devuelve true o false.
}
