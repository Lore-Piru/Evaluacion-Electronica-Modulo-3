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
*Ejercicio número 2
*Consigna:
*Realice un programa que permita al Arduino variar la intensidad 
*lumínica de un led en función de la tensión de entrada del canal 
*analógico A2. El mínimo de intensidad deberá ser cuando la tensión de
*entrada sea de 4V y la intensidad máxima deberá ser cuando la tensión
*de entrada sea de 2V. 
*Aclaración:
*Incluyo los extremos del lapso.
************************************************************************
*/
/************************Definición de constantes***********************/
//Constantes para entradas y salidas
#define POTENCIOMETRO A0    //Esta será la entrada analógica
#define LED            9    //Esta será la salida PWM

//Constantes para el programa

#define min_tension          0//La tensión del arduino va de 0v a 5v
#define max_tension       5000//Pero lo utilizaré en mV
#define min_pote_mvolts   2000//El lapso que se pide en la consigna en mV
#define max_pote_mvolts   4000
#define min_pote_in          0//El lapso de valores que puede tomar el pote
#define max_pote_in       1023

//Constantes por comodidad y costumbre
#define ON                HIGH
#define OFF                LOW

/********************Declaración de variables globales******************/
int valorDelPote;

void setup() {
  //Inicialización de entradas y salidas
  pinMode(POTENCIOMETRO, INPUT );
  pinMode(LED          , OUTPUT);
}

void loop() {
  valorDelPote = analogRead(POTENCIOMETRO);
  //1023 de lectura corresponde a 5v en una entrada analógica.
  valorDelPote = map(valorDelPote,min_pote_in,max_pote_in,min_tension,max_tension);

  if(poteEntreLapso()){ 
    //Si el potenciómetro está en rango prende el led, si no, apaga el led.
    analogWrite(LED, valorDeseadoLed());
  }else{
    digitalWrite(LED, OFF);
  }
  delay(500);
}

boolean poteEntreLapso(){ 
  //Devuelve un true o false según si está o no en el lapso.
  return ((valorDelPote >= min_pote_mvolts) && (valorDelPote <= max_pote_mvolts));
  //Al tener entre () una expresión booleana devuelve true o false.
}

int valorDeseadoLed(){
  return map(valorDelPote, 4000, 2000, 0, 255);
  //Con ese map logro que 4000mV en la entrada sea 0 en la salida.
  //Y que 2000mV en la entrada sea 255 en la salida.
  //(Considerando los valores intermedios con el map)
}
