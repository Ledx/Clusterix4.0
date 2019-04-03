/******************************************************************************/
/* Universidad Nacional Autónoma de México                                    */
/* Instituto de Ciencias Nucleares                                            */
/*----------------------------------------------------------------------------*/
/* Programa: COLMENA									                      */
/* Director: Dr. Gustavo Adolfo Medina Tanco			                      */
/*----------------------------------------------------------------------------*/
/* Simulador de formacion de clusters en elementos dispersos                  */
/*----------------------------------------------------------------------------*/
/* Desarrollador: Ing. Led Eduardo Ruvalcaba Tamés			                  */
/*----------------------------------------------------------------------------*/
/* Actualizado 19/03/19 1000 hrs UTC-6                                        */
/******************************************************************************/

/*----------------------------------------------------------------------------*/
/*Cabeceras generales														  */
/*----------------------------------------------------------------------------*/
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <windows.h>

/*----------------------------------------------------------------------------*/
/*Constantes simbolicas														  */
/*----------------------------------------------------------------------------*/
#define PI 3.14159265358979323846
#define EU 2.7182818284590452354
#define NP 19
/*----------------------------------------------------------------------------*/
/*Variables globales    													  */
/*----------------------------------------------------------------------------*/
int REPETICIONES, TMAX, L, *histograma, N;
float E, ALFA, R, PASO, desv, fd, *w;
/*----------------------------------------------------------------------------*/
/*Estructuras           													  */
/*----------------------------------------------------------------------------*/

struct robot {
	double thetha;
	double thethaP = 0;
	float *giro;
	float radio;
	float posicion[2];
	float pasos = 0;
	float carga;
	int clustered = 0;
	int *vecindad;
	float *potenciales;
	int clase = 1;
}*e;

struct nodoP {
	int valor;
	struct nodoP *sig;
};
struct nodoL {
	robot robot;
	struct nodoL *sig;
};
/*----------------------------------------------------------------------------*/
/*Prototipos de Funciones  													  */
/*----------------------------------------------------------------------------*/

void acomodo(FILE *archivo);
int inicializacion(FILE *archivo);
void LecturaParametros(FILE *archivo);
void movimiento(int i);
float potencial(int i, int clase);
void limpiador();
float distancia(float a[], float b[]);
void BarridoFrecuencias(int i);
void cluster();
void SumaMediaDistancias(FILE *archivo);
int Exito();
void Clases(FILE *archivo);
void gotoxy(float x, float y);
void imprimirTablero(int Nexperimento);
void espera(int mseg);
struct nodoP* push(struct nodoP *primero, int valortemp);
struct nodoP* pop(struct nodoP *primero, int *resultado);
struct nodoL* NuevoElementoLista(struct nodoL *primero, robot robot);
float distribucionNormal(float media, float var, float restriccion);

/*----------------------------------------------------------------------------*/
/*Definicion de Funciones  													  */
/*----------------------------------------------------------------------------*/

int main(int argc, char *argv[]) {

	int salir2 = 0;
	int i, j, k, salir1 = 0, status = 2;
	FILE *archivo[6];

	archivo[0] = fopen("posicionesIni-Fin.txt", "w");
	archivo[1] = fopen("posicionesTotales.txt", "w");
	archivo[2] = fopen("Clusters.txt", "w");
	archivo[3] = fopen("SumaMediaDistancias.txt", "w");
	archivo[4] = fopen("parametrosIniciales.txt", "r");
	archivo[5] = fopen("histograma.txt", "w");

	while (status) {
		status = inicializacion(archivo[4]);
		if (status == 1) {
			return 0;
		}
	}

	while (salir1 < REPETICIONES) {
		acomodo(archivo[0]);
		while (salir2 < TMAX && (Exito() != 0)) {

			for (i = 0; i < N; i++) {
				fprintf(archivo[1], "%.2f %.2f %.2f ", e[i].posicion[0], e[i].posicion[1], e[i].thetha);
			}
			fprintf(archivo[1], "\n");

			imprimirTablero(salir1);
			espera(500);
			system("Cls");

			SumaMediaDistancias(archivo[3]);

			for (i = 0; i < N; i++) {
				for (j = 0; j < N; j++) {
					e[i].giro[j] = potencial(i, j + 1);
				}
			}

			for (i = 0; i < N; i++) {
				movimiento(i);
			}
			cluster();

			salir2++;

		}
		Clases(archivo[2]);

		for (k = 0; k < N; k++) {
			fprintf(archivo[0], "%.2f %.2f %.2f  ", e[k].posicion[0], e[k].posicion[1], e[k].thetha);
		}

		limpiador();
		fprintf(archivo[0], "\n");
		fprintf(archivo[1], "\n");
		fprintf(archivo[2], "\n");
		fprintf(archivo[3], "\n");

		salir1++;
		salir2 = 0;
	}
	for (i = 0; i < N; i++) {
		fprintf(archivo[5], "%d ", histograma[i]);
	}
	fprintf(archivo[5], "\n\n Generado para %d realizaciones ", REPETICIONES);

	fclose(archivo[0]);
	fclose(archivo[1]);
	fclose(archivo[2]);
	fclose(archivo[3]);
	fclose(archivo[5]);
	free(histograma);

	return 0;
}

void espera(int mseg) {
	clock_t t = mseg + clock();
	while (t > clock());
}

void acomodo(FILE *archivo) {
	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < 2; j++) {
			/*Lado de una region de borde cuadrado de dimension LxL*/
			e[i].posicion[j] = rand() % L;
		}
		/*Orientacion del robot en el sistema de coordenadas del laboratorio*/
		e[i].thetha = rand() % 360;

	}
	for (i = 0; i < N; i++) {

		fprintf(archivo, "%.2f %.2f %.2f  ", e[i].posicion[0], e[i].posicion[1], e[i].thetha);
	}
}

void BarridoFrecuencias(int i) {
	int j;

	for (j = 0; j < N; j++) {
		if (e[j].clase > e[i].clase) {
			e[i].clustered = 0;
			break;
		}
	}
}
void movimiento(int i) {

	float thetaG, pasoG, wP = 0;
	int j;
	if (e[i].clase > 1) {
		BarridoFrecuencias(i);
		for (j = 0; j < N; j++) {

			if (j > (e[i].clase - 1)) {
				e[i].thethaP = e[i].thethaP + (w[j] * e[i].giro[j]);
				wP = w[j] + wP;
			}

		}
		e[i].thethaP = e[i].thethaP / wP;
	}
	else {
		e[i].thethaP = e[i].giro[0];
	}

	if (e[i].clustered == 0) {
		/*Version que patina*/
		//////////////////////////////////////////////////////////////////
		thetaG = distribucionNormal(e[i].thethaP*(180 / PI), desv, 0);
		//////////////////////////////////////////////////////////////////
		pasoG = distribucionNormal(PASO, fd*PASO, 1);
		/*Sistema de coordenadas del laboratorio*/
		e[i].thetha = e[i].thetha - (90 - thetaG);
		e[i].posicion[0] += pasoG * cos((e[i].thetha*(PI / 180)));
		e[i].posicion[1] += pasoG * sin((e[i].thetha*(PI / 180)));
		//Version que no patina
		/*e[i].thetha = e[i].thetha - ((1.5707963267949 - e[i].giro)*(180/PI));
		e[i].posicion[0] += PASO * cos((e[i].thetha*(PI/180)));
		e[i].posicion[1] += PASO * sin((e[i].thetha*(PI / 180)));*/
		for (j = 0; j < N; j++) {
			e[i].thethaP = 0;
		}
		e[i].pasos += 1;
	}

}

float potencial(int j, int clase) {

	int i;
	float potencial, d, potencialx = 0, potencialy = 0, phi = 0, alfa = 0, gama = 0, dx, dy;

	if (clase == 1) {
		for (i = 0; i < N; i++) {
			if (j != i) {
				d = distancia(e[j].posicion, e[i].posicion);
				potencial = (E / (pow(d, ALFA)));

				dy = e[i].posicion[1] - e[j].posicion[1];
				dx = e[i].posicion[0] - e[j].posicion[0];

				phi = atan2(dy, dx);
				alfa = phi + (1.5707963267949 - (e[j].thetha*(PI / 180)));
				potencialx = potencialx + (potencial*cos(alfa));
				potencialy = potencialy + (potencial*sin(alfa));
			}
			phi = 0;
			alfa = 0;
		}
	}
	else {
		for (i = 0; i < N; i++) {
			if (j != i && e[i].clase == clase) {
				d = distancia(e[j].posicion, e[i].posicion);
				potencial = (E / (pow(d, ALFA)));

				dy = e[i].posicion[1] - e[j].posicion[1];
				dx = e[i].posicion[0] - e[j].posicion[0];

				phi = atan2(dy, dx);
				alfa = phi + (1.5707963267949 - (e[j].thetha*(PI / 180)));
				potencialx = potencialx + (potencial*cos(alfa));
				potencialy = potencialy + (potencial*sin(alfa));
			}
			phi = 0;
			alfa = 0;
		}
	}
	/*Sistema de coordenadas del robot*/
	gama = atan2(potencialy, potencialx);

	return gama;

}

float distancia(float a[], float b[])
{
	float d = 0;
	d = sqrt((pow((a[0] - b[0]), 2)) + (pow((a[1] - b[1]), 2)));
	return d;
}
void limpiador()
{
	int j, i;
	for (j = 0; j < N; j++) {
		e[j].thetha = 0;
		e[j].thethaP = 0;
		e[j].clustered = 0;
		e[j].pasos = 0;
		e[j].posicion[0] = 0;
		e[j].posicion[1] = 0;
		for (i = 0; i < N; i++) {
			e[j].vecindad[i] = 0;
			e[j].giro[i] = 0;
		}

	}
}
void cluster() {
	int j, i;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (i != j) {
				if (distancia(e[i].posicion, e[j].posicion) < (2 * R)) {
					e[i].clustered = 1;
					e[j].clustered = 1;
					e[i].vecindad[j] = 1;
					e[j].vecindad[i] = 1;
				}

			}
		}
	}

}

void Clases(FILE *archivo) {

	struct nodoP *primero = NULL;
	int valortemp = 10, resultado;
	int i, j, l, m, *cluster, **vecindades = NULL, k = 1, *vecindadTemp, *mapaClase, devuelto;

	cluster = (int*)malloc(N * sizeof(int*));
	vecindadTemp = (int*)malloc(N * sizeof(int*));
	mapaClase = (int*)malloc(N * sizeof(int*));
	vecindades = (int **)malloc(N * sizeof(int*));
	for (i = 0; i < N; i++) {
		vecindades[i] = (int*)malloc(N * sizeof(int));
	}
	for (i = 0; i < N; i++) {
		if (i > 0) {
			cluster[i] = 0;
		}
		else {
			cluster[i] = N;
		}
		vecindadTemp[i] = 0;
		mapaClase[i] = 0;
	}

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			vecindades[i][j] = e[i].vecindad[j];
		}
	}

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (vecindades[i][j] == 1 && vecindadTemp[i] != 1) {
				primero = push(primero, j);
			}
		}
		vecindadTemp[i] = 1;
		while (primero != NULL) {
			primero = pop(primero, &devuelto);
			if (vecindadTemp[devuelto] != 1) {
				vecindadTemp[devuelto] = 1;
				k++;
				for (l = 0; l < N; l++) {
					if (vecindades[devuelto][l] == 1 && vecindadTemp[l] != 1) {
						primero = push(primero, l);
					}
				}
			}
		}
		/*Determinacion de clases*/
		for (m = 0; m < N; m++) {
			if (vecindadTemp[m] == 1 && mapaClase[m] == 0) {
				mapaClase[m] = k;
				e[m].clase = k;
			}
		}

		if (k > 1) {
			cluster[k - 1]++;
			cluster[0] = cluster[0] - k;
		}

		k = 1;
	}
	for (i = 0; i < N; i++) {
		histograma[i] = histograma[i] + cluster[i];
		fprintf(archivo, "%d ", cluster[i]);
	}
}

void SumaMediaDistancias(FILE *archivo) {
	int j, i;
	float *suma, promedio = 0;
	suma = (float*)malloc(N * sizeof(float*));
	for (j = 0; j < N; j++) {
		suma[j] = 0;
	}
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			suma[i] += distancia(e[i].posicion, e[j].posicion);

		}
	}

	for (i = 0; i < N; i++) {
		promedio = promedio + suma[i];
	}
	fprintf(archivo, "%.2f\n", promedio / N);
}

int Exito() {
	int i;

	for (i = 0; i < N; i++) {
		if (e[i].clustered == 0) {
			return 1;
		}
	}
	return 0;
}

void gotoxy(float x, float y)
{
	COORD c = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
}

void imprimirTablero(int Nexperimento) {
	int i;
	printf("Experimento:%d", Nexperimento + 1);
	for (i = 0; i < N; i++) {
		gotoxy(e[i].posicion[0] / 10, e[i].posicion[1] / 10);
		printf("%d", i + 1);
	}
}

void LecturaParametros(FILE *archivo) {
	int i = 0, j = 0, z;
	char buffer[100], cadena[NP][100], c = '\n', d = '#', parametro[25];
	double parametros[NP];

	for (z = 0; z < 25; z++) {
		parametro[z] = ' ';
	}

	while (i < NP) {

		while (d == '#') {
			fgets(cadena[i], 100, archivo);
			d = cadena[i][0];
		}

		while (c != '#') {
			c = cadena[i][j];
			parametro[j] = c;
			j++;
		}

		parametros[i] = (float)strtod(parametro, NULL);

		for (z = 0; z < 25; z++)
			parametro[z] = ' ';
		i++;
		j = 0;
		c = '\n';
		d = '#';
	}

	N = parametros[0];
	R = parametros[1];
	L = (int)parametros[2] * R;
	E = parametros[3];
	ALFA = parametros[4];
	PASO = parametros[5] * R;
	REPETICIONES = (int)parametros[6];
	TMAX = (int)parametros[7];
	desv = parametros[8];
	fd = parametros[9];
	w = (float*)malloc(N * sizeof(float*));

	for (i = 0; i <= N; i++) {
		w[i] = parametros[(NP - N) + i];
	}

}

int inicializacion(FILE *archivo) {
	int i, j, status = 0;
	time_t t;

	/*Semilla de tiempo*/
	srand((unsigned)time(NULL));
	if (archivo != NULL) {
		LecturaParametros(archivo);
		fclose(archivo);
		status = 0;
	}
	else {
		printf("No se cuenta con archivo de parametros, presiona 1 para salir");
		scanf("%d", &status);
	}
	histograma = (int*)malloc(N * sizeof(int*));
	e = (robot*)malloc(N * sizeof(robot));
	for (i = 0; i < N; i++) {
		histograma[i] = 0;
		e[i].vecindad = (int*)malloc(N * sizeof(int*));
		e[i].potenciales = (float*)malloc(N * sizeof(float*));
		e[i].clustered = 0;
		e[i].pasos = 0;
		e[i].clase = 1;
		e[i].thethaP = 0;
		e[i].giro = (float*)malloc(N * sizeof(float*));

		/*Radio del robot*/
		e[i].radio = R;
		for (j = 0; j < N; j++) {
			e[i].vecindad[j] = 0;
			e[i].potenciales[j] = 0;
			e[i].giro[j] = 0;
		}
	}
	return status;
}

struct nodoP* push(struct nodoP *primero, int valortemp) {
	struct nodoP *temp, *nuevo;
	if (primero == NULL)
	{
		primero = (struct nodoP *)malloc(sizeof(struct nodoP));
		primero->valor = valortemp;
		primero->sig = NULL;
	}
	else
	{
		temp = primero;
		nuevo = (struct nodoP *)malloc(sizeof(struct nodoP));
		nuevo->valor = valortemp;
		nuevo->sig = temp;
		primero = nuevo;
	}
	return primero;
}

struct nodoP* pop(struct nodoP *primero, int *resultado) {
	if (primero != NULL) {

		*resultado = primero->valor;
		primero = primero->sig;
	}
	return primero;
}

struct nodoL* NuevoElementoLista(struct nodoL *primero, robot r) {
	struct nodoL *temp, *nuevo;
	if (primero == NULL)
	{
		primero = (struct nodoL *)malloc(sizeof(struct nodoL));
		primero->robot = r;
		primero->sig = NULL;
	}
	else
	{
		temp = primero;
		while (temp->sig != NULL)
		{
			temp = temp->sig;
		}
		nuevo = (struct nodoL *)malloc(sizeof(struct nodoL));
		nuevo->robot = r;
		nuevo->sig = NULL;
		temp->sig = nuevo;
	}
	return primero;
}
float distribucionNormal(float media, float sigma, float restriccion) {
	int i, k, ndat = 1000;
	float y, aux = 0, dy;

	for (k = 1; k <= ndat; k++) {
		aux = aux + (float)rand() / RAND_MAX;
	}
	if (restriccion != 0) {
		y = sigma * sqrt(12.0 / ndat)*(aux - (float)ndat / 2) + media;
		if (y > media) {
			dy = abs((long)(media - y));
			y = media - dy;
		}
	}
	else {
		y = sigma * sqrt(12.0 / ndat)*(aux - (float)ndat / 2) + media;
	}
	return y;
}