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
