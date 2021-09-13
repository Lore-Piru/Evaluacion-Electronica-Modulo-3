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
*Ejercicio número 3
*Consigna:

*Realice un sistema que funcione como un registrador de datos o
*Datalogger. Este sistema deberá transmitir los datos de tensión 
*presentes en sus entradas analógicas(A0, A1, A2 y A3) a través del 
*puerto serie cada X cantidad de tiempo.
*El sistema deberá permitir recibir comandos por el puerto serie 
*(8N1) para poder configurar los siguientes parámetros:
*   a) Puertos analógicos a registrar. El sistema deberá permitir la
*     selección independiente de cada canal a transmitir. Como mínimo 
*     tendrá que registrar un canal analógico
*     y como máximo 4 de forma simultanea.
*   b) El tiempo entre muestras. Este tiempo deberá
*     ser indicado en segundos.
*   c) La velocidad de comunicación del sistema. Esta opción deberá
*     permitir variar entre las velocidades 9600, 38400 y 115200 bit/s.
*     La velocidad por defecto deberá ser de 9600 bit/s.
*   d) Inicio y fin de transmisión de datos.
*Los datos deberán ser transmitidos separados por coma y el fin de la
*trama deberá ser indicado con un fin de linea.
*Con el informe deberán adjuntar la descripción del protocolo
*establecido para la configuración del dispositivo.

*Aclaraciones:
*
*Lo hice de forma que en cualquier momento se puede configurar.
*
*Protocolo elegido
*     S -> Para comenzar o dejar de transmitir los datos.
*     C -> Para cambiar la velocidad de comunicación.
*     A -> Para seleccionar qué canales mostrar o dejar de mostrar.
*         (Luego de seleccionar A se informa qué canales se están
*           mostrando o no para poder cambiar sus estados)
*     + -> Aumentar en 0,5 segundos el tiempo entre muestras.
*     - -> Disminuir en 0,5 segundos el tiempo entre muestras.
*     H -> Para volver a mostrar el menú.

************************************************************************
*/
/************************Definición de constantes***********************/
//Constantes para entradas y salidas
#define C1                  A0 //Los 4 canales
#define C2                  A1
#define C3                  A2
#define C4                  A3

//Constantes para el programa
#define LAPSO_MUESTREO     500 //Tiempo default de muestreo
#define LAPSO_MUESTREO_MIN 500 //Tiempo mínimo de muestreo

//(Debe ser múltiplo de 500 por el condicional en cambio_lapso_muestreo).

//Constantes por comodidad y costumbre
#define ON                HIGH
#define OFF                LOW
#define F                false
#define T                 true

/********************Declaración de variables globales******************/
bool transmicion = F; //Variable que indica el estado de la transmicion.
int lapso_muestreo = LAPSO_MUESTREO; //Valor del tiempo entre muestreos.

//Estado de los canales:
bool estadoC1 = T;
bool estadoC2 = T;
bool estadoC3 = T;
bool estadoC4 = T;

/***********************************************************************/
//Ciclo de configuración
void setup() {
  //Inicio la comunicación serie
  Serial.begin(9600);

  //Aviso al usario el menú de opciones.
  menu_texto();
}

//Bucle
void loop() {
  if (Serial.available())
    //Si hay un dato en el serial llamo a la función de lectura del menú
    menu(Serial.read());
  else
    //Si no, realizo el muestreo.
    muestreo();
}

/*******************************Funciones*******************************/

//Funcion para el menú.
//Básicamente según el protocolo llamo a la función correspondiente.
void menu(char dato){
  switch(dato){
    case 'S':
    case 's'://Duplico el case para abarcar minúsculas y mayúsculas.
      transmicion_fun();
      break;
    case 'C':
    case 'c':
      comunicacion_cambio();
      break;
    case 'A':
    case 'a':
      seleccion_canales_intro();
      break;
    case '+':
    case '-'://El cambio del lapso tiene código en 
    //común, por eso envío el dato y llamo a la misma función.
    //De esta manera me ahorro código.
      cambio_lapso_muestreo(dato);
      break;
    case 'H':
    case 'h':
      menu_texto();
      break;
    default:
      Serial.println("Ingrese H para volver a ver las opciones :) <3");
      break;
  }
}

//Funcion para mostrar el menú de opciones. O protocolo elegido.
void menu_texto(){
  Serial.println("*********************************Menu**********************************");
  Serial.println("*S -> Para comenzar o dejar de transmitir los datos.");
  Serial.println("*C -> Para cambiar la velocidad de comunicacion.");
  Serial.println("*A -> Para seleccionar que canales mostrar o dejar de mostrar.");
  Serial.println("*   (Luego de seleccionar A se informa que canales se estan");
  Serial.println("*   mostrando o no para poder cambiar sus estados)");
  Serial.println("*+ -> Aumentar en 0,5 segundos el tiempo entre muestras.");
  Serial.println("*- -> Disminuir en 0,5 segundos el tiempo entre muestras.");
  Serial.println("*H -> Para volver a mostrar el menu.");
  Serial.println("***********************************************************************");
}

//Funcion para comenzar o terminar la transmicion de datos.
void transmicion_fun(){
  if(transmicion) //Aviso el nuevo estado de la transmición
    Serial.println("La transmicion de datos ahora se encuentra apagada");
  else
    Serial.println("La transmicion de datos ahora se encuentra encendida");
  
  transmicion = !transmicion; //Invierto el estado
}

//Función para cambiar el valor del lapso de muestreo.
void cambio_lapso_muestreo(char dato){
  //Verifico si el dato enviado es un + o - y sumo o resto 500 al lapso.
  if(dato == '+')
    lapso_muestreo += 500;
  else
    //Como el lapso no puede tener un valor menor al mínimo verifico antes de restar.
    if(lapso_muestreo == LAPSO_MUESTREO_MIN){
      Serial.print("El minimo valor de muestro es ");
      Serial.print(LAPSO_MUESTREO_MIN);
      Serial.println(". :)");
      return;//Si no se puede restar termino la función en este punto.
    }
    else
      lapso_muestreo -= 500;
  //Luego paso el lapso en milisegundos a segundos.
  float lapsoEnSeg = lapso_muestreo/1000.0;
  //Y por último informo el valor actual.
  Serial.print("El lapso entre muestras ahora vale: ");
  Serial.print(lapsoEnSeg);
  Serial.println(" segundos.");
}

//Funcion para cambiar la velocidad de comunicacion
void comunicacion_cambio(){
  Serial.println("*****************Cambiar la velocidad de comunicacion*****************");
  Serial.println("1 -> Para cambiar la velocidad a   9600 bit/s");
  Serial.println("2 -> Para cambiar la velocidad a  38400 bit/s");
  Serial.println("3 -> Para cambiar la velocidad a 115200 bit/s");
  Serial.println("Ingrese otra cosa para volver al menú inicial");

  while(Serial.available() == 0); //Espero a recibir un dato en el Serial.

  char dato = Serial.read();
  switch(dato){ //Según el dato realizo el cambio correspondiente
    case '1':
      Serial.begin(9600);
      break;
    case '2':
      Serial.begin(38400);
      break;
    case '3':
      Serial.begin(115200);
      break;
  }
  Serial.println("Volviendo al menu");
  menu_texto();
}
//Funcion para la introducción a la selección de canales
void seleccion_canales_intro(){
  Serial.println("*************************Seleccion de canales**************************");
  Serial.println("*Los estados actuales son los siguientes:");
  Serial.print("*  El canal 1 se encuentra ");
  //Esto es para poder mostrar si está activo o no.
  Serial.println(conversion_bool_texto(estadoC1));
  Serial.print("*  El canal 2 se encuentra ");
  Serial.println(conversion_bool_texto(estadoC2));
  Serial.print("*  El canal 3 se encuentra ");
  Serial.println(conversion_bool_texto(estadoC3));
  Serial.print("*  El canal 4 se encuentra ");
  Serial.println(conversion_bool_texto(estadoC4));
  Serial.println("Ingrese el canal que desea seleccionar");
  Serial.println("(u otra cosa para volver al menu)");
  seleccion_canales();
}

//Funcion para la selección de canales
void seleccion_canales(){
  while(Serial.available() == 0);  //Espero a recibir un dato en el Serial.

  char dato = Serial.read();
  if((estadoC1+estadoC2+estadoC3+estadoC4) == 1){ //Si solo hay un canal activo
    if(verificarEstadosYDato(dato)){ //Verifico que no se pueda seleccionar el dato
      Serial.print("No se puede desactivar este canal ya que ");
      Serial.println("es el unico seleccionado.");
      Serial.println("Volviendo al menu");
      menu_texto();
      return; //En el caso de que no se pueda vuelvo al menú y salgo de la función.
    }
  }
  switch(dato){ //Como se puede seleccionar cualquier canal entonces invierto el estado.
    case '1':
      estadoC1  = !estadoC1;
      break;
    case '2':
      estadoC2  = !estadoC2;
      break;
    case '3':
      estadoC3  = !estadoC3;
      break;
    case '4':
      estadoC4  = !estadoC4;
      break;
    default:
      Serial.println("Volviendo al menu");
      menu_texto();
      return; //Si se ingresa otra cosa vuelvo al menú y salgo de la función.
  }
  seleccion_canales_intro(); //Muestro el estado actual de los canales.
}

//Funcion para convertir un booleano a un texto
String conversion_bool_texto (bool dato){
  if(dato)
    return("activo.");
  else
    return("inactivo.");
}

//Funcion de la verificacion de estados y dato
bool verificarEstadosYDato(char dato){
  //En este punto sé que hay un solo canal activo.
  //La finalidad de esta función es verificar que ese canal activo
  //no sea el mismo que el seleccionado por el usuario.
  //Si devuelve True significa que no se puede seleccionar ese canal.
  //(Y viceversa)

  switch(dato){
    case '1':
      if(estadoC1) //Si se seleccionó el 1 y está activo devuelvo True.
        return T;
      else
        return F; //Si no está activo devuelvo False.
    case '2':
      if(estadoC2)
        return T;
      else
        return F;
    case '3':
      if(estadoC3)
        return T;
      else
        return F;
    case '4':
      if(estadoC4)
         return T;
      else
         return F;
    //No utilizo break porque uso return.
    //¿Se puede simplificar más?
  }
}

//Funcion para el muestreo
void muestreo(){
  if(!transmicion) //Si la transmición está desactivada salgo de la función.
    return;
  //El tema de la lógica de las comas lo detallo en un .txt en este repo durante la semana.
  //En resúmen hice una tabla de verdad, verifiqué los casos donde va.
  //Dividí los casos en 2 tipos y llegué a la siguiente conclusión de uso.
  if(estadoC1) //Si el canal 1 está activo
    Serial.print(mapeoDeCanal(1)); //Muestro el valor en mV del canal.

  if(estadoC1 && estadoC2)
    Serial.print(",");

  if(estadoC2)
    Serial.print(mapeoDeCanal(2));

  if((estadoC1 || estadoC2) && estadoC3)
    Serial.print(",");

  if(estadoC3)
    Serial.print(mapeoDeCanal(3));
    
  if(!(((estadoC1+estadoC2+estadoC3+estadoC4) == 1) && estadoC4))
    Serial.print(",");

  if(estadoC4)
    Serial.print(mapeoDeCanal(4));

  Serial.println(); //Para poder cumplir el salto de línea
  delay(lapso_muestreo);
  //Sé que es bloqueante y está mal, pero todavía no vimos timers.
  //Asique no me parece tan mal.
}

//Funcion para mapear las entradas analógicas al valor en mV
int mapeoDeCanal(int dato){
  int valorDelSensor;
  switch(dato){
    case 1:
      valorDelSensor = analogRead(A0);
      break;
    case 2:
      valorDelSensor = analogRead(A1);
      break;
    case 3:
      valorDelSensor = analogRead(A2);
      break;
    case 4:
      valorDelSensor = analogRead(A3);
      break;
  }
  return map(valorDelSensor, 0, 1023, 0, 5000);
}