#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <conio.h>
#include "SerialClass.h"
#include "SerialClass.cpp"

#define MAX_BUFFER 200
#define PAUSA_MS 200

// Funciones prototipo
int menu(void);
void verifica_sensores(Serial*, char*);
void monitorizar_sensor_temperatura(Serial*);
//void visualizar_registro_temperatura(Serial*);
void activar_alarma_temperatura(Serial* Arduino);
void comprobar_mensajes(Serial*);
float leer_sensor_temperatura(Serial*);
int Enviar_y_Recibir(Serial*, const char*, char*);
float float_from_cadena(char* cadena);

int main(void)
{
	Serial* Arduino;
	char puerto[] = "COM6"; // Puerto serie al que está conectado Arduino
	int opcion_menu;


	setlocale(LC_ALL, "es-ES");
	Arduino = new Serial((char*)puerto);
	comprobar_mensajes(Arduino);
	opcion_menu = menu();

	while (opcion_menu > 14 || opcion_menu < 0)
	{
		printf("Introduzca una de las opciones disponibles, por favor: ");
		scanf_s("%d", &opcion_menu);
	}
	while (opcion_menu <= 14 && opcion_menu >= 1)
	{
		if (opcion_menu == 1) {
			printf("Mostrando datos...\n");
			verifica_sensores(Arduino, puerto);
		}
		if (opcion_menu == 2) {
			printf("Mostrando datos...\n");
			monitorizar_sensor_temperatura(Arduino);
		}
		if (opcion_menu == 3) {
			printf("Mostrando datos...\n");
			//visualizar_registro_temperatura(Arduino);
		}
		if (opcion_menu == 4) {
			printf("Mostrando datos...\n");
			activar_alarma_temperatura(Arduino);
		}
		if (opcion_menu == 5) {
			printf("Mostrando datos...\n");
			verifica_sensores(Arduino, puerto);
		}
		if (opcion_menu == 6) {
			printf("Mostrando datos...\n");
			monitorizar_sensor_temperatura(Arduino);
		}
		if (opcion_menu == 7) {
			printf("Mostrando datos...\n");
			
		}
		if (opcion_menu == 8) {
			printf("Mostrando datos...\n");
			activar_alarma_temperatura(Arduino);
		}
		if (opcion_menu == 9) {
			printf("Mostrando datos...\n");
			verifica_sensores(Arduino, puerto);
		}
		if (opcion_menu == 10) {
			printf("Mostrando datos...\n");
			monitorizar_sensor_temperatura(Arduino);
		}
		if (opcion_menu == 11) {
			printf("Mostrando datos...\n");
			
		}
		if (opcion_menu == 12) {
			printf("Mostrando datos...\n");
			activar_alarma_temperatura(Arduino);
		}
		if (opcion_menu == 13)
			printf("Mostrando datos...\n");
		if (opcion_menu == 14)
			printf("Mostrando datos...\n");


		printf("   Introduzca otra opción: ");
		scanf_s("%d", &opcion_menu);
		while (opcion_menu > 14 || opcion_menu < 0)
		{
			printf("   Introduzca una de las opciones disponibles, por favor: ");
			scanf_s("%d", &opcion_menu);
		}


		if (opcion_menu == 0)
			printf("Fin del programa");
	}
}


// *********************************************************************************
// La función menú se limita a mostrar en pantalla la oferta de opciones disponibles
// *********************************************************************************
int menu(void)
{
	int opcion = -1;

	printf("\n   Sistema automático de control de invernadero");
	printf("\n   ===========================================");
	printf("\n\n\t1 - Verificar sensores de TEMPERATURA");
	printf("\n\t2 - Monitorizar sensores de TEMPERATURA");
	printf("\n\t3 - Visualizar registro de TEMPERATURA");
	printf("\n\t4 - Activar/Desactivar alarma de TEMPERATURA\n");

	printf("\n\n\t5 - Verificar sensores de HUMEDAD");
	printf("\n\t6 - Monitorizar sensores de HUMEDAD");
	printf("\n\t7 - Visualizar registro de HUMEDAD");
	printf("\n\t8 - Activar/Desactivar alarma de HUMEDAD\n");

	printf("\n\n\t9 - Verificar sensores de LUMINOSIDAD");
	printf("\n\t10 - Monitorizar sensores de LUMINOSIDAD");
	printf("\n\t11 - Visualizar registro de LUMINOSIDAD");
	printf("\n\t12 - Activar/Desactivar alarma de LUMINOSIDAD\n");

	printf("\n\tINFORMACIÓN ADICIONAL");
	printf("\n\n\t\t13 - Historial cronológico");
	printf("\n\t\t14 - Gráficas");
	printf("\n\n\t0 - Salir de la aplicación");
	printf("\n\n   Introduzca una opción: ");

	scanf_s("%d", &opcion);

	return opcion;
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

void activar_alarma_temperatura(Serial* Arduino)
{
	int bytesRecibidos;
	char mensaje_recibido[MAX_BUFFER];

	bytesRecibidos = Enviar_y_Recibir(Arduino, "SET_MODO_ALARMA\n", mensaje_recibido);
	if (bytesRecibidos <= 0)
		printf("\nNo se ha recibido confirmación\n");
	else
		printf("\n%s\n", mensaje_recibido);
}

void monitorizar_sensor_temperatura(Serial* Arduino)
{
	float frecuencia, temperatura;
	char tecla;
	do
	{
		printf("Establezca frecuencia de muestreo (0,5 Hz - 2,0 Hz):");
		scanf_s("%f", &frecuencia);
	} while (frecuencia < 0.5 || frecuencia>2.0);

	printf("Pulse una tecla para finalizar la monitorización\n");
	do
	{
		if (Arduino->IsConnected())
		{
			temperatura = leer_sensor_temperatura(Arduino);
			if (temperatura != -1)
				printf("%.2f ", temperatura);
			else
				printf("XXX ");
		}
		else
			printf("\nNo se ha podido conectar con Arduino.\n");
		if ((1 / frecuencia) * 1000 > PAUSA_MS)
			Sleep((1 / frecuencia) * 1000 - PAUSA_MS);
	} while (_kbhit() == 0);
	tecla = _getch();
	return;
}

void verifica_sensores(Serial* Arduino, char* port)
{
	float temperatura;

	if (Arduino->IsConnected())
	{
		temperatura = leer_sensor_temperatura(Arduino);
		if (temperatura != -1)
			printf("\nDistancia: %f\n", temperatura);
	}
	else
	{
		printf("\nNo se ha podido conectar con Arduino.\n");
		printf("Revise la conexión, el puerto %s y desactive el monitor serie del IDE de Arduino.\n", port);
	}
}

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

int Enviar_y_Recibir(Serial* Arduino, const char* mensaje_enviar, char* mensaje_recibir)
{
	int bytes_recibidos = 0, total = 0;
	int intentos = 0, fin_linea = 0;


	Arduino->WriteData((char*)mensaje_enviar, strlen(mensaje_enviar));
	Sleep(PAUSA_MS);

	bytes_recibidos = Arduino->ReadData(mensaje_recibir, sizeof(char) * MAX_BUFFER - 1);

	while ((bytes_recibidos > 0 || intentos < 5) && fin_linea == 0)
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

	//printf("LOG: %d bytes -> %s\nIntentos=%d - EOLN=%d\n", total, mensaje_recibir,intentos,fin_linea);
	return total;
}





float float_from_cadena(char* cadena)
{
	float numero = 0;
	int i, divisor = 10, estado = 0;


	for (i = 0; cadena[i] != '\0' && estado != 3 && i < MAX_BUFFER; i++)
		switch (estado)
		{
		case 0:// Antes del número
			if (cadena[i] >= '0' && cadena[i] <= '9')
			{
				numero = cadena[i] - '0';
				estado = 1;
			}
			break;
		case 1:// Durante el número
			if (cadena[i] >= '0' && cadena[i] <= '9')
				numero = numero * 10 + cadena[i] - '0';
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
				divisor *= 10;
			}
			else
				estado = 3;
			break;
		}
	return numero;
}