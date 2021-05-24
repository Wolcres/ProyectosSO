# include <stdio.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <string.h>
# include <pthread.h>
# include <unistd.h>
# include <sys/types.h>
# include <semaphore.h>

sem_t *semaforo1;
sem_t *semaforo2;
sem_t *semaforo3;
sem_t *semaforo4;

#define SH_SIZE 100

struct mensaje{
    char escribir[SH_SIZE];
    char recibir [SH_SIZE];
};

struct mensaje *chat;

void *EnviarMensaje(void *parametro){

    while(1){
        if(fgets(chat->recibir, SH_SIZE, stdin) != NULL){
            sem_post(semaforo2);
            sem_wait(semaforo4);
            if(strcasecmp(chat->recibir, "end\n") == 0) break;
        }

    }

    return NULL;

}

void *RecibirMensaje(void *parametro){

    while (1){
        sem_wait(semaforo1);
        printf("Mensaje recibido: %s", chat->escribir);
        sem_post(semaforo3);
        if(strcasecmp(chat->escribir, "end\n") == 0) break;
    }

    return NULL;

}


int main(void){

    int shm_fd = shm_open("compartido", O_RDWR, 0666);
    chat = (struct mensaje *) mmap(NULL, SH_SIZE * 2, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);

    semaforo1 = sem_open("Semaforo1", 0);
    semaforo2 = sem_open("Semaforo2", 0);
    semaforo3 = sem_open("Semaforo3", 0);
    semaforo4 = sem_open("Semaforo4", 0);

    pthread_t hiloEnviar, hiloRecibir;

    pthread_create(&hiloEnviar, NULL, &EnviarMensaje, NULL);
    pthread_create(&hiloRecibir, NULL, &RecibirMensaje, NULL);

    pthread_join(hiloEnviar, NULL);
    pthread_join(hiloRecibir, NULL);

    munmap(chat, SH_SIZE);
    close(shm_fd);

    sem_close(semaforo1);
    sem_close(semaforo2);
    sem_close(semaforo3);
    sem_close(semaforo4);

    return 0;

}