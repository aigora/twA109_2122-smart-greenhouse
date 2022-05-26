#include <Adafruit_Sensor.h>
#include <dht.h>
dht DHT;

#define DHTPIN 2    // Sensor temperatura: Se conecta el cable de SIG (Data output) al pin 2

enum Estados {MODO_ALARMA_ON,MODO_ALARMA_OFF};


// Definir rangos
    int tmin = 18;
    int tmax = 22;
    int hmin = 90;
    int hmax = 95;
    int lmin = 870;
    int lmax = 980;

// Definir modo inicial
    //Modo MANUAL:
      Estados estado=MODO_ALARMA_OFF;    // Empieza con modo manual por defecto
    //Modo AUTOMÁTICO:
    //Estado estado=MODO_ALARMA_ON;    // Empieza con modo automático por defecto

    
//Aquí almacenamos los datos recogidos del LDR (sensor luminosidad):
int luminosidad;
//Y en qué pin analógico conectarmos el LDR
int pinLDR = A0;
// Ventilador:
const int alt = 7;
const int cont = 8;
int speed = 200;

//Decimos qué pines vamos a utilizar para el LED RGB
const int red_light_pin= 6;
const int green_light_pin = 5;
const int blue_light_pin = 3;
const int red_light_pin2= 9;
const int green_light_pin2 = 10;
const int blue_light_pin2 = 11;
const int orange_light_pin= 12;
String mensaje_entrada;
String mensaje_salida;
int temperatura;
int humedad;
int temp_media=0;
int hum_media=0;
int lum_media=0;
int i;



void setup()
{
   Serial.begin(9600);    //inicializamos monitor serie para visualizar valores de LDR
   //Establecemos como salida los pines para LED
   pinMode(red_light_pin, OUTPUT);
   pinMode(green_light_pin, OUTPUT);
   pinMode(blue_light_pin, OUTPUT);

   pinMode(red_light_pin2, OUTPUT);
   pinMode(green_light_pin2, OUTPUT);
   pinMode(blue_light_pin2, OUTPUT);
   
   pinMode(orange_light_pin, OUTPUT);

   pinMode(alt,OUTPUT);     // Pin ventilador hacia un sentido
   pinMode(cont,OUTPUT);    // Pin ventilador hacia el sentido contrario

   pinMode(LED_BUILTIN, OUTPUT);      // Relé pin 13

   digitalWrite(red_light_pin,HIGH);
   digitalWrite(green_light_pin,HIGH);
   digitalWrite(blue_light_pin,HIGH); //Apaga led 1

   digitalWrite(red_light_pin2,HIGH);
   digitalWrite(green_light_pin2,HIGH);
   digitalWrite(blue_light_pin2,HIGH); //Apaga led

   digitalWrite(alt,LOW);
   digitalWrite(cont,LOW); //Apaga led

   digitalWrite(orange_light_pin,HIGH); //Apaga led
   DHT.read11(DHTPIN);
}

void loop ()
{
  procesar_mensajes1();
  alarma(tmin,tmax,hmin,hmax,lmin,lmax);
}


int medir_temperatura(void)     // Medición de temperatura
 {
  temperatura = DHT.temperature;
  return temperatura;
}

int medir_humedad(void)         // Medición de humedad
{
  humedad = DHT.humidity;
  return humedad;
}

int medir_luminosidad(void)         // Medición de luminosidad
{
  luminosidad = analogRead(pinLDR);
  return luminosidad;
}


void procesar_mensajes1()
{
  if( Serial.available()> 0) 
  {
    mensaje_entrada = Serial.readStringUntil('\n');
    if (mensaje_entrada.compareTo("GET_TEMPERATURA")==0)
    {
      temperatura = medir_temperatura();
      mensaje_salida=String(temperatura,3);
    }
    if (mensaje_entrada.compareTo("GET_HUMEDAD")==0)
    {
      humedad = medir_humedad();
      mensaje_salida=String(humedad,3);
    }
    if (mensaje_entrada.compareTo("GET_LUMINOSIDAD")==0)
    {
      luminosidad = medir_luminosidad();    
      mensaje_salida=String(luminosidad,3);
    }
    if (mensaje_entrada.compareTo("SET_MODO_ALARMA")==0)  // Desactivar alarma
      {
        if (estado==MODO_ALARMA_OFF)
        {
            estado=MODO_ALARMA_ON;
            mensaje_salida=String("MODO AUTOMATICO ACTIVADO");
        }
        else
        {
          estado=MODO_ALARMA_OFF;
          mensaje_salida=String("MODO MANUAL ACTIVADO");
         }
      }
    Serial.println(mensaje_salida);
  }
}


void alarma(int tmin, int tmax, int hmin, int hmax, int lmin, int lmax)
{
    // Sensor DHT11 y LDR
    temperatura =  medir_temperatura();    // Cada segundo, temperatura se va actualizando
    humedad = medir_humedad();
    luminosidad = medir_luminosidad(); 
    
    switch (estado)
    {
      case MODO_ALARMA_ON:  //temperatura_minima, temperatura_maxima,int modo ;  modo = 1 -> actuadores activados
          modo_alarma_temp(temperatura,tmin,tmax,1);
          modo_alarma_hum(humedad,hmin,hmax,1);
          modo_alarma_lum(luminosidad,lmin,lmax,1);
          break;
      case MODO_ALARMA_OFF:
          modo_alarma_temp(temperatura,tmin,tmax,0);
          modo_alarma_hum(humedad,hmin,hmax,0);
          modo_alarma_lum(luminosidad,lmin,lmax,0);
          break;
    }
}


void modo_alarma_temp(int temperatura, int temperatura_minima, int temperatura_maxima,int modo)
{
  int i,rango; // rango=0 -> Dentro rango

  rango = (temperatura >= temperatura_minima && temperatura <= temperatura_maxima) ? 0:1;  // 0 = temp dentro rango, 1 = temp fuera rango
  if (modo==0)    // Modo manual. Solo se encenderá el led rojo para avisar si se sale fuera de rango
  {
    if(rango==0){
      digitalWrite(red_light_pin,HIGH);   // Led rojo apagado
      analogWrite(cont,LOW);
      analogWrite(alt,LOW);     // Ventilador apagado
    }
    else{
      digitalWrite(red_light_pin,LOW);    // Led rojo encendido
      analogWrite(cont,LOW);
      analogWrite(alt,LOW);     // Ventilador apagado
    }
  }
  else          // Modo automático. Se enciende el led rojo y se acciona el ventilador
  {
    if(rango==0){
      digitalWrite(red_light_pin,HIGH);       // Led rojo apagado
      analogWrite(cont,LOW);
      analogWrite(alt,LOW);       //Ventilador apagado
    } 
    else{
      digitalWrite(red_light_pin,LOW);      // Led rojo encendido
      if(temperatura > temperatura_maxima){
          analogWrite(cont,LOW);
          analogWrite(alt,speed);     // Se activa el ventilador
      }     
    }
  }
}


void modo_alarma_hum(int humedad, int hum_minima, int hum_maxima,int modo)
{
  int i,rango; // rango=0 -> Dentro rango

  rango = (humedad >= hum_minima && humedad <=hum_maxima) ? 0:1;  // 0 = humedad dentro rango, 1 = humedad fuera rango
  if (modo==0)    // Modo manual. Solo se encenderá el led verde para avisar si se sale fuera de rango
  {
    if(rango==0){
      digitalWrite(green_light_pin2,HIGH);
      digitalWrite(LED_BUILTIN, LOW);   // Relé apagado
    }
    else{
      digitalWrite(green_light_pin2,LOW);   // Se enciende led 2 verde
      digitalWrite(LED_BUILTIN, LOW);   // Relé apagado
    }
  }
  else    // Modo automático. Se encenderá led verde y bomba de agua
  {
    if(rango == 0){
      digitalWrite(green_light_pin2,HIGH);
      digitalWrite(LED_BUILTIN, LOW);   // Desactiva la bomba de agua
    }
    else{
      digitalWrite(green_light_pin2,LOW);
      if(humedad < hum_minima)      // Solo si hay poca humedad
        digitalWrite(LED_BUILTIN, HIGH);   // Activa la bomba de agua
    }
  }
}

void modo_alarma_lum(int lum_media, int lum_minima, int lum_maxima,int modo)
{
  int i,rango; // rango=0 -> Dentro rango
  rango = (lum_media >=lum_minima && lum_media <=lum_maxima) ? 0:1;  // 0 = lum dentro rango, 1 = lum fuera rango
  if (modo==0)        // Modo manual. Se encenderá el led naranja para avisar si se sale fuera de rango
  {
    if(rango==0){
      digitalWrite(orange_light_pin,HIGH);
    }
    else
      digitalWrite(orange_light_pin,LOW);   // Se enciende el diodo
  }
  else
  {
    if(rango == 0)
      digitalWrite(orange_light_pin,HIGH);
    else
      digitalWrite(orange_light_pin,LOW);     // Simulación de un foco más potente
  }
}
