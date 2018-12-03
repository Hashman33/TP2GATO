#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "strutil.h"
#include "abb.h"
#include "hash.h"
#include "heap.h"

void borrar (char* tinicial, char* tfinal);//Formato AAAA-MM-DDTHH:MM:SS
void prioridad_vuelos (size_t nvuelo, heap_t* heap);
void info_vuelo(size_t nvuelo, hash_t* hash);
void ver_tablero(size_t lineq, char* orden, char* tinicial, char* tfinal, abb_t* abb);//Formato AAAA-MM-DDTHH:MM:SS
void agregar_archivo (const char * nombre, hash_t* hash, abb_t* abb, heap_t* heap);



int cmpabb (void * a, void * b){
	char * a1 = a;
	char * b1 = b; 
	return strcmp(a1,b1);
}

int cmpheap(void *a, void *b){
	vuelo_t * vuelo1 = a;
	vuelo_t * vuelo2 = b;
	if(vuelo1->prioridad > vuelo2->prioridad) return 1;
	if(vuelo1->prioridad < vuelo2->prioridad) return -1;
	return 0;
}


size_t splen (char ** strv){
	size_t cant = 0;
	while(strv[cant]) cant++;
	return cant;
}

typedef struct vuelo{
	char * n_vuelo;
	int prioridad;
}vuelo_t;

vuelo_t * vuelo_crear(char * prioridad, char * n_vuelo ){
	vuelo_t * vuelo = malloc(sizeof(vuelo_t));
	vuelo->n_vuelo = n_vuelo;
	vuelo->prioridad = atoi(prioridad);
	return vuelo;
}

void agregar_archivo (const char * nombre, hash_t* hash, abb_t* abb, heap_t* heap){
	FILE* archivo = fopen(nombre, "rt");
	if (!archivo){
		fprintf(stderr, "ERROR\n");
		return;
	}
	char* buffer = NULL; size_t cap = 0;
	while(getline(&buffer, &cap, archivo)>0){
		char ** strv = split(buffer, ',');
		if (splen(strv) != 10){
			fprintf(stderr, "ERROR\n");
			continue;
		}
		char * n_vuelo = malloc(sizeof(char) * strlen(strv[0]));
		char * fecha = malloc(sizeof(char) * strlen(strv[6]));
		strcpy( n_vuelo, strv[0]);
		strcpy(fecha,strv[6]);
		hash_guardar(hash, n_vuelo, join(strv, ' '));
		abb_guardar(abb, fecha, n_vuelo);
		vuelo_t* vuelo = vuelo_crear(strv[5], n_vuelo);
		heap_encolar(heap, vuelo);
		free_strv(strv);
	}
}

void info_vuelo(char * nvuelo, hash_t* hash){
	void * vuelo = hash_obtener(nvuelo);
	char * vuelo1= vuelo;
	fprintf(stdout, "%s\n", vuelo1);
}

void prioridad_vuelos (int n_vuelos, heap_t* heap){
	void ** vuelos = malloc(sizeof(void*) * n_vuelos)
	for (int i = 0; i < n_vuelos; i++){
		vuelo_t * vuelo = heap_desencolar(heap);
		vuelos[i] = vuelo;
		printf("%d - &s\n", vuelo->prioridad, vuelo->n_vuelo );
	}
	for(int i = 0 ; i < n_vuelos; i++){
		heap_encolar(heap,vuelos[i]);
	}
}

void ver_tablero(size_t lineq, char* orden, char* tinicial, char* tfinal, abb_t* abb){

}	

void borrar (char* tinicial, char* tfinal){

}

void interfaz(){
	heap_t* heap = heap_crear(cmpheap) ;
	abb_t* abb = abb_crear(cmpabb);
	hash_t* hash = hash_crear();
	char* buffer = NULL; size_t cap = 0;
	while(getline(&buffer, &cap, stdin)>0){
		strtok(buffer, "\n");
		char ** strv = split(buffer, ' ');
		size_t splen = splen(strv);
		if (!strcmp(strv[0], "agregar_archivo")&&(splen == 2)){
			agregar_archivo(strv[1], hash, abb, heap);
			continue;
		}
		if (!strcmp(strv[0], "ver_tablero")&&(splen == 5)){
			ver_tablero(strv[1], strv[2], strv[3], strv[4], abb);
			continue;
		}
		if (!strcmp(strv[0], "informacion_vuelo")&&(splen == 2)){
			info_vuelo(strv[1], hash);
			continue;
		}
		if (!strcmp(strv[0], "prioridad_vuelos")&&(splen == 2)){
			int k_vuelos = atoi(strv[1]);
			prioridad_vuelos(k_vuelos, heap);
			continue;
		}
		if (!strcmp(strv[0], "borrar")&&(splen == 3)){
			borrar(strv[1], strv[2]);
			continue;
		}
		fprintf(stderr, "ERROR\n");
		free_strv(strv);
		free(buffer);
		char* buffer = NULL; size_t cap = 0;
	}
}

int main (){
	interfaz();
	return 0;
}


