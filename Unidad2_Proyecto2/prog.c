#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>


typedef struct Persona{
  int edad;
  char nombre [20];
  char ocupacion [20];
}Persona;

static int longitud = 0;
static char *argumentos1;
static char *argumentos2;

void * inverso(void* var){

  Persona *datohilo1 = (Persona *)var;

  FILE * salida1 = fopen(argumentos1, "w");

  if(salida1 == NULL){
    perror("Error en la apertura del archivo");
  }

  for(int i = longitud-1 ; i >= 0; i--){
    fprintf(salida1, "%s %s %d\n", (datohilo1+i)->nombre, (datohilo1+i)->ocupacion,(datohilo1+i)->edad);
  }

  fclose(salida1);

  return NULL;

}

void * ocupacionOrdenado(void* var){

  Persona *datohilo2 = (Persona *)var;
  Persona copia[longitud];
  Persona auxiliar;

  for(int i=0;i<longitud;i++){
    copia[i] = *(datohilo2+i);
  }

  FILE * salida2 = fopen(argumentos2, "w");

  if(salida2 == NULL){
    perror("Error en la apertura del archivo");
  }

  for(int i=0;i<longitud;i++){

    for(int j=0;j<longitud-1;j++){

      	int k=0;
      	while(1){

        	if(copia[j].ocupacion[k] > copia[j+1].ocupacion[k]){

          		auxiliar=copia[j];
          		copia[j]=copia[j+1];
          		copia[j+1]=auxiliar;

          	  break;
        	}
        	else if(copia[j].ocupacion[k] == copia[j+1].ocupacion[k]) k++;

        	else break;
      }
    }
  }

  for(int i = 0; i < longitud; i++){

    fprintf(salida2,"%s %s %d\n",copia[i].nombre, copia[i].ocupacion,copia[i].edad);

  }

  fclose(salida2);

  return NULL;

}

int main(int argc, char *argv[]) {
  
  char nombre[20], ocupacion[20];
  int edad;
  pthread_t hilo1, hilo2;
  Persona dato[100];
  char linea[200];

  argumentos1 = argv[2];
  argumentos2 = argv[3];

  FILE * arch_entrada = fopen(argv[1], "r");

  if(arch_entrada == NULL){
    perror("Error en la apertura del archivo");
    return 1;
  }
  else{}

  while(feof(arch_entrada) == 0){
    fscanf(arch_entrada, "%s %s %d", nombre, ocupacion,  &edad);
    longitud = longitud + 1;
  }

  rewind(arch_entrada);

  int n = 0;
  while(1){
		if(fgets(linea,sizeof(linea),arch_entrada)==NULL){
			fclose(arch_entrada);
			break;
		}
		sscanf(linea,"%s %s %d", (char *)dato[n].nombre, (char *)dato[n].ocupacion, &dato[n].edad);
		n++;
	}

  printf("\nInformacion del archivo: \n\n");
  for(int i = 0; i < longitud; i++){
    printf("%s %s %d\n", (dato+i)->nombre, (dato+i)->ocupacion, (dato+i)->edad);
  }
  printf("\n");

  pthread_create( &hilo1, NULL, &inverso, dato);
  pthread_create( &hilo2, NULL, &ocupacionOrdenado, dato);

  pthread_join ( hilo1 , NULL ) ;
  pthread_join ( hilo2 , NULL ) ;


  FILE * archHilo1 = fopen(argv[2], "r");
  printf("\nInformacion del archivo Hilo 1\n\n");
  while(feof(archHilo1) == 0){
    fscanf(archHilo1, "%s %s %d", nombre, ocupacion,  &edad);
    printf("%s %s %d\n", nombre, ocupacion, edad);
  }

  fclose(archHilo1);

  FILE * archHilo2 = fopen(argv[3], "r");
  printf("\nInformacion del archivo Hilo 2\n\n");
  while(feof(archHilo2) == 0){
    fscanf(archHilo2, "%s %s %d", nombre, ocupacion,  &edad);
    printf("%s %s %d\n", nombre, ocupacion, edad);
  }

  fclose(archHilo2);

  fclose(arch_entrada);
  printf("\nfin\n");

}


