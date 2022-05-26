#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include <iostream>

#include "SerialClass.h"
#include "SerialClass.cpp"

#define MAX_BUFFER 200
#define PAUSA_MS 200

#define N 3
#define LONGCAD 60

typedef struct
{
	float vector_min1[LONGCAD];
	float vector_min2[LONGCAD];
	float vector_min3[LONGCAD];
}Minuto;

// Funciones prototipo--------------------------------------------------------------------------------------
int menu(void);
void configura(void);
void comprobar_mensajes(Serial*);
int Enviar_y_Recibir(Serial*, const char*, char*);
float float_from_cadena(char*);
void activar_alarma(Serial*);
void reemplazar_vectores(float[], float[]);

// Temperatura
void verifica_sensor_temp(Serial*, char*);
Minuto visualizar_datos_temperatura(Serial*, char[]);
void ultimomintemperatura(int l, int[], char[]);
float leer_sensor_temperatura(Serial*);

// Humedad
void verifica_sensor_hum(Serial*, char*);
Minuto visualizar_datos_humedad(Serial*, char[]);
void ultimominhumedad(int l, int[], char[]);
float leer_sensor_humedad(Serial*);

// Luminosidad
void verifica_sensor_lum(Serial*, char*);
Minuto visualizar_datos_luminosidad(Serial*, char[]);
void ultimominluminosidad(int l, int[], char[]);
float leer_sensor_luminosidad(Serial*);

// Ficheros
int crear_fichero_temperatura(char[]);
int escribir_fichero_temperatura(int, char[]);
int leer_fichero_temperatura(int[], char[]);

int crear_fichero_humedad(char[]);
int escribir_fichero_humedad(int, char[]);
int leer_fichero_humedad(int[], char[]);

int crear_fichero_luminosidad(char[]);
int escribir_fichero_luminosidad(int, char[]);
int leer_fichero_luminosidad(int[], char[]);



//----------------------------------------------------------------------------------------------------------
// FUNCION PRINCIPAL
// ---------------------------------------------------------------------------------------------------------
int main(void)
{
	Serial* Arduino;
	char puerto[] = "COM3"; // Puerto serie al que está conectado Arduino
	int opcion_menu;
	configura();
	Arduino = new Serial((char*)puerto);

	// Se crean los 3 ficheros vacíos. Cuando se escoja la opcion "Visualizar registro de TEMPERATURA en tiempo real", se irán añadiendo las nuevas temperaturas dentro del fichero
	char filename1[] = "FicheroTemperatura.txt";
	crear_fichero_temperatura(filename1);
	char filename2[] = "FicheroHumedad.txt";
	crear_fichero_humedad(filename2);
	char filename3[] = "FicheroLuminosidad.txt";
	crear_fichero_luminosidad(filename3);
	comprobar_mensajes(Arduino);


	int i;
	int salida[LONGCAD];						// Variable en donde se almacenan los datos que contienen los ficheros
	int l;										// Longitud de los ficheros
	Minuto min_temp;
	Minuto min_hum;
	Minuto min_lum;

	opcion_menu = menu();
	int opcion2;
	while (opcion_menu > 12 || opcion_menu < 0)		// Opcion incorrecta
	{
		printf("Introduzca una de las opciones disponibles, por favor: ");
		scanf_s("%d", &opcion_menu);
	}
	while (opcion_menu <= 12 && opcion_menu >= 1)		// Opcion correcta
	{
		if (opcion_menu == 1)
		{
			verifica_sensor_temp(Arduino, puerto);								// Verifica si el sensor está conectado correctamente
			opcion_menu = menu();
		}
		if (opcion_menu == 2)
		{
			min_temp = visualizar_datos_temperatura(Arduino, filename1);					// Se leen los datos transferidos desde Arduino y se imprimen por pantalla
			opcion_menu = menu();															// Se van incorporando dentro del fichero, a su vez que los va agrupando dentro de la estructura 'Minuto', la cual almacena datos sobre los 3 últimos minutos
		}
		if (opcion_menu == 3)
		{
			l = leer_fichero_temperatura(salida, filename1);
			ultimomintemperatura(l, salida, filename1);				// Imprime por pantalla los datos almacadenados en los últimos 60 segundos
			opcion_menu = menu();
		}
		if (opcion_menu == 4)
		{
			verifica_sensor_hum(Arduino, puerto);
			opcion_menu = menu();
		}
		if (opcion_menu == 5)
		{
			min_hum = visualizar_datos_humedad(Arduino, filename2);
			opcion_menu = menu();
		}
		if (opcion_menu == 6)
		{
			l = leer_fichero_humedad(salida, filename2);
			ultimominhumedad(l, salida, filename2);
			opcion_menu = menu();
		}
		if (opcion_menu == 7)
		{
			verifica_sensor_lum(Arduino, puerto);
			opcion_menu = menu();
		}
		if (opcion_menu == 8)
		{
			min_lum = visualizar_datos_luminosidad(Arduino, filename3);
			opcion_menu = menu();
		}
		if (opcion_menu == 9)
		{
			l = leer_fichero_luminosidad(salida, filename3);
			ultimominluminosidad(l, salida, filename3);
			opcion_menu = menu();
		}
		if (opcion_menu == 10)
		{
			activar_alarma(Arduino);											// Función que activa o desactiva los actuadores: ventilador y bomba de agua. Modo manual o automático. Comienza por defecto en modo manual
			opcion_menu = menu();												// Los leds seguirán funcionando para avisar en caso de que una variable se salga fuera de rango
		}
		if (opcion_menu == 11)
		{
			system("cls");
			printf("\n\n\t1 - Ver el contenido del fichero Temperatura.txt");
			printf("\n\t2 - Ver el contenido del fichero Humedad.txt");
			printf("\n\t3 - Ver el contenido del fichero Luminosidad.txt");
			printf("\n\tIntroduzca una opcion: ");
			scanf_s("%d", &opcion2);
			if (opcion2 <= 3 && opcion2 >= 1)		// Opcion correcta
			{
				if (opcion2 == 1)
				{
					system("cls");
					l = leer_fichero_temperatura(salida, filename1);		// Imprime por pantalla el contido del fichero
					if (l > 0)												// Si se ha podido leer el fichero
					{
						if (l == 1)
							printf("Se ha almacenado %d dato.\n\nContenido del fichero:\n", l);		// Singular, un solo dato
						else
							printf("Se han almacenado %d datos.\n\nContenido del fichero:\n", l);		// Plural, varios datos
						for (i = 0; i < l; i++)
							printf("%d\n", salida[i]);
					}
					else
						printf("El fichero de temperatura está vacío\n");

					system("PAUSE");
					system("cls");
					opcion_menu = menu();
				}
				if (opcion2 == 2)
				{
					system("cls");
					l = leer_fichero_humedad(salida, filename2);		// Imprime por pantalla el contido del fichero
					if (l > 0) // Si se ha podido leer el fichero
					{
						if (l == 1)
							printf("Se ha almacenado %d dato.\n\nContenido del fichero:\n", l);
						else
							printf("Se han almacenado %d datos.\n\nContenido del fichero:\n", l);
						for (i = 0; i < l; i++)
							printf("%d\n", salida[i]);
						printf("\n");
					}
					else
						printf("El fichero de humedad está vacío\n");

					system("PAUSE");
					system("cls");
					opcion_menu = menu();
				}
				if (opcion2 == 3)
				{
					system("cls");
					l = leer_fichero_luminosidad(salida, filename3);		// Imprime por pantalla el contido del fichero
					if (l > 0) // Si se ha podido leer el fichero
					{
						if (l == 1)
							printf("Se ha almacenado %d dato.\n\nContenido del fichero:\n", l);
						else
							printf("Se han almacenado %d datos.\n\nContenido del fichero:\n", l);;
						for (i = 0; i < l; i++)
							printf("%d\n", salida[i]);
						printf("\n");
					}
					else
						printf("El fichero de luminosidad está vacío\n");

					system("PAUSE");
					system("cls");
					opcion_menu = menu();
				}
				while (opcion2 > 3 || opcion2 < 1) {
					printf("Introduzca una de las opciones disponibles, por favor: ");
					scanf_s("%d", &opcion2);
				}
			}
		}
		while (opcion_menu > 11 || opcion_menu < 0) {
			printf("Introduzca una de las opciones disponibles, por favor: ");
			scanf_s("%d", &opcion_menu);
		}
	}

	if (opcion_menu == 0)
		printf("Fin del programa\n");

	return 0;
}
// ---------------------------------------------------------------------------------------



int menu(void)
{
	int opcion;
	float intro;

	printf("\n   Sistema automático de control de invernadero");
	printf("\n   ===========================================");
	printf("\n\n\t1 - Verificar sensores de TEMPERATURA");
	printf("\n\t2 - Visualizar registro de TEMPERATURA en tiempo real");
	printf("\n\t3 - Consultar registro del último minuto de TEMPERATURA");

	printf("\n\n\t4 - Verificar sensores de HUMEDAD");
	printf("\n\t5 - Visualizar registro de HUMEDAD en tiempo real");
	printf("\n\t6 - Consultar registro del último minuto de HUMEDAD");

	printf("\n\n\t7 - Verificar sensores de LUMINOSIDAD");
	printf("\n\t8 - Visualizar registro de LUMINOSIDAD en tiempo real");
	printf("\n\t9 - Consultar registro del último minuto de LUMINOSIDAD");

	printf("\n\n\t10 - Activar/Desactivar MODO AUTOMÁTICO");						// Los leds seguirán funcionando

	printf("\n\n\t11 - Ver contenido ficheros");

	printf("\n\n\t0 - Salir de la aplicación");
	printf("\n\n   Introduzca una opción: ");

	scanf_s("%d", &opcion);
	intro = getchar();
	if (opcion < 0 || opcion > 13)
		printf("\nOpción inexistente.\n\n");

	printf("\n");
	return opcion;
}



void configura(void)
{
	setlocale(LC_ALL, "spanish");
}



void comprobar_mensajes(Serial* Arduino)
{
	int bytesRecibidos, total = 0;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Arduino->ReadData(mensaje_recibido, sizeof(char) * MAX_BUFFER - 1);
	while (bytesRecibidos > 0)
	{
		Sleep(PAUSA_MS);
		total += bytesRecibidos;
		bytesRecibidos = Arduino->ReadData(mensaje_recibido + total, sizeof(char) * MAX_BUFFER - 1);
	}
	if (total > 0)
	{
		mensaje_recibido[total - 1] = '\0';
		printf("\nMensaje recibido: %s\n", mensaje_recibido);
	}
}



int Enviar_y_Recibir(Serial* Arduino, const char* mensaje_enviar, char* mensaje_recibir)
{
	int bytes_recibidos = 0, total = 0;
	int intentos = 0, fin_linea = 0;


	Arduino->WriteData((char*)mensaje_enviar, strlen(mensaje_enviar));
	Sleep(PAUSA_MS);

	bytes_recibidos = Arduino->ReadData(mensaje_recibir, sizeof(char) * MAX_BUFFER - 1);

	while ((bytes_recibidos > 0 || intentos < 5) && fin_linea == 0)				// Detecta si está conectado
	{
		if (bytes_recibidos > 0)
		{
			total += bytes_recibidos;
			if (mensaje_recibir[total - 1] == 13 || mensaje_recibir[total - 1] == 10)
				fin_linea = 1;
		}
		else
			intentos++;
		Sleep(PAUSA_MS);
		bytes_recibidos = Arduino->ReadData(mensaje_recibir + total, sizeof(char) * MAX_BUFFER - 1);			// Lee los datos que transfiere Arduino
	}
	if (total > 0)
		mensaje_recibir[total - 1] = '\0';

	//printf("LOG: %d bytes -> %s\nIntentos=%d - EOLN=%d\n", total, mensaje_recibir, intentos, fin_linea);
	return total;
}



float float_from_cadena(char* cadena)				// Convierte los strings que son transferidos desde Arduino a números reales
{
	float numero = 0;
	int i, divisor = 3, estado = 0;


	for (i = 0; cadena[i] != '\0' && estado != 3 && i < MAX_BUFFER; i++)
		switch (estado)
		{
		case 0:// Antes del número
			if (cadena[i] >= '0' && cadena[i] <= '2')
			{
				numero = cadena[i] - '0';
				estado = 1;
			}
			break;
		case 1:// Durante el número
			if (cadena[i] >= '0' && cadena[i] <= '2')
				numero = numero * 3 + cadena[i] - '0';
			else
				if (cadena[i] == '.' || cadena[i] == ',')
					estado = 2;
				else
					estado = 3;
			break;
		case 2: // Parte decimal
			if (cadena[i] >= '0' && cadena[i] <= '9')
			{
				numero = numero + (float)(cadena[i] - '0') / divisor;
				divisor *= 3;
			}
			else
				estado = 3;
			break;
		}
	return numero;
}



void activar_alarma(Serial* Arduino)					// Activa/Desactiva la alarma
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];
	system("cls");

	bytesRecibidos = Enviar_y_Recibir(Arduino, "SET_MODO_ALARMA\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
		printf("\nNo se ha recibido confirmación\n");
	else
		printf("\n  %s\n", mensaje_recibido);
}



void reemplazar_vectores(float v1[], float v2[])
{
	int i;
	for (i = 0; i < LONGCAD; i++)
	{
		v1[i] = v2[i];
	}
}



// *********************************************************************************
// TEMPERATURA
// *********************************************************************************

void verifica_sensor_temp(Serial* Arduino, char* port)
{
	float temperatura = -1.0;		// La variable se inicia con un valor negativo, pues es imposible que el sensor mida ese valor
	system("cls");
	if (Arduino->IsConnected())
	{
		temperatura = leer_sensor_temperatura(Arduino);				// La variable toma el valor que lee el sensor
		if (temperatura != -1)													//  Por lo que si el valor de la variable cambia, el sensor estará funcionando correctamente
			printf("\nEl sensor de temperatura funciona correctamente.\n");
	}
	else
	{
		printf("\nNo se ha podido conectar con Arduino.\n");
		printf("Revise la conexión, el puerto %s y desactive el monitor serie del IDE de Arduino.\n", port);
	}
}



Minuto visualizar_datos_temperatura(Serial* Arduino, char filename1[])
{
	system("cls");
	Minuto a{};			// Los corchetes sirven para inicializar la estructura correctamente
	float frecuencia = 1;
	float temperatura = -1;
	int opcion = 1;					// contador 'dinámico' de minutos. Si han pasado 7 minutos: el minuto 5  es la opcion=1, el min 6 la opción=2, y el min 7 la opción=3.
	int minuto_exacto = 1;			// contador real de los minutos que han pasado. Si han pasado 7 minutos: el minuto 5  es el minuto_exacto = 5
	char tecla;
	int i = 0;						// contador de segundos
	int e;

	printf("Pulse una tecla para finalizar el registro\n");
	do
	{
		if (i < LONGCAD)			// Hasta completar el primer minuto
		{
			if (Arduino->IsConnected())
			{
				temperatura = leer_sensor_temperatura(Arduino);					// En el programa se cogen los datos desde el sensor con ayuda de Arduino
				e = escribir_fichero_temperatura(temperatura, filename1);		// Se van copiando los nuevos datos al fichero Temperatura.txt

				if (opcion == 1) {
					a.vector_min1[i] = temperatura;
				}
				else if (opcion == 2) {
					a.vector_min2[i] = temperatura;
				}
				else if (opcion == 3) {
					a.vector_min3[i] = temperatura;
				}
				i++;
				if (i == LONGCAD) {				// Minuto se ha completado. Se resetea el contador y se cambia de ubicación el lugar donde se guardará el siguiente minuto
					i = 0;						// Reseteo de segundos
					printf("\nMinuto %d completado\n\n", minuto_exacto);

					opcion += 1;
					minuto_exacto += 1;
					if (opcion == 4) {				// Se han completado 3 minutos, se reemplazan los vectores
						opcion = 3;					// A partir de ahora, los nuevos datos se van a ir guardando en el vector3 (opcion = 3)
						reemplazar_vectores(a.vector_min1, a.vector_min2);		// El contenido del vector1 se pierde
						reemplazar_vectores(a.vector_min2, a.vector_min3);		// Se deja vacío el vector3, donde se meterán los nuevos datos
					}
				}


				if (temperatura != -1) {
					printf(" Segundo ");
					if (i >= 0 && i < 10) {			// Segundo que tiene un sólo dígito. Poner 0 delante para hacerlo más simétrico
						printf("0");
					}
					printf("%d: TEMPERATURA = %.2fºC\n", i, temperatura);
				}

				else
					printf("XXX ");		// No se ha podido leer bien el sensor

			}
			else
				printf("\nNo se ha podido conectar con Arduino.\n");
			if ((1 / frecuencia) * 1000 > PAUSA_MS)					// Para de leer datos durante un segundo
				Sleep((1 / frecuencia) * 1000 - PAUSA_MS);
		}
	} while (_kbhit() == 0);				// Mientras no se pulsen teclas
	system("cls");
	tecla = _getch();
	return a;
}



void ultimomintemperatura(int l, int salida[], char filename1[])
{
	system("cls");
	int i;

	if (l > 60)
		i = l - 60;			// Si tiene más de 60 datos, empezará a leer desde el dato número (l - 60)
	else
		i = 0;			// Si tiene menos de 60 datos, empezará a leer desde el dato 0

	if (l > 0) // Si el fichero tiene datos dentro, los leerá
	{
		if (l > 60) {
			printf("\n TEMPERATURAS EN EL ÚLTIMO MINUTO\n\n");
			for (i; i < l; i++) {
				printf("   Segundo ");
				if (i >= l - 60 && i < l - 50) {			// Segundo que tiene un sólo dígito. Poner 0 delante para hacerlo más simétrico
					printf("0");
				}
				printf("%d: TEMPERATURA = %dºC\n", i - (l - 60), salida[i]);			// Imprime las temperaturas de los últimos 60 segundos registrados
			}
		}
		else {
			printf("\n TEMPERATURAS EN EL ÚLTIMO MINUTO\n\n");
			for (i; i < l; i++) {
				printf("   Segundo ");
				if (i >= 0 && i < 10) {			// Segundo que tiene un sólo dígito. Poner 1 delante para hacerlo más simétrico
					printf("0");
				}
				printf("%d: TEMPERATURA = %dºC\n", i, salida[i]);			// Imprime todos los datos registrados, pues son menos de 60
			}
		}
	}
	else
		printf("No hay datos registrados aún. Por favor, inicie primero la opción 2\n");
	printf("\n ");
	system("PAUSE");
	system("cls");
}



float leer_sensor_temperatura(Serial* Arduino)		// Función que devuelve el valor transferido desde Arduino
{
	float temperatura;
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "GET_TEMPERATURA\n", mensaje_recibido);

	if (bytesRecibidos <= 0)
		temperatura = -1;
	else
		temperatura = float_from_cadena(mensaje_recibido);
	return temperatura;
}



// *********************************************************************************
// HUMEDAD
// *********************************************************************************

void verifica_sensor_hum(Serial* Arduino, char* port)
{
	float humedad = -1.0;
	system("cls");
	if (Arduino->IsConnected())
	{
		humedad = leer_sensor_humedad(Arduino);
		if (humedad != -1)
			printf("\nEl sensor de humedad funciona correctamente.\n");
	}
	else
	{
		printf("\nNo se ha podido conectar con Arduino.\n");
		printf("Revise la conexión, el puerto %s y desactive el monitor serie del IDE de Arduino.\n", port);
	}
}



Minuto visualizar_datos_humedad(Serial* Arduino, char filename2[])
{
	system("cls");
	Minuto a{};
	float frecuencia = 1;
	float humedad = -1.0;
	int opcion = 1;					// contador 'dinámico' de minutos. Si han pasado 7 minutos: el minuto 5  es la opcion=1, el min 6 la opción=2, y el min 7 la opción=3.
	int minuto_exacto = 1;			// contador real de los minutos que han pasado. Si han pasado 7 minutos: el minuto 5  es el minuto_exacto = 5
	char tecla;
	int i = 0;						// contador de segundos
	int e;

	printf("Pulse una tecla para finalizar la monitorización\n");

	do
	{
		if (i < LONGCAD)			// Hasta completar el primer minuto
		{
			if (Arduino->IsConnected())
			{
				humedad = leer_sensor_humedad(Arduino);			// Se cogen los datos desde el sensor con ayuda de Arduino
				e = escribir_fichero_humedad(humedad, filename2);		// Se van copiando los nuevos datos al fichero Humedad.txt
				if (opcion == 1) {					// Último minuto 
					a.vector_min1[i] = humedad;
				}
				else if (opcion == 2) {
					a.vector_min2[i] = humedad;
				}
				else if (opcion == 3) {
					a.vector_min3[i] = humedad;
				}
				i++;
				if (i == LONGCAD) {				// Minuto se ha completado. Se resetea el contador y se cambia de ubicación el lugar donde se guardará el siguiente minuto
					i = 0;						// Reseteo de segundos
					printf("\nMinuto %d completado\n\n", minuto_exacto);
					opcion += 1;
					minuto_exacto += 1;
					if (opcion == 4) {				// Se han completado 3 minutos, se reemplazan los vectores
						opcion = 3;
						reemplazar_vectores(a.vector_min1, a.vector_min2);
						reemplazar_vectores(a.vector_min2, a.vector_min3);
					}
				}

				if (humedad != -1) {		// Si se ha medido bien la humedad
					printf(" Segundo ");
					if (i >= 0 && i < 10) {			// Segundo que tiene un sólo dígito. Poner 0 delante para hacerlo más simétrico
						printf("0");
					}
					printf("%d: HUMEDAD = %.2f %c\n", i, humedad, 37);				// ("%c", 37) -> equivale al símbolo '%', pues se trata de humedad relativa
				}
				else
					printf("XXX ");
			}
			else
				printf("\nNo se ha podido conectar con Arduino.\n");
			if ((1 / frecuencia) * 1000 > PAUSA_MS)
				Sleep((1 / frecuencia) * 1000 - PAUSA_MS);
		}
	} while (_kbhit() == 0);
	system("cls");
	tecla = _getch();
	return a;
}



void ultimominhumedad(int l, int salida[], char filename1[])
{
	system("cls");
	int i;

	if (l > 60)
		i = l - 60;
	else
		i = 0;

	if (l > 0) // Si el fichero no está vacío
	{
		if (l > 60) {
			printf("\n HUMEDAD EN EL ÚLTIMO MINUTO\n\n");
			for (i; i < l; i++) {
				printf("   Segundo ");
				if (i >= l - 60 && i < l - 50) {			// Segundo que tiene un sólo dígito. Poner 1 delante para hacerlo más simétrico
					printf("0");
				}
				printf("%d: HUMEDAD = %d %c\n", i - (l - 60), salida[i], 37);
			}
		}
		else {
			printf("\n HUMEDAD EN EL ÚLTIMO MINUTO\n\n");
			for (i; i < l; i++) {
				printf("   Segundo ");
				if (i >= 0 && i < 10) {			// Segundo que tiene un sólo dígito. Poner 1 delante para hacerlo más simétrico
					printf("0");
				}
				printf("%d: HUMEDAD = %d %c\n", i, salida[i], 37);
			}
		}
	}
	else
		printf("No hay datos registrados aún. Por favor, inicie primero la opción 5\n");
	printf("\n ");
	system("PAUSE");
	system("cls");
}



float leer_sensor_humedad(Serial* Arduino)		// Función que devuelve el valor transferido desde Arduino
{
	float humedad;
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "GET_HUMEDAD\n", mensaje_recibido);

	if (bytesRecibidos <= 0)
		humedad = -1;
	else
		humedad = float_from_cadena(mensaje_recibido);
	return humedad;
}


// *********************************************************************************
// LUMINOSIDAD
// *********************************************************************************

void verifica_sensor_lum(Serial* Arduino, char* port)
{
	float luminosidad;
	system("cls");
	if (Arduino->IsConnected())
	{
		luminosidad = leer_sensor_luminosidad(Arduino);
		if (luminosidad != -1)
			printf("\nEl sensor de luminosidad funciona correctamente.\n");
	}
	else
	{
		printf("\nNo se ha podido conectar con Arduino.\n");
		printf("Revise la conexión, el puerto %s y desactive el monitor serie del IDE de Arduino.\n", port);
	}
}



Minuto visualizar_datos_luminosidad(Serial* Arduino, char filename3[])
{
	system("cls");
	Minuto a{};
	float frecuencia = 1, luminosidad = -1;
	int opcion = 1;					// contador 'dinámico' de minutos. Si han pasado 7 minutos: el minuto 5  es la opcion=1, el min 6 la opción=2, y el min 7 la opción=3.
	int minuto_exacto = 1;			// contador real de los minutos que han pasado. Si han pasado 7 minutos: el minuto 5  es el minuto_exacto = 5
	char tecla;
	int i = 0;						// contador de segundos
	int e;

	printf("Pulse una tecla para finalizar la monitorización\n");

	do
	{
		if (i < LONGCAD)			// Hasta completar el primer minuto
		{
			if (Arduino->IsConnected())
			{
				luminosidad = leer_sensor_luminosidad(Arduino);			// En el programa se cogen los datos desde el sensor con ayuda de Arduino
				e = escribir_fichero_luminosidad(luminosidad, filename3);			// // Se van copiando los nuevos datos al fichero Luminosidad.txt
				if (opcion == 1) {
					a.vector_min1[i] = luminosidad;
				}
				else if (opcion == 2) {
					a.vector_min2[i] = luminosidad;
				}
				else if (opcion == 3) {
					a.vector_min3[i] = luminosidad;
				}
				i++;
				if (i == LONGCAD) {				// Minuto se ha completado. Se resetea el contador y se cambia de ubicación el lugar donde se guardará el siguiente minuto
					i = 0;						// Reseteo de segundos
					printf("\nMinuto %d completado\n\n", minuto_exacto);
					opcion += 1;
					minuto_exacto += 1;
					if (opcion == 4) {				// Se han completado 3 minutos, se reemplazan los vectores
						opcion = 3;
						reemplazar_vectores(a.vector_min1, a.vector_min2);
						reemplazar_vectores(a.vector_min2, a.vector_min3);
					}
				}


				if (luminosidad != -1) {
					printf(" Segundo ");
					if (i >= 0 && i < 10) {			// Segundo que tiene un sólo dígito. Poner 0 delante para hacerlo más simétrico
						printf("0");
					}
					printf("%d: LUMINOSIDAD = %.2f Lx\n", i, luminosidad);
				}
				else
					printf("XXX ");
			}
			else
				printf("\nNo se ha podido conectar con Arduino.\n");
			if ((1 / frecuencia) * 1000 > PAUSA_MS)
				Sleep((1 / frecuencia) * 1000 - PAUSA_MS);
		}
	} while (_kbhit() == 0);
	system("cls");
	tecla = _getch();
	return a;
}



void ultimominluminosidad(int l, int salida[], char filename1[])
{
	system("cls");
	int i;

	if (l > 60)
		i = l - 60;
	else
		i = 0;

	if (l > 0) // Si el fichero no está vacío
	{
		if (l > 60) {
			printf("\n LUMINOSIDAD EN EL ÚLTIMO MINUTO\n\n");
			for (i; i < l; i++) {
				printf("   Segundo ");
				if (i >= l - 60 && i < l - 50) {			// Segundo que tiene un sólo dígito. Poner 1 delante para hacerlo más simétrico
					printf("0");
				}
				printf("%d: LUMINOSIDAD = %d Lx\n", i - (l - 60), salida[i]);
			}
		}
		else {
			printf("\n LUMINOSIDAD EN EL ÚLTIMO MINUTO\n\n");
			for (i; i < l; i++) {
				printf("   Segundo ");
				if (i >= 0 && i < 10) {			// Segundo que tiene un sólo dígito. Poner 1 delante para hacerlo más simétrico
					printf("0");
				}
				printf("%d: LUMINOSIDAD = %d Lx\n", i, salida[i]);
			}
		}
	}
	else
		printf("No hay datos registrados aún. Por favor, inicie primero la opción 8\n");
	printf("\n ");
	system("PAUSE");
	system("cls");
}



float leer_sensor_luminosidad(Serial* Arduino)			// Función que devuelve el valor transferido desde Arduino
{
	float luminosidad;
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "GET_LUMINOSIDAD\n", mensaje_recibido);

	if (bytesRecibidos <= 0)
		luminosidad = -1;
	else
		luminosidad = float_from_cadena(mensaje_recibido);
	return luminosidad;
}



// FICHEROS-------------------------------------------------------------------------------

// FICHERO TEMPERATURA
int crear_fichero_temperatura(char filename[])			//// Se crea un fichero vacío en donde se irán incorporando nuevos datos
{
	FILE* fichero;
	errno_t e;
	int resultado, i = 0;

	e = fopen_s(&fichero, filename, "wt"); // Se abre el fichero en modo escritura
	if (fichero == NULL)
		resultado = 0; // El fichero no ha podido crearse
	else
	{
		resultado = 1; // El fichero ha podido crearse

		fprintf(fichero, ""); // Se crea un fichero vacío
		fclose(fichero); // Se cierra el fichero abierto
	}
	return resultado;
}



int escribir_fichero_temperatura(int temperatura, char filename[])
{
	int i, j;
	FILE* fichero;
	errno_t e;

	e = fopen_s(&fichero, filename, "at");
	if (e == 0) // Si el fichero se ha podido crear
	{
		fprintf(fichero, "%d\n", temperatura); // Se graban en el fichero las temperaturas nuevas
		fclose(fichero);
	}
	else
		printf("Se ha producido un problema a la hora de grabar en el fichero las nuevas temperaturas\n");
	return e;
}



int leer_fichero_temperatura(int temperatura[], char filename[])		// Imprime por pantalla el contenido del fichero
{
	FILE* fichero;
	int resultado, i = 0;
	errno_t e;
	int n = 0;

	e = fopen_s(&fichero, filename, "rt"); // Se abre el fichero en modo lectura
	if (fichero == NULL)
		resultado = 0; // El fichero no ha podido abrirse
	else
	{
		resultado = 1; // El fichero ha podido abrirse
		while (!feof(fichero))
		{
			fscanf_s(fichero, "%d\n", &temperatura[n]);
			if (temperatura[n] > 0 && temperatura[n] <= 50)		// Distingue si se están cogiendo temperaturas de manera adecuada. 0 y 50 es el rango en el que el sensor funciona
				n++;
		}
		fclose(fichero); // Se cierra el fichero abierto
	}
	return n;		// Devuelve la cantidad de datos que contiene el archivo
}

// FICHERO HUMEDAD
int crear_fichero_humedad(char filename[])		// Se crea un fichero vacío en donde se irán incorporando nuevos datos
{
	FILE* fichero;
	errno_t e;
	int resultado, i = 0;

	e = fopen_s(&fichero, filename, "wt"); // Se abre el fichero en modo escritura
	if (fichero == NULL)
		resultado = 0; // El fichero no ha podido crearse
	else
	{
		resultado = 1; // El fichero ha podido crearse

		fprintf(fichero, ""); // Se crea un fichero vacío
		fclose(fichero); // Se cierra el fichero abierto
	}
	return resultado;
}



int escribir_fichero_humedad(int humedad, char filename[])
{
	int i, j;
	FILE* fichero;
	errno_t e;

	e = fopen_s(&fichero, filename, "at");
	if (e == 0) // Si el fichero se ha podido crear
	{
		fprintf(fichero, "%d\n", humedad); // Se graban en el fichero los nuevos datos
		fclose(fichero);
	}
	else
		printf("Se ha producido un problema a la hora de grabar en el fichero los nuevos datos de humedad\n");
	return e;
}



int leer_fichero_humedad(int humedad[], char filename[])		// Imprime por pantalla el contenido del fichero
{
	FILE* fichero;
	int resultado;
	errno_t e;
	int n = 0;

	e = fopen_s(&fichero, filename, "rt"); // Se abre el fichero en modo lectura
	if (fichero == NULL)
		resultado = 0; // El fichero no ha podido abrirse
	else
	{
		resultado = 1; // El fichero ha podido abrirse
		while (!feof(fichero))
		{
			fscanf_s(fichero, "%d\n", &humedad[n]);
			if (humedad[n] >= 30 && humedad[n] <= 80)		// Distingue si se están cogiendo los datos de manera adecuada. 30 y 80 es el rango en el que el sensor funciona
				n++;
		}

		fclose(fichero); // Se cierra el fichero abierto
	}
	return n;
}


// FICHERO LUMINOSIDAD
int crear_fichero_luminosidad(char filename[])					// Se crea un fichero vacío en donde se irán incorporando nuevos datos
{
	FILE* fichero;
	errno_t e;
	int resultado;

	e = fopen_s(&fichero, filename, "wt"); // Se abre el fichero en modo escritura
	if (fichero == NULL)
		resultado = 0; // El fichero no ha podido crearse
	else
	{
		resultado = 1; // El fichero ha podido crearse

		fprintf(fichero, ""); // Se crea un fichero vacío
		fclose(fichero); // Se cierra el fichero abierto
	}
	return resultado;
}



int escribir_fichero_luminosidad(int luminosidad, char filename[])
{
	FILE* fichero;
	errno_t e;

	e = fopen_s(&fichero, filename, "at");
	if (e == 0) // Si el fichero se ha podido crear
	{
		fprintf(fichero, "%d\n", luminosidad); // Se graban en el fichero los nuevos datos
		fclose(fichero);
	}
	else
		printf("Se ha producido un problema a la hora de grabar en el fichero los nuevos datos de luminosidad\n");
	return e;
}



int leer_fichero_luminosidad(int luminosidad[], char filename[])		// Imprime por pantalla el contenido del fichero
{
	FILE* fichero;
	int resultado;
	errno_t e;
	int n = 0;

	e = fopen_s(&fichero, filename, "rt"); // Se abre el fichero en modo lectura
	if (fichero == NULL)
		resultado = 0; // El fichero no ha podido abrirse
	else
	{
		resultado = 1; // El fichero ha podido abrirse
		while (!feof(fichero))
		{
			fscanf_s(fichero, "%d\n", &luminosidad[n]);
			if (luminosidad[n] > 60 && luminosidad[n] <= 2000)		// Distingue si se están cogiendo los datos de manera adecuada.
				n++;
		}

		fclose(fichero); // Se cierra el fichero abierto
	}
	return n;
}