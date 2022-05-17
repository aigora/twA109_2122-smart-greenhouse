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
int crear_fichero_temperatura(int[],char[]);
int escribir_fichero_temperatura(Serial*, int[],char[]);
int leer_fichero_temperatura(int[],char[]);
void fichero(void);

// Temperatura
void verifica_sensor_temp(Serial*, char*);
void visualizar_datos_temperatura(Serial*);
//void print_minuto_temp(float*, Serial*);
float leer_sensor_temperatura(Serial*);

// Humedad
void verifica_sensor_hum(Serial*, char*);
void visualizar_datos_humedad(Serial*);
//void tres_minutos_humedad(Serial*);
float leer_sensor_humedad(Serial*);

// Luminosidad
void verifica_sensor_lum(Serial*, char*);
void visualizar_datos_luminosidad(Serial*);
//Minuto tres_minutos_luminosidad(Serial*);
float leer_sensor_luminosidad(Serial*);



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

	comprobar_mensajes(Arduino);

	opcion_menu = menu();
	int opcion2;
	while (opcion_menu > 13 || opcion_menu < 0)		// Opcion incorrecta
	{
		printf("Introduzca una de las opciones disponibles, por favor: ");
		scanf_s("%d", &opcion_menu);
	}
	while (opcion_menu <= 13 && opcion_menu >= 1)		// Opcion correcta
	{
		if (opcion_menu == 1)
		{
			verifica_sensor_temp(Arduino, puerto);
			opcion_menu = menu();
		}
		if (opcion_menu == 2)
		{
			visualizar_datos_temperatura(Arduino);
			opcion_menu = menu();
		}
		if (opcion_menu == 3)
		{
			visualizar_datos_temperatura(Arduino);
			opcion_menu = menu();
		}
		if (opcion_menu == 4)
		{
			verifica_sensor_hum(Arduino, puerto);
			opcion_menu = menu();
		}
		if (opcion_menu == 5)
		{
			visualizar_datos_humedad(Arduino);
			opcion_menu = menu();
		}
		if (opcion_menu == 6)
		{
			//tres_minutos_humedad(Arduino);
			visualizar_datos_humedad(Arduino);
			opcion_menu = menu();
		}
		if (opcion_menu == 7)
		{
			verifica_sensor_lum(Arduino, puerto);
			opcion_menu = menu();
		}
		if (opcion_menu == 8)
		{
			visualizar_datos_luminosidad(Arduino);
			opcion_menu = menu();
		}
		if (opcion_menu == 9)
		{
			//tres_minutos_luminosidad(Arduino);
			visualizar_datos_luminosidad(Arduino);
			opcion_menu = menu();
		}
		if (opcion_menu == 10)
		{
			activar_alarma(Arduino);
			opcion_menu = menu();
		}
		if (opcion_menu == 11)
		{
			system("cls");
			printf("\n\n\t1 - Historial cronológico");
			printf("\n\t2 - Gráficas");
			printf("\n\tIntroduzca una opcion: ");
			scanf_s("%d", &opcion2);
			if (opcion2 == 1)
			{
				system("cls");
				printf("\n\tHistorial cronológico:\n");
				printf("\t---------------------\n\n\tInsertar historial\n\n");
				opcion_menu = menu();
			}
			if (opcion2 == 2)
			{
				system("cls");
				printf("\n\tGráficas:\n");
				printf("\t--------\n\n\tInsertar gráfica\n\n");
				opcion_menu = menu();
			}
		}
		if (opcion_menu == 12)
		{
			system("cls");
			fichero();		// Imprime por pantalla el contenido
			opcion_menu = menu();
		}
		while (opcion_menu > 12 || opcion_menu < 0) {
			printf("Introduzca una de las opciones disponibles, por favor: ");
			scanf_s("%d", &opcion_menu);
		}
	}

	if (opcion_menu == 0)
		printf("Fin del programa\n");

	// Fichero
	
	//system("PAUSE");			// Espera a que el usuario pulse una tecla

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
	printf("\n\t3 - Consultar últimos tres minutos de TEMPERATURA");

	printf("\n\n\t4 - Verificar sensores de HUMEDAD");
	printf("\n\t5 - Visualizar registro de HUMEDAD en tiempo real");
	printf("\n\t6 - Consultar últimos tres minutos de HUMEDAD");

	printf("\n\n\t7 - Verificar sensores de LUMINOSIDAD");
	printf("\n\t8 - Visualizar registro de LUMINOSIDAD en tiempo real");
	printf("\n\t9 - Consultar últimos tres minutos de LUMINOSIDAD");

	printf("\n\n\t10 - Activar/Desactivar alarma");

	printf("\n\n\t11 - INFORMACIÓN ADICIONAL");

	printf("\n\n\t12 - Ver contenido fichero");

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
		bytes_recibidos = Arduino->ReadData(mensaje_recibir + total, sizeof(char) * MAX_BUFFER - 1);
	}
	if (total > 0)
		mensaje_recibir[total - 1] = '\0';

	//printf("LOG: %d bytes -> %s\nIntentos=%d - EOLN=%d\n", total, mensaje_recibir, intentos, fin_linea);
	return total;
}



float float_from_cadena(char* cadena)
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



void activar_alarma(Serial* Arduino)
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];
	system("cls");

	bytesRecibidos = Enviar_y_Recibir(Arduino, "SET_MODO_ALARMA\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
		printf("\nNo se ha recibido confirmación\n");
	else
		printf("\n%s\n", mensaje_recibido);
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
	float temperatura = -1;
	system("cls");
	if (Arduino->IsConnected())
	{
		temperatura = leer_sensor_temperatura(Arduino);
		if (temperatura != -1)
			printf("\nEl sensor de temperatura funciona correctamente.\n");
	}
	else
	{
		printf("\nNo se ha podido conectar con Arduino.\n");
		printf("Revise la conexión, el puerto %s y desactive el monitor serie del IDE de Arduino.\n", port);
	}
}



void visualizar_datos_temperatura(Serial* Arduino)
{
	float frecuencia = 1, temperatura = -1;
	float vector1[LONGCAD];			// Vector que recoge 60 temperaturas en un minuto
	float vector2[LONGCAD];
	float vector3[LONGCAD];
	int opcion = 1;					// contador 'relativo' de minutos
	int minuto_exacto = 1;			// contador real de los minutos que han pasado
	char tecla;
	int i = 0;						// contador de segundos


	printf("Pulse una tecla para finalizar el registro\n");

	do
	{
		if (i < LONGCAD)			// Hasta completar el primer minuto
		{
			if (Arduino->IsConnected())
			{
			//temperatura = rand();											// Para hacer la prueba de que el programa imprime bien los datos, se escogen numeros aleatorios
				temperatura = leer_sensor_temperatura(Arduino);			// En el programa se cogen los datos desde el sensor con ayuda de Arduino
			if (opcion == 1) {
				vector1[i] = temperatura;
			}
			else if (opcion == 2) {
				vector2[i] = temperatura;
			}
			else if (opcion == 3) {
				vector3[i] = temperatura;
			}
			i++;
			if (i == LONGCAD) {				// Minuto se ha completado. Se resetea el contador y se cambia de ubicación donde se guarda el siguiente minuto
				i = 0;						// reseteo de segundos
				printf("\nMinuto %d completado\n\n", minuto_exacto);
				opcion += 1;
				minuto_exacto += 1;
				if (opcion == 4) {				// Se han completado 3 minutos, se reemplazan los vectores
					opcion = 3;
					reemplazar_vectores(vector1, vector2);
					reemplazar_vectores(vector2, vector3);
				}
			}


			if (temperatura != -1) {
				printf(" Segundo ");
				if (i >= 0 && i < 10) {			// Segundo que tiene un sólo dígito. Poner 1 delante para hacerlo más simétrico
					printf("0");
				}
				printf("%d: TEMPERATURA = %.2fºC\n", i, temperatura);
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
	return;
}


/*void print_minuto_temp(float* p, Serial* Arduino)
{
	float* p = visualizar_datos_temperatura(Arduino);
	for (int j = 0; j < 3; j++) {
		printf("\nMinuto %d:\n", j + 1);
		for (int i = 0; i < LONGCAD; i++)
		{
			if (i >= 0 && i < 10)
				printf(" ");
			printf("%d seg - TEMPERATURA: %fºC\n", i, p[j]->[i]);
		}
	}
	printf("\n\n");
}*/


/*Minuto tres_minutos_temperatura(Serial* Arduino)
{
	float frecuencia = 1, temperatura = -1;
	float vector1[LONGCAD];			// Vector que recoge 60 temperaturas en un minuto
	float vector2[LONGCAD];
	float vector3[LONGCAD];
	int opcion = 1;					// contador 'relativo' de minutos
	int minuto_exacto = 1;			// contador real de los minutos que han pasado
	char tecla;
	int i = 0;						// contador de segundos


	do
	{
		if (i < LONGCAD)			// Hasta completar el primer minuto
		{
			//if (Arduino->IsConnected())
			//{
			temperatura = rand();											// Para hacer la prueba de que el programa imprime bien los datos, se escogen numeros aleatorios
				//temperatura = leer_sensor_temperatura(Arduino);			// En el programa se cogen los datos desde el sensor con ayuda de Arduino
			if (opcion == 1) {
				vector1[i] = temperatura;
			}
			else if (opcion == 2) {
				vector2[i] = temperatura;
			}
			else if (opcion == 3) {
				vector3[i] = temperatura;
			}
			i++;
			if (i == LONGCAD) {				// Minuto se ha completado. Se resetea el contador y se cambia de ubicación donde se guarda el siguiente minuto
				i = 0;						// reseteo de segundos
				//printf("\nMinuto %d completado\n\n", minuto_exacto);
				opcion += 1;
				minuto_exacto += 1;
				if (opcion == 4) {				// Se han completado 3 minutos, se reemplazan los vectores
					opcion = 3;
					reemplazar_vectores(vector1, vector2);
					reemplazar_vectores(vector2, vector3);
				}
			}
			//}
			//else
			//	printf("\nNo se ha podido conectar con Arduino.\n");
			if ((1 / frecuencia) * 1000 > PAUSA_MS)
				Sleep((1 / frecuencia) * 1000 - PAUSA_MS);
		}
	} while (_kbhit() == 0);
	tecla = _getch();

	Minuto minuto;
	for (int j = 0; j < LONGCAD; j++) {
		minuto.vector1[j] = vector1[j];
		minuto.vector2[j] = vector2[j];
		minuto.vector3[j] = vector3[j];
	}
	return minuto;
}*/



float leer_sensor_temperatura(Serial* Arduino)
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
	float humedad;
	system("cls");
	if (Arduino->IsConnected())
	{
		humedad = leer_sensor_humedad(Arduino);
		if (humedad != -1)
			printf("\Humedad: %f\n", humedad);
	}
	else
	{
		printf("\nNo se ha podido conectar con Arduino.\n");
		printf("Revise la conexión, el puerto %s y desactive el monitor serie del IDE de Arduino.\n", port);
	}
}



void visualizar_datos_humedad(Serial* Arduino)
{
	float frecuencia = 1, humedad = -1;
	float vector1[LONGCAD];			// Vector que recoge 60 temperaturas en un minuto
	float vector2[LONGCAD];
	float vector3[LONGCAD];
	int opcion = 1;					// contador 'relativo' de minutos
	int minuto_exacto = 1;			// contador real de los minutos que han pasado
	char tecla;
	int i = 0;						// contador de segundos

	printf("Pulse una tecla para finalizar la monitorización\n");

	do
	{
		if (i < LONGCAD)			// Hasta completar el primer minuto
		{
			if (Arduino->IsConnected())
			{
			//humedad = rand();											// Para hacer la prueba de que el programa imprime bien los datos, se escogen numeros aleatorios
				humedad = leer_sensor_humedad(Arduino);			// En el programa se cogen los datos desde el sensor con ayuda de Arduino
			if (opcion == 1) {
				vector1[i] = humedad;
			}
			else if (opcion == 2) {
				vector2[i] = humedad;
			}
			else if (opcion == 3) {
				vector3[i] = humedad;
			}
			i++;
			if (i == LONGCAD) {				// Minuto se ha completado. Se resetea el contador y se cambia de ubicación donde se guarda el siguiente minuto
				i = 0;						// reseteo de segundos
				printf("\nMinuto %d completado\n\n", minuto_exacto);
				opcion += 1;
				minuto_exacto += 1;
				if (opcion == 4) {				// Se han completado 3 minutos, se reemplazan los vectores
					opcion = 3;
					reemplazar_vectores(vector1, vector2);
					reemplazar_vectores(vector2, vector3);
				}
			}


			if (humedad != -1) {
				printf(" Segundo ");
				if (i >= 0 && i < 10) {			// Segundo que tiene un sólo dígito. Poner 1 delante para hacerlo más simétrico
					printf("0");
				}
				printf("%d: HUMEDAD = %.2f g/m3\n", i, humedad);
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
	return;
}



/*void tres_minutos_humedad(Serial* Arduino)
{
	float frecuencia = 1, humedad = -1;
	float vector1[LONGCAD];			// Vector que recoge 60 temperaturas en un minuto
	float vector2[LONGCAD];
	float vector3[LONGCAD];
	int opcion = 1;					// contador 'relativo' de minutos
	int minuto_exacto = 1;			// contador real de los minutos que han pasado
	char tecla;
	int i = 0;						// contador de segundos


	do
	{
		if (i < LONGCAD)			// Hasta completar el primer minuto
		{
			//if (Arduino->IsConnected())
			//{
			humedad = rand();											// Para hacer la prueba de que el programa imprime bien los datos, se escogen numeros aleatorios
				//temperatura = leer_sensor_temperatura(Arduino);			// En el programa se cogen los datos desde el sensor con ayuda de Arduino
			if (opcion == 1) {
				vector1[i] = humedad;
			}
			else if (opcion == 2) {
				vector2[i] = humedad;
			}
			else if (opcion == 3) {
				vector3[i] = humedad;
			}
			i++;
			if (i == LONGCAD) {				// Minuto se ha completado. Se resetea el contador y se cambia de ubicación donde se guarda el siguiente minuto
				i = 0;						// reseteo de segundos
				//printf("\nMinuto %d completado\n\n", minuto_exacto);
				opcion += 1;
				minuto_exacto += 1;
				if (opcion == 4) {				// Se han completado 3 minutos, se reemplazan los vectores
					opcion = 3;
					reemplazar_vectores(vector1, vector2);
					reemplazar_vectores(vector2, vector3);
				}
			}
			//}
			//else
			//	printf("\nNo se ha podido conectar con Arduino.\n");
			if ((1 / frecuencia) * 1000 > PAUSA_MS)
				Sleep((1 / frecuencia) * 1000 - PAUSA_MS);
		}
	} while (_kbhit() == 0);
	tecla = _getch();

	Minuto minuto;
	for (int j = 0; j < LONGCAD; j++) {
		minuto.vector1[j] = vector1[j];
		minuto.vector2[j] = vector2[j];
		minuto.vector3[j] = vector3[j];
	}
	//return minuto;
}*/



float leer_sensor_humedad(Serial* Arduino)
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
			printf("\Luminosidad: %f\n", luminosidad);
	}
	else
	{
		printf("\nNo se ha podido conectar con Arduino.\n");
		printf("Revise la conexión, el puerto %s y desactive el monitor serie del IDE de Arduino.\n", port);
	}
}



void visualizar_datos_luminosidad(Serial* Arduino)
{
	float frecuencia = 1, luminosidad = -1;
	float vector1[LONGCAD];			// Vector que recoge 60 datos de luminosidad en un minuto
	float vector2[LONGCAD];
	float vector3[LONGCAD];
	int opcion = 1;					// contador 'relativo' de minutos
	int minuto_exacto = 1;			// contador real de los minutos que han pasado
	char tecla;
	int i = 0;						// contador de segundos

	printf("Pulse una tecla para finalizar la monitorización\n");

	do
	{
		if (i < LONGCAD)			// Hasta completar el primer minuto
		{
			if (Arduino->IsConnected())
			{
			//luminosidad = rand();											// Para hacer la prueba de que el programa imprime bien los datos, se escogen numeros aleatorios
				luminosidad = leer_sensor_luminosidad(Arduino);			// En el programa se cogen los datos desde el sensor con ayuda de Arduino
			if (opcion == 1) {
				vector1[i] = luminosidad;
			}
			else if (opcion == 2) {
				vector2[i] = luminosidad;
			}
			else if (opcion == 3) {
				vector3[i] = luminosidad;
			}
			i++;
			if (i == LONGCAD) {				// Minuto se ha completado. Se resetea el contador y se cambia de ubicación donde se guarda el siguiente minuto
				i = 0;						// Reseteo de segundos
				printf("\nMinuto %d completado\n\n", minuto_exacto);
				opcion += 1;
				minuto_exacto += 1;
				if (opcion == 4) {				// Se han completado 3 minutos, se reemplazan los vectores
					opcion = 3;
					reemplazar_vectores(vector1, vector2);
					reemplazar_vectores(vector2, vector3);
				}
			}


			if (luminosidad != -1) {
				printf(" Segundo ");
				if (i >= 0 && i < 10) {			// Segundo que tiene un sólo dígito. Poner 1 delante para hacerlo más simétrico
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
	return;
}



/*void tres_minutos_luminosidad(Serial* Arduino)
{
	float frecuencia = 1, luminosidad = -1;
	float vector1[LONGCAD];			// Vector que recoge 60 temperaturas en un minuto
	float vector2[LONGCAD];
	float vector3[LONGCAD];
	int opcion = 1;					// contador 'relativo' de minutos
	int minuto_exacto = 1;			// contador real de los minutos que han pasado
	char tecla;
	int i = 0;						// contador de segundos


	do
	{
		if (i < LONGCAD)			// Hasta completar el primer minuto
		{
			//if (Arduino->IsConnected())
			//{
			luminosidad = rand();											// Para hacer la prueba de que el programa imprime bien los datos, se escogen numeros aleatorios
				//temperatura = leer_sensor_temperatura(Arduino);			// En el programa se cogen los datos desde el sensor con ayuda de Arduino
			if (opcion == 1) {
				vector1[i] = luminosidad;
			}
			else if (opcion == 2) {
				vector2[i] = luminosidad;
			}
			else if (opcion == 3) {
				vector3[i] = luminosidad;
			}
			i++;
			if (i == LONGCAD) {				// Minuto se ha completado. Se resetea el contador y se cambia de ubicación donde se guarda el siguiente minuto
				i = 0;						// reseteo de segundos
				//printf("\nMinuto %d completado\n\n", minuto_exacto);
				opcion += 1;
				minuto_exacto += 1;
				if (opcion == 4) {				// Se han completado 3 minutos, se reemplazan los vectores
					opcion = 3;
					reemplazar_vectores(vector1, vector2);
					reemplazar_vectores(vector2, vector3);
				}
			}
			//}
			//else
			//	printf("\nNo se ha podido conectar con Arduino.\n");
			if ((1 / frecuencia) * 1000 > PAUSA_MS)
				Sleep((1 / frecuencia) * 1000 - PAUSA_MS);
		}
	} while (_kbhit() == 0);
	tecla = _getch();

	Minuto minuto;
	for (int j = 0; j < LONGCAD; j++) {
		minuto.vector1[j] = vector1[j];
		minuto.vector2[j] = vector2[j];
		minuto.vector3[j] = vector3[j];
	}
	for (int k = 0; k < LONGCAD; k++) {
		printf(" Segundo ");
		if (i >= 0 && i < 10) {			// Segundo que tiene un sólo dígito. Poner 1 delante para hacerlo más simétrico
			printf("0");
		}
		printf("%d: LUMINOSIDAD = %.2f Lx\n", k, minuto.vector1[k]);
	}
	return;
}*/



float leer_sensor_luminosidad(Serial* Arduino)
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

int crear_fichero_temperatura(int temperatura[],char filename[])
{
	FILE* fichero;
	errno_t e;
	int resultado, i;

	e = fopen_s(&fichero, filename, "wt"); // Se abre el fichero en modo escritura
	if (fichero == NULL)
		resultado = 0; // El fichero no ha podido crearse
	else
	{
		resultado = 1; // El fichero ha podido crearse
		for (i = 0; i < LONGCAD; i++)
			fprintf(fichero, "%d ", temperatura[i]); // ¡Importante!: Dejar un espacio blanco
		fclose(fichero); // Se cierra el fichero abierto
	}
	return resultado;
}



int escribir_fichero_temperatura(Serial* Arduino, int temperatura[],char filename[])
{
	int i, j;
	FILE* fichero;
	errno_t e;
	float vector1[LONGCAD];
	for (j = 0; j < LONGCAD; j++) {
		vector1[j] = leer_sensor_temperatura(Arduino);		// Transferir datos a vector
	}


	e = fopen_s(&fichero, filename, "at");
	if (e == 0) // Si el fichero se ha podido crear
	{
		for (j = 0; j < LONGCAD; j++) {
			fprintf(fichero, "%d ", temperatura[j]); // Se graban en el fichero las temperaturas nuevas
		}

		fclose(fichero);
	}
	else
		printf("Se ha producido un problema a la hora de grabar el fichero de las temperaturas\n");
	return e;
}



int leer_fichero_temperatura(int temperatura[],char filename[])		// Imprime por pantalla el contenido del fichero
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
			fscanf_s(fichero, "%d ", &temperatura[n]);
			n++;
		}

		fclose(fichero); // Se cierra el fichero abierto
	}
	return resultado;
}

void fichero(void)		// Junta todas las funciones relacionadas con ficheros de este programa
{
	char filename[] = "Temperatura.txt";
	int entrada[LONGCAD];
	int salida[LONGCAD];
	int exito, i;
	for (i = 0; i < LONGCAD; i++)
		//entrada[i] = leer_sensor_temperatura(Arduino);
		entrada[i] = 2 * i;		// Se ponen numeros aleatorios para ver que se crea correctamente el fichero
	exito = crear_fichero_temperatura(entrada, filename);
	if (exito == 1) // Si se ha podido crear el fichero
	{
		exito = leer_fichero_temperatura(salida, filename);		// Imprime por pantalla el contido del fichero
		if (exito == 1) // Si se ha podido leer el fichero
		{
			printf("Contenido del fichero: ");
			for (i = 0; i < LONGCAD; i++)
				printf("%d ", salida[i]);
		}
		else
			printf("No se ha podido crear el fichero\n");
	}
	else
		printf("No se ha podido abrir el fichero\n");

	printf("\n");
}
