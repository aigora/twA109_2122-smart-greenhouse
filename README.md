# Sistema automático de control de invernadero

Consiste en un pequeño invernadero capaz de medir distintos factores de su entorno y, en función de los valores que se obtengan en cada una de las situaciones, actuará de una manera u otra. Estos elementos son, concretamente: luminosidad, humedad y temperatura.

## Integrantes del equipo

Carlos Osuna Cabezudo: @Osuuunaa
<br /> 	Pablo Muñoz Moreno: @Pablomunozmoreno
<br /> Javier Rodríguez Jiménez @javierrjimenez

## Objetivos del trabajo

El objetivo fundamental del proyecto es la construcción de un sistema que permita cuidar una planta, así como monitorizar todas las variables que influyen en su crecimiento y en su correcto desarrollo mediante una serie de sensores y actuadores que funcionan de manera autónoma en base a nuestros requerimientos. A grandes rasgos, el proyecto constará de unos sensores de temperatura, humedad y luminosidad. Además contará con una serie de actuadores, ya sea una pequeña bomba de agua o un sistema de irrigación, que permitan el riego automático, varios ventiladores que faciliten la disminución de la temperatura y un relé capaz de controlar luces que emulen el ciclo de luz-oscuridad que necesita una planta. A su vez el programa realizará informes de las condiciones y del nivel del tanque de agua que alimenta el sistema de riego.

![IMG_20220526_175328](https://user-images.githubusercontent.com/100867734/170655672-4b2f350a-daaa-4602-8ce3-773bea957702.jpg)


## Actuadores
Ventilador DC 5V PWM - HALJIA L9110
<br /> Moto Bomba Sumergible de Agua - 108.MT0103 - Ref. MMBB001
<br /> Diodo LED 5 mm - Cátodo común - Clear RGB 35° - Ref. SMDL5C0A   (Simulación de luz solar)

## Sensores
Módulo sensor humedad y temperatura DHT11 - 101020011 - Ref. SSHU004
<br /> Resistencia LDR 5 x 4 mm - Sensor de Luz - Ref. SSLDR34  (Sensor de luminosidad)

## Microcontroladores
Módulo Arduino Uno Rev03 Compatible con cable USB - Ref. LCA1201
<br /> Seeed Studio - Módulo Relé 5 V - Plug and Play - 103020005 - Ref. LCYLRL5    (Acciona bomba de agua)
<br /> Módulo Board Experimental 830 Contactos - Ref. CN1A003
<br /> 8x Varta LR06 - Pila Alcalina 1,5 V AA - Ref. BA51680    (Fuente de alimentación)
<br /> Portapilas 8 Pilas AA Cable - Ref. BA71677
<br /> 10x Resistencia Película de Carbón ¼ W - 330K Ohmios 5% - CR25330K - Ref. RS2A25B330K
<br /> Diodo Led 5mm Difuso Naranja - Ref. SMDL5DN    (Simula luz solar)

## Pruebas de sensores y actuadores:
### Sensor de luminosidad fotorresistor LDR
```
//Aquí almacenamos los datos recogidos del LDR:
int valorLDR = 0;

//Decimos que pines vamos a utilizar para LED
int red_light_pin= 6;
int green_light_pin = 5;
int blue_light_pin = 3;

//Y que pin analógico conectarmos el LDR
int pinLDR = A0;

void setup()
{
//Establecemos como salida los pines para LED
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

//inicializamos monitor serie para visualizar valores de LDR
Serial.begin(9600);

}

void loop()
{
//Guardamos el valor leido en una variable
//IMPORTANTE UTILIZAR EL SERIAL PARA VER LOS VALORES Y COMPROBAR QUE RECIBE VALORES BUENOS

valorLDR = analogRead(pinLDR);
Serial.println(valorLDR);

//Y comenzamos las comparaciones:
if(valorLDR >= 1000)   // POCA LUZ
{
digitalWrite(6, HIGH);
digitalWrite(5, LOW);
digitalWrite(3, HIGH);
}
else if((valorLDR >= 800) & (valorLDR < 1000))    // LUZ INTERMEDIA
{
digitalWrite(6, HIGH);
digitalWrite(5, HIGH);
digitalWrite(3, LOW);
}
else if(valorLDR < 800)       // MUCHA LUZ
{
digitalWrite(6, LOW);
digitalWrite(5, HIGH);
digitalWrite(3, HIGH);
}
}
```

### Sensor de temperatura y humedad DHT-11
```
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2    // Se conecta el cable de SIG (Data output) al pin 2
#define DHTTYPE    DHT11     // Nombre del sensor

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

void setup() {
  Serial.begin(9600);
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

void loop() {
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
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error al leer la humedad"));
  }
  else {
    Serial.print(F("Humedad: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }
}
```
### Ventilador
```
const int alt = 7;
const int cont = 8;
int speed = 200;

void setup(){
  pinMode(alt,OUTPUT);
  pinMode(cont,OUTPUT);
}

void loop(){
  analogWrite(cont,LOW);
  analogWrite(alt,speed);
  delay(2000);
  analogWrite(cont,LOW);
  analogWrite(alt,LOW);
  delay(2000);
}
```
### Bomba de agua por acción de un relé
```
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(2000);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(3000);                       // wait for a second
}
```
