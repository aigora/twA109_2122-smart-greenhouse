#include<stdio.h>
#include<stdlib.h>
#include<locale.h>

#define MAX_BUFFER 200
#define PAUSA_MS 200

int main(void)
{
	setlocale(LC_ALL, "");
	int opcion;

	printf("   Sistema automático de control de invernadero");
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


	while (opcion > 14 || opcion < 0)
	{
		printf("Introduzca una de las opciones disponibles, por favor: ");
		scanf_s("%d", &opcion);
	}
	while (opcion <= 14  && opcion >= 1)
	{
		if (opcion == 1)
			printf("Mostrando datos...\n");
		if (opcion == 2)
			printf("Mostrando datos...\n");
		if (opcion == 3)
			printf("Mostrando datos...\n");
		if (opcion == 4)
			printf("Mostrando datos...\n");
		if (opcion == 5)
			printf("Mostrando datos...\n");
		if (opcion == 6)
			printf("Mostrando datos...\n");
		if (opcion == 7)
			printf("Mostrando datos...\n");
		if (opcion == 8)
			printf("Mostrando datos...\n");
		if (opcion == 9)
			printf("Mostrando datos...\n");
		if (opcion == 10)
			printf("Mostrando datos...\n");
		if (opcion == 11)
			printf("Mostrando datos...\n");
		if (opcion == 12)
			printf("Mostrando datos...\n");
		if (opcion == 13)
			printf("Mostrando datos...\n");
		if (opcion == 14)
			printf("Mostrando datos...\n");


		printf("   Introduzca otra opción: ");
		scanf_s("%d", &opcion);
		while (opcion > 14 || opcion < 0)
		{
			printf("   Introduzca una de las opciones disponibles, por favor: ");
			scanf_s("%d", &opcion);
		}
	}

	if (opcion == 0)
		printf("Fin del programa");
}
