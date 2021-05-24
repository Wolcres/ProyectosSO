#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {

  char clave[20];
  int valor, status1, status2;
  pid_t pid1, pid2;

  if ((pid1=fork()) == 0 ){
    execv("./exep1", argv);
  }
  else{
    if ((pid2=fork()) == 0 ){
      execv("./exep2", argv);
    }
    else{
      waitpid(pid1, &status1, 0);
      waitpid(pid2, &status2, 0);
    }
  }

  FILE * arch_Ordenado = fopen(argv[2], "r");
  FILE * arch_Reves = fopen(argv[3], "r");

  if(arch_Ordenado == NULL || arch_Reves == NULL){
    perror("Error en la apertura del archivo");
  }
  else{}

  printf("\n\n");
  printf("Archivo con datos ordenados:\n\n");
  while(feof(arch_Ordenado) == 0){
    fscanf(arch_Ordenado, "%s %d", clave, &valor);
    printf("%s %d\n", clave, valor);
  }

  printf("\n\n");
  printf("Archivo con datos alreves:\n\n");
  while(feof(arch_Reves) == 0){
    fscanf(arch_Reves, "%s %d", clave, &valor);
    printf("%s %d\n", clave, valor);
  }

  printf("\n\n");
  fclose(arch_Reves);
  fclose(arch_Ordenado);
  printf("Fin\n");

}