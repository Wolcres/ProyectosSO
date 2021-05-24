#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){

  int padre[2], hijo[2], status;
  char mensajeP[100], mensajeH[100], opcion, capta;
  pid_t pid;

  if(pipe(padre) == -1){
    perror("Error tuberia padre: ");
    exit(EXIT_FAILURE);
  }

  if(pipe(hijo) == -1){
    perror("Error tuberia hijo: ");
    exit(EXIT_FAILURE);
  }

  printf("Ingrese el mensaje: \n");
  fgets(mensajeP,100,stdin);
  for(int indice = 0; mensajeP[indice] != '\0'; indice++){
    mensajeP[indice] = tolower(mensajeP[indice]);
  }

  write(padre[1], mensajeP, 100);
  pid = fork();

  if(pid == 0){
    close(padre[1]);
    close(hijo[0]);
  }

  if(pid != 0){
    close(padre[0]);
    close(hijo[1]);
  }

  while(1){

    switch (pid)
    {
      case -1:
        perror("Error en proceso hijo: ");
        exit(EXIT_FAILURE);
      break;
        
      case 0:
        read(padre[0], mensajeH, 100);
        for(int indice = 0; mensajeH[indice] != '\0'; indice++){
           mensajeH[indice] = toupper(mensajeH[indice]);
        }       
        write(hijo[1], mensajeH, 100);
      break;
        
      default:
        read(hijo[0], mensajeP, 100);
        printf("%s\n", mensajeP);
      break;
    }

    if(pid != 0){
      printf("¿Desea continuar? S/n\n");
      scanf("%c",&opcion);
      scanf("%c",&capta);
      if(opcion == 'n' || opcion == 'N'){
        break;
      }
    }
        
    if(pid != 0){
      printf("\nIngrese el mensaje: \n");
      fgets(mensajeP,100,stdin);
      for(int indice = 0; mensajeP[indice] != '\0'; indice++){
        mensajeP[indice] = tolower(mensajeP[indice]);
      }       
      write(padre[1], mensajeP, 100);
    }

  }

  if(pid == 0){
    close(padre[0]);
    close(hijo[1]);
    waitpid(pid, &status, 0);
  }

  if(pid != 0){
    close(padre[1]);
    close(hijo[0]);
  }

  return 0;
}