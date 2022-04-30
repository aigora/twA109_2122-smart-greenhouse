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
/*else if((valorLDR >= 423) & (valorLDR < 523))
{
digitalWrite(6, HIGH);
digitalWrite(5, LOW);
digitalWrite(3, LOW);

}
else if((valorLDR >= 323) & (valorLDR < 423))
{
digitalWrite(6, HIGH);
digitalWrite(5, HIGH);
digitalWrite(3, LOW);
}*/
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
