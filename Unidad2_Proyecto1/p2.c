#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Dato{
  int valor;
  char clave [20];
}Dato;

int main(int argc, char *argv[]) {

  char clave[20];
  int longitud = 0, valor;
  Dato *par;
  FILE * arch_entrada = fopen(argv[1], "r");
  FILE * arch_salida = fopen(argv[3], "w");

  if(arch_entrada == NULL || arch_salida == NULL){
    perror("Error en la apertura del archivo");
    return 1;
  }
  else{}

  while(feof(arch_entrada) == 0){
    fscanf(arch_entrada, "%s %d", clave, &valor);
    longitud = longitud + 1;
  }

  par = (Dato *)malloc(sizeof(Dato)*longitud);

  rewind(arch_entrada);

  for(int i = 0; i < longitud; i++){
    fscanf(arch_entrada, "%s %d", clave, &valor);
    strncpy((par+i)->clave, clave , 20);
    (par+i)->valor = valor;
  }

  for(int i = longitud-1 ; i >= 0; i--){
    fprintf(arch_salida, "%s %d\n", (par+i)->clave, (par+i)->valor);
  }

  free(par);
  fflush(arch_salida);
  fclose(arch_salida);
  fclose(arch_entrada);
  exit(EXIT_SUCCESS);

  return 0;

}