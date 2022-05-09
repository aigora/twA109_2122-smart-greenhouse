#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2    // Se conecta el cable de SIG (Data output) al pin 2
#define DHTTYPE    DHT11     // Nombre del sensor

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

enum Estados {MODO_ALARMA_ON,MODO_ALARMA_OFF};
Estados estado=MODO_ALARMA_OFF;
//Aquí almacenamos los datos recogidos del LDR (sensor luminosidad):
int luminosidad = 0;
//Y en qué pin analógico conectarmos el LDR
int pinLDR = A0;

//Decimos qué pines vamos a utilizar para el LED RGB
const int red_light_pin= 6;
const int green_light_pin = 5;
const int blue_light_pin = 3;
String mensaje_entrada;
String mensaje_salida;
float temperatura;
float humedad;

void setup()
{
   Serial.begin(9600);    //inicializamos monitor serie para visualizar valores de LDR
   //Establecemos como salida los pines para LED
   pinMode(red_light_pin, OUTPUT);
   pinMode(green_light_pin, OUTPUT);
   pinMode(blue_light_pin, OUTPUT);

   digitalWrite(red_light_pin,HIGH);
   digitalWrite(green_light_pin,HIGH);
   digitalWrite(blue_light_pin,HIGH); //Apaga led
   // Sensor DHT-11 temperatura-humedad
     // Inicializamos sensor
  dht.begin();
  // Imprime los detalles del sensor
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("\nSensor de temperatura"));
  Serial.print  (F("Nombre del sensor: ")); Serial.println(sensor.name);
  Serial.print  (F("Valor máximo:   ")); Serial.print(sensor.max_value); Serial.println(F("°C"));
  Serial.print  (F("Valor mínimo:   ")); Serial.print(sensor.min_value); Serial.println(F("°C"));
  Serial.print  (F("Error de precisión:  ")); Serial.print(sensor.resolution); Serial.println(F("°C"));
  Serial.println(F("------------------------------------"));

  dht.humidity().getSensor(&sensor);
  Serial.println(F("Sensor de humedad"));
  Serial.print  (F("Nombre del sensor: ")); Serial.println(sensor.name);
  Serial.print  (F("Valor máximo:   ")); Serial.print(sensor.max_value); Serial.println(F("%"));
  Serial.print  (F("Valor mínimo:   ")); Serial.print(sensor.min_value); Serial.println(F("%"));
  Serial.print  (F("Error de precisión:  ")); Serial.print(sensor.resolution); Serial.println(F("%"));
  Serial.println(F("------------------------------------"));
  // Para un segunda tras mostrar la información
  delayMS = sensor.min_delay / 1000;
   
}

void loop ()
{
  // Sensor DHT11
  temperatura =  medir_temperatura();    // Cada segundo, temperatura se va actualizando
  humedad = medir_humedad();

  
  // Sensor de luminosidad
  //Guardamos el valor leido en una variable
  //IMPORTANTE UTILIZAR EL SERIAL PARA VER LOS VALORES Y COMPROBAR QUE RECIBE VALORES BUENOS
  int poca_luz=1000;
  int mucha_luz=800;
  luminosidad = analogRead(pinLDR);
  Serial.print(F("Luminosidad: "));
  Serial.print(luminosidad);
  Serial.println(F("Lx\n"));    // Serial.println hace un \n al final

  
 
  //procesar_mensajes();
  estado=  MODO_ALARMA_ON;
  switch (estado)
  {
    case MODO_ALARMA_ON:  //temperatura_minima, temperatura_maxima,int ms_fuera,int ms_led,int modo ;  modo = 1 -> alarma activa
        modo_alarma_temp(temperatura,18.0,21.0,1);
        modo_alarma_hum(humedad,40.0,50.0,1);
        modo_alarma_lum(luminosidad,900,1000,1);
        break;
    case MODO_ALARMA_OFF:
        modo_alarma_temp(temperatura,18.0,21.0,0);
        modo_alarma_hum(humedad,40.0,50.0,0);
        modo_alarma_lum(luminosidad,900,1000,0);
        break;
  }
}


float medir_temperatura(void)     // Medición de temperatura
 {
   // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error al leer la temperatura"));
  }
  else {
    Serial.print(F("Temperatura: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
  }
  return event.temperature;
}

float medir_humedad(void)
{
    //delay(delayMS);
    // Get humidity event and print its value.
      sensors_event_t event;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error al leer la humedad"));
  }
  else {
    Serial.print(F("Humedad: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
    return event.relative_humidity;
}


void procesar_mensajes(void)
{
  if( Serial.available()> 0) 
  {
    mensaje_entrada = Serial.readStringUntil('\n');
    if (mensaje_entrada.compareTo("GET_TEMPERATURA")==0)
    {
      float temperatura = medir_temperatura();
      mensaje_salida=String("TEMPERATURA="+String(temperatura ,3));
    }
    else
      if (mensaje_entrada.compareTo("SET_MODO_ALARMA")==0)  // Desactivar alarma
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




void modo_alarma_temp(float temperatura, float temperatura_minima, float temperatura_maxima,int modo) // ms_fuera = tiempo en alerta por temp fuera del rango; ms_led que esta encendido
{
  static int  estado_alarma=0; // Estado actual
  //static unsigned long  tiempo_alarma=0; // Ref. de tiempo alarma
  //static unsigned long tiempo_led_on=0; // Ref. de tiempo led on
  //float temperatura =0.0; // Temperatura captada
  int i,rango; // rango=0 -> Dentro rango

  if (modo==0)
  {
    estado_alarma=0;      // Led está apagado
    digitalWrite(red_light_pin,HIGH);
    digitalWrite(green_light_pin,HIGH);
    digitalWrite(blue_light_pin,HIGH);
  }
  else
  {
    rango = (temperatura >=temperatura_minima && temperatura <=temperatura_maxima) ? 0:1;  // 0 = temp dentro rango, 1 = temp fuera rango
    switch (estado_alarma)
     {
       case 0: // Si detecta temperatura dentro del rango
          if(rango==0)
            digitalWrite(red_light_pin,HIGH);
            digitalWrite(green_light_pin,HIGH);
            digitalWrite(blue_light_pin,HIGH);
         if (rango==1) // Si detecta una temperatura fuera del rango
         {
           //tiempo_alarma=millis(); // Referencia temporal
           estado_alarma=1; // Cambia al estado 1     // Espera unos segundos para asegurarse de que la temperatura está fuera del rango
         }
         break;
       case 1: // Espera unos segundos para asegurarse de que la temperatura está fuera del rango
          if (rango == 1){
            estado_alarma=2; // Cambio de estado... y se enciende el led de color rojo
            for(i=0;i<6;i++){
               digitalWrite(red_light_pin,LOW);
               delay(500);
               digitalWrite(red_light_pin,HIGH);
               delay(300);
           Serial.println("ATENCION: LA TEMPERATURA ESTÁ FUERA DEL RANGO PREESTABLECIDO"); // Mensaje de alarma
            }
            Serial.print("\n");
          }
         if (rango==0)  // La temperatura no se ha mantenido el tiempo mínimo
           estado_alarma=0;  // Regresa al estado inicial
        break;
       case 2: // Alarma activa
         if (rango==0)  // Si la temperatura se mantiene dentro del rango
         {
           //tiempo_led_on=millis(); // Inicia temporización para mantener el led 
           digitalWrite(red_light_pin,HIGH);
           digitalWrite(green_light_pin,HIGH);
           digitalWrite(blue_light_pin,HIGH); //Apaga led
           estado_alarma=0; //Regresa a estado inicial
           Serial.println("ATENCION:LA TEMPERATURA HA REGRESADO A SU RANGO PREDETERMINADO");   
         }
         else
          estado_alarma = 1;
         break;
       /*case 3: // Verifica si la temperatura está de nuevo dentro del rango
         if (millis()-tiempo_led_on>ms_led) // Si ha pasado el tiempo del led on
         {
          
           digitalWrite(red_light_pin,HIGH);
           digitalWrite(green_light_pin,HIGH);
           digitalWrite(blue_light_pin,HIGH); //Apaga led
           estado_alarma=0; //Regresa a estado inicial
           Serial.println("ATENCION:LA TEMPERATURA HA REGRESADO A SU RANGO PREDETERMINADO");           
         }
         else
          if (rango==1) // Si vuelve a detectar una temperatura fuera del rango
            estado_alarma=2; // Regresa a estado 2, alarma activada*/
     }
  }
}


void modo_alarma_hum(float humedad, float hum_minima, float hum_maxima,int modo) // ms_fuera = tiempo en alerta por temp fuera del rango; ms_led que esta encendido
{
  static int  estado_alarma=0; // Estado actual
  //static unsigned long  tiempo_alarma=0; // Ref. de tiempo alarma
  //static unsigned long tiempo_led_on=0; // Ref. de tiempo led on
  //float humedad =0.0; // Temperatura captada
  int i,rango; // rango=0 -> Dentro rango

  if (modo==0)
  {
    estado_alarma=0;      // Led está apagado
    /*digitalWrite(red_light_pin,HIGH);
    digitalWrite(green_light_pin,HIGH);
    digitalWrite(blue_light_pin,HIGH);*/
  }
  else
  {
    rango = (humedad >=hum_minima && humedad <=hum_maxima) ? 0:1;  // 0 = temp dentro rango, 1 = temp fuera rango
    switch (estado_alarma)
     {
       case 0: // Si detecta temperatura dentro del rango
          if(rango==0)
            digitalWrite(green_light_pin,HIGH);
         if (rango==1) // Si detecta una temperatura fuera del rango
         {
           //tiempo_alarma=millis(); // Referencia temporal
           estado_alarma=1; // Cambia al estado 1     // Espera unos segundos para asegurarse de que la temperatura está fuera del rango
         }
         break;
       case 1: // Espera unos segundos para asegurarse de que la temperatura está fuera del rango
          if (rango == 1){
            estado_alarma=2; // Cambio de estado... y se enciende el led de color rojo
            for(i=0;i<6;i++){
               digitalWrite(green_light_pin,LOW);
               delay(500);
               digitalWrite(green_light_pin,HIGH);
               delay(300);
           Serial.println("ATENCION: LA HUMEDAD ESTÁ FUERA DEL RANGO PREESTABLECIDO"); // Mensaje de alarma
            }
            Serial.print("\n");
          }
         if (rango==0)  // La temperatura no se ha mantenido el tiempo mínimo
           estado_alarma=0;  // Regresa al estado inicial
        break;
       case 2: // Alarma activa
         if (rango==0)  // Si la temperatura se mantiene dentro del rango
         {
           //tiempo_led_on=millis(); // Inicia temporización para mantener el led 
           digitalWrite(red_light_pin,HIGH);
           digitalWrite(green_light_pin,HIGH);
           digitalWrite(blue_light_pin,HIGH); //Apaga led
           estado_alarma=0; //Regresa a estado inicial
           Serial.println("ATENCION:LA HUMEDAD HA REGRESADO A SU RANGO PREDETERMINADO");   
         }
         else
          estado_alarma = 1;
         break;
       /*case 3: // Verifica si la temperatura está de nuevo dentro del rango
         if (millis()-tiempo_led_on>ms_led) // Si ha pasado el tiempo del led on
         {
          
           digitalWrite(red_light_pin,HIGH);
           digitalWrite(green_light_pin,HIGH);
           digitalWrite(blue_light_pin,HIGH); //Apaga led
           estado_alarma=0; //Regresa a estado inicial
           Serial.println("ATENCION:LA TEMPERATURA HA REGRESADO A SU RANGO PREDETERMINADO");           
         }
         else
          if (rango==1) // Si vuelve a detectar una temperatura fuera del rango
            estado_alarma=2; // Regresa a estado 2, alarma activada*/
     }
  }
}

void modo_alarma_lum(int luminosidad, int lum_minima, int lum_maxima,int modo) // ms_fuera = tiempo en alerta por temp fuera del rango; ms_led que esta encendido
{
  static int  estado_alarma=0; // Estado actual
  //static unsigned long  tiempo_alarma=0; // Ref. de tiempo alarma
  //static unsigned long tiempo_led_on=0; // Ref. de tiempo led on
  //float luminosidad =0.0; // Temperatura captada
  int i,rango; // rango=0 -> Dentro rango

  if (modo==0)
  {
    estado_alarma=0;      // Led está apagado
    /*digitalWrite(red_light_pin,HIGH);
    digitalWrite(green_light_pin,HIGH);
    digitalWrite(blue_light_pin,HIGH);*/
  }
  else
  {
    rango = (luminosidad >=lum_minima && luminosidad <=lum_maxima) ? 0:1;  // 0 = temp dentro rango, 1 = temp fuera rango
    switch (estado_alarma)
     {
       case 0: // Si detecta temperatura dentro del rango
          if(rango==0)
            digitalWrite(blue_light_pin,HIGH);
         if (rango==1) // Si detecta una temperatura fuera del rango
         {
           //tiempo_alarma=millis(); // Referencia temporal
           estado_alarma=1; // Cambia al estado 1     // Espera unos segundos para asegurarse de que la temperatura está fuera del rango
         }
         break;
       case 1: // Espera unos segundos para asegurarse de que la temperatura está fuera del rango
          if (rango == 1){
            estado_alarma=2; // Cambio de estado... y se enciende el led de color rojo
            for(i=0;i<6;i++){
               digitalWrite(blue_light_pin,LOW);
               delay(500);
               digitalWrite(blue_light_pin,HIGH);
               delay(300);
           Serial.println("ATENCION: LA LUMINOSIDAD ESTÁ FUERA DEL RANGO PREESTABLECIDO"); // Mensaje de alarma
            }
            Serial.print("\n");
         }
         if (rango==0)  // La temperatura no se ha mantenido el tiempo mínimo
           estado_alarma=0;  // Regresa al estado inicial
        break;
       case 2: // Alarma activa
         if (rango==0)  // Si la temperatura se mantiene dentro del rango
         {
           //tiempo_led_on=millis(); // Inicia temporización para mantener el led 
           digitalWrite(red_light_pin,HIGH);
           digitalWrite(green_light_pin,HIGH);
           digitalWrite(blue_light_pin,HIGH); //Apaga led
           estado_alarma=0; //Regresa a estado inicial
           Serial.println("ATENCION:LA LUMINOSIDAD HA REGRESADO A SU RANGO PREDETERMINADO");   
         }
         else
          estado_alarma = 1;
         break;
       /*case 3: // Verifica si la temperatura está de nuevo dentro del rango
         if (millis()-tiempo_led_on>ms_led) // Si ha pasado el tiempo del led on
         {
          
           digitalWrite(red_light_pin,HIGH);
           digitalWrite(green_light_pin,HIGH);
           digitalWrite(blue_light_pin,HIGH); //Apaga led
           estado_alarma=0; //Regresa a estado inicial
           Serial.println("ATENCION:LA TEMPERATURA HA REGRESADO A SU RANGO PREDETERMINADO");           
         }
         else
          if (rango==1) // Si vuelve a detectar una temperatura fuera del rango
            estado_alarma=2; // Regresa a estado 2, alarma activada*/
     }
  }
}
