#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
int main(void)
{
	setlocale(LC_ALL, "");
	int opcion;

	printf("   Sistema automático de control de invernadero");
	printf("\n   ===========================================");
	printf("\n\n\t1 - Visualizar registro de TEMPERATURA");
	printf("\n\t2 - Visualizar registro de HUMEDAD");
	printf("\n\t3 - Visualizar registro de LUMINOSIDAD");
	printf("\n\t4 - Historial cronológico");
	printf("\n\t5 - Gráficas");
	printf("\n\t6 - Salir de la aplicación");
	printf("\n\n   Introduzca una opción: ");
	scanf_s("%d", &opcion);


	while (opcion > 6 || opcion < 1)
	{
		printf("Introduzca una de las opciones disponibles, por favor: ");
		scanf_s("%d", &opcion);
	}
	while (opcion < 6 && opcion >= 1)
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

		printf("Introduzca otra opción: ");
		scanf_s("%d", &opcion);
		while (opcion > 6 || opcion < 1)
		{
			printf("Introduzca una de las opciones disponibles, por favor: ");
			scanf_s("%d", &opcion);
		}
	}
	
	if (opcion == 6)
		printf("Fin del programa");
}