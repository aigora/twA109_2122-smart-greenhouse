enum Estados {MODO_ALARMA_ON,MODO_ALARMA_OFF};
Estados estado=MODO_ALARMA_OFF;
const int LedPin = 4;
const int EchoPin = 5;
const int TriggerPin = 6;
String mensaje_entrada;
String mensaje_salida;

void setup()
{
   Serial.begin(9600);
   pinMode(TriggerPin, OUTPUT);
   pinMode(EchoPin, INPUT);
   pinMode(LedPin, OUTPUT);
}

void loop ()
{
  procesar_mensajes();
  switch (estado)
  {
    case MODO_ALARMA_ON:
        modo_alarma(5.0,15.0,2000,3000,1);
        break;
    case MODO_ALARMA_OFF:
        modo_alarma(5.0,15.0,2000,3000,0);
        break;
  }
}

void procesar_mensajes(void)
{
  if( Serial.available()> 0) 
  {
    mensaje_entrada = Serial.readStringUntil('\n');
    if (mensaje_entrada.compareTo("GET_TEMPERATURA")==0)
    {
      float grados = ping(TriggerPin, EchoPin);
      mensaje_salida=String("TEMPERATURA="+String(grados ,3));
    }
    else
      if (mensaje_entrada.compareTo("SET_MODO_ALARMA")==0)
      {
        if (estado==MODO_ALARMA_OFF)
        {
            estado=MODO_ALARMA_ON;
            mensaje_salida=String("ALARMA_OPERATIVA");
        }
        else
        {
          estado=MODO_ALARMA_OFF;
          mensaje_salida=String("ALARMA_INACTIVA");
         }
      }
      else
        mensaje_salida="COMANDO DESCONOCIDO";

    Serial.println(mensaje_salida);
  }
}

float ping(int TriggerPin, int EchoPin)
 {
   long duration, temperaturagrados;
   
   digitalWrite(TriggerPin, LOW);  //para generar un pulso limpio ponemos a LOW 4us
   delayMicroseconds(4);
   digitalWrite(TriggerPin, HIGH);  //generamos Trigger (disparo) de 10us
   delayMicroseconds(10);
   digitalWrite(TriggerPin, LOW);
   
   duration = pulseIn(EchoPin, HIGH);  //medimos el tiempo entre pulsos, en microsegundos
   
   temperaturagrados = duration * 10 / 292/ 2;   //convertimos a temperatura, en grados
   return temperaturagrados;
}


void modo_alarma(float temperatura_minima, float temperatura_maxima,int ms_fuera,int ms_led,int modo)
{
  static int  estado_alarma=0; // Estado actual
  static unsigned long  tiempo_alarma=0; // Ref. de tiempo alarma
  static unsigned long tiempo_led_on=0; // Ref. de tiempo led on
  float grados =0.0; // Temperatura captada
  int i,rango; 

  if (modo==0)
  {
    estado_alarma=0;      // Led está apagado
    digitalWrite(LedPin,LOW);
  }
  else
  {
    for (i=0;i<3;i++)  // La temperatura captada será la media aritmética de tres sondeos.
      grados += ping(TriggerPin, EchoPin);
    grados =grados /3;
    rango = (grados >=temperatura_minima && grados <=temperatura_maxima) ? 0:1;  // 0 = temp dentro rango, 1 = temp fuera rango
    switch (estado_alarma)
     {
       case 0: // Si detecta temperatura dentro del rango
         if (rango==1) // Si detecta una temperatura fuera del rango
         {
           tiempo_alarma=millis(); // Referencia temporal
           estado_alarma=1; // Cambia al estado 1     // Espera unos segundos para asegurarse de que la temperatura está fuera del rango
         }
         break;
       case 1: // Espera unos segundos para asegurarse de que la temperatura está fuera del rango
         if (millis()-tiempo_alarma>ms_fuera) // Si la temperatura se ha mantenido el tiempo mínimo requerido… la alarma cambia al estado dos
         {
           estado_alarma=2; // Cambio de estado... y se enciende el led
           digitalWrite(LedPin,HIGH); // Activa el led de alarma
           Serial.println("ATENCION: LA TEMPERATURA ESTÁ FUERA DEL RANGO PREESTABLECIDO"); // Mensaje de alarma
         }
         else
         if (rango==0)  // La temperatura no se ha mantenido el tiempo mínimo
           estado_alarma=0;  // Regresa al estado inicial
        break;
       case 2: // Alarma activa
         if (rango==0)  // Si la temperatura se mantiene dentro del rango
         {
           tiempo_led_on=millis(); // Inicia temporización para mantener el led 
           estado_alarma=3; // Cambio de estado
         }
         break;
       case 3: // Verifica si la temperatura está de nuevo dentro del rango
         if (millis()-tiempo_led_on>ms_led) // Si ha pasado el tiempo del led on
         {
           digitalWrite(LedPin,LOW); //Apaga led
           estado_alarma=0; //Regresa a estado inicial
           Serial.println("ATENCION:LA TEMPERATURA HA REGRESADO A SU RANGO PREDETERMINADO");           
         }
         else
          if (rango==1) // Si vuelve a detectar una temperatura fuera del rango
            estado_alarma=2; // Regresa a estado 2, alarma activada
     }
  }
}
