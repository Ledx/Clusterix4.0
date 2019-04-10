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
int REPETICIONES, ITERACIONES, L, *histograma, N, ACTIVAR_GRAFICOS;
float E, ALFA_POTENCIAL, A_POTENCIAL, R, DELTA_AVANCE, DELTA_ROTACION, MEDIA_RUIDO, SIGMA_POTENCIAL, SIGMA_ROTACION, SIGMA_AVANCE,
R_COLISION, *w;
/*----------------------------------------------------------------------------*/
/*Estructuras           													  */
/*----------------------------------------------------------------------------*/

struct robot {
	double thetha;
	float radio;
	float constante_potencial;
	float posicion[2];
	double thethaInicial = 0;
	float pasos = 0;
	int clustered = 0;
	int *vecindad;
	int clase = 1;
	float tiempo = 0;
	float distancia_recorrida;
	float tiempo_estabilidad = 0;
	float carga;
	float *giro;
}*robots;

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
int Inicializacion(FILE *archivo);
void CargaParametros(FILE *archivo);
void RotacionAvance(int i);
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
/*Funciones principales														  */
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
		status = Inicializacion(archivo[4]);
		if (status == 1) {
			return 0;
		}
	}

	while (salir1 < REPETICIONES) {
		acomodo(archivo[0]);
		while (salir2 < ITERACIONES && (Exito() != 0)) {

			for (i = 0; i < N; i++) {
				fprintf(archivo[1], "%.2f %.2f %.2f ", robots[i].posicion[0], robots[i].posicion[1], robots[i].thetha);
			}
			fprintf(archivo[1], "\n");

			imprimirTablero(salir1);
			espera(500);
			system("Cls");

			SumaMediaDistancias(archivo[3]);

			for (i = 0; i < N; i++) {
				for (j = 0; j < N; j++) {
					robots[i].giro[j] = potencial(i, j + 1);
				}
			}

			for (i = 0; i < N; i++) {
				RotacionAvance(i);
			}
			cluster();

			salir2++;

		}
		Clases(archivo[2]);

		for (k = 0; k < N; k++) {
			fprintf(archivo[0], "%.2f %.2f %.2f  ", robots[k].posicion[0], robots[k].posicion[1], robots[k].thetha);
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

/*----------------------------------------------------------------------------*/
/*Funciones secundarias														  */
/*----------------------------------------------------------------------------*/

void espera(int mseg) {
	clock_t t = mseg + clock();
	while (t > clock());
}

void acomodo(FILE *archivo) {
	int i, j;

	for (i = 0; i < N; i++) {
		for (j = 0; j < 2; j++) {
			/*Lado de una region de borde cuadrado de dimension LxL*/
			robots[i].posicion[j] = rand() % L;
		}
		/*Orientacion del robot en el sistema de coordenadas del laboratorio*/
		robots[i].thetha = rand() % 360;

	}
	for (i = 0; i < N; i++) {

		fprintf(archivo, "%.2f %.2f %.2f  ", robots[i].posicion[0], robots[i].posicion[1], robots[i].thetha);
	}
}

void BarridoFrecuencias(int i) {
	int j;

	for (j = 0; j < N; j++) {
		if (robots[j].clase > robots[i].clase) {
			robots[i].clustered = 0;
			break;
		}
	}
}
void RotacionAvance(int i) {

	float thetaG, pasoG, wP = 0;
	int j;
	if (robots[i].clase > 1) {
		BarridoFrecuencias(i);
		for (j = 0; j < N; j++) {

			if (j > (robots[i].clase - 1)) {
				//robots[i].thethaP = robots[i].thethaP + (w[j] * robots[i].giro[j]);
				wP = w[j] + wP;
			}

		}
		//robots[i].thethaP = robots[i].thethaP / wP;
	}
	else {
		//robots[i].thethaP = robots[i].giro[0];
	}

	if (robots[i].clustered == 0) {
		/*Version que patina*/
		//////////////////////////////////////////////////////////////////
		//thetaG = distribucionNormal(robots[i].thethaP*(180 / PI), desv, 0);
		//////////////////////////////////////////////////////////////////
		pasoG = distribucionNormal(DELTA_AVANCE, SIGMA_AVANCE, 1);
		/*Sistema de coordenadas del laboratorio*/
		robots[i].thetha = robots[i].thetha - (90 - thetaG);
		robots[i].posicion[0] += pasoG * cos((robots[i].thetha*(PI / 180)));
		robots[i].posicion[1] += pasoG * sin((robots[i].thetha*(PI / 180)));
		//Version que no patina
		/*e[i].thetha = e[i].thetha - ((1.5707963267949 - e[i].giro)*(180/PI));
		e[i].posicion[0] += PASO * cos((e[i].thetha*(PI/180)));
		e[i].posicion[1] += PASO * sin((e[i].thetha*(PI / 180)));*/
		for (j = 0; j < N; j++) {
			//robots[i].thethaP = 0;
		}
		robots[i].pasos += 1;
	}

}

float potencial(float *thetha) {

	int i;
	float potencial, d, potencialx = 0, potencialy = 0, phi = 0, alfa = 0, gama = 0, dx, dy;

	potencial = (A_POTENCIAL * (LobuloCos(abs(thetha[0] - thetha[1]))) / (norm(x1 - x2) ^ ALFA_POTENCIAL) + normrnd(MEDIA_RUIDO, SIGMA_POTENCIAL));
	return potencial;

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
		robots[j].thetha = 0;
		robots[j].clustered = 0;
		robots[j].pasos = 0;
		robots[j].posicion[0] = 0;
		robots[j].posicion[1] = 0;
		for (i = 0; i < N; i++) {
			robots[j].vecindad[i] = 0;
			robots[j].giro[i] = 0;
		}

	}
}
void cluster() {
	int j, i;

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			if (i != j) {
				if (distancia(robots[i].posicion, robots[j].posicion) < (2 * R)) {
					robots[i].clustered = 1;
					robots[j].clustered = 1;
					robots[i].vecindad[j] = 1;
					robots[j].vecindad[i] = 1;
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
			vecindades[i][j] = robots[i].vecindad[j];
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
				robots[m].clase = k;
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
			suma[i] += distancia(robots[i].posicion, robots[j].posicion);

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
		if (robots[i].clustered == 0) {
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
		gotoxy(robots[i].posicion[0] / 10, robots[i].posicion[1] / 10);
		printf("%d", i + 1);
	}
}

void CargaParametros(FILE *archivo) {
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
	ALFA_POTENCIAL = parametros[4];
	DELTA_AVANCE = parametros[5] * R;
	REPETICIONES = (int)parametros[6];
	ITERACIONES = (int)parametros[7];
	SIGMA_ROTACION = parametros[8];
	SIGMA_POTENCIAL = parametros[9];
	w = (float*)malloc(N * sizeof(float*));

	///////////////////////////////////A_POTENCIAL, R,  DELTA_ROTACION, MEDIA_RUIDO,  R_COLISION

	for (i = 0; i <= N; i++) {
		w[i] = parametros[(NP - N) + i];
	}

}

int Inicializacion(FILE *archivo) {
	int i, j, status = 0;
	time_t t;

	/*Semilla de tiempo*/
	srand((unsigned)time(NULL));
	if (archivo != NULL) {
		CargaParametros(archivo);
		fclose(archivo);
		status = 0;
	}
	else {
		printf("No se cuenta con archivo de parametros, presiona 1 para salir");
		scanf("%d", &status);
	}
	histograma = (int*)malloc(N * sizeof(int*));
	robots = (robot*)malloc(N * sizeof(robot));
	for (i = 0; i < N; i++) {
		histograma[i] = 0;
		robots[i].vecindad = (int*)malloc(N * sizeof(int*));
		robots[i].clustered = 0;
		robots[i].pasos = 0;
		robots[i].clase = 1;
		robots[i].giro = (float*)malloc(N * sizeof(float*));

		/*Radio del robot*/
		robots[i].radio = R;
		for (j = 0; j < N; j++) {
			robots[i].vecindad[j] = 0;
			robots[i].giro[j] = 0;
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

float LobuloCos(float thetha) {
	/*Funcion que calcula la atenuación con la que un robot percibe a otro*/

	float a;
	if (thetha < 0) {
		while (thetha < 0)
			thetha = thetha + (2 * PI);
	}

	if (thetha > (2 * PI)) {
		while (thetha > (2 * PI))
			thetha = thetha - (2 * PI);
	}


	if (thetha >= 0 && thetha <= ((70 * PI) / 180)){
		a = cos(thetha)*cos(thetha)*cos(thetha);
	}
		
	else
		a = abs((long)cos(3 * thetha)) / 3;

	return a;

}