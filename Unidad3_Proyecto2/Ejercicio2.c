# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <unistd.h>
# include <errno.h>
# include <sys/types.h>
# include <sys/ipc.h>
# include <sys/msg.h>

#define Permisos 0666

struct my_msgbuf {
    long mtype;
    char mtext[100];
};

struct my_msgbuf msn[2];
int msgID;

void *EnviarMensaje(void *parametro){

    int longitud;
    msn[0].mtype = 1;
    while(fgets(msn[0].mtext, sizeof(msn[0].mtext), stdin) != NULL){
        longitud = strlen(msn[0].mtext);
        if(msn[0].mtext[longitud - 1] == '\n'){
            msn[0].mtext[longitud - 1] = '\0';
        }

        if(msgsnd(msgID, &msn[0], longitud + 1, 0) == -1){
            perror("error en msgsnd");
        }
    }
    strcpy(msn[0].mtext, "end");
    longitud = strlen(msn[0].mtext);
    if(msgsnd(msgID, &msn[0], longitud + 1, 0) == -1){
        perror("error en msgsnd #2");
    }
    
    return NULL;

}

void *RecibirMensaje(void *parametro){

    while(1){
        if(msgrcv(msgID, &msn[1], sizeof(msn[1].mtext), 2, 0) == -1){
            perror("error en msgrcv");
        }
        printf("Menseaje recibido: %s \n", msn[1].mtext);

        if(strcmp(msn[1].mtext, "end") == 0){
            break;
        }
    }

    return NULL;
    
}

int main(void){

    key_t llave;
    pthread_t hiloEnviar, hiloRecibir;

    system("touch chat.txt");
    if((llave = ftok("chat.txt", 'i')) == -1){
        perror("Error en ftok");
        return 1;
    }

    if((msgID = msgget(llave, Permisos | IPC_CREAT)) == -1){
        perror("error en mdgget");
        return 1;
    }

    pthread_create(&hiloEnviar, NULL, &EnviarMensaje, NULL);
    pthread_create(&hiloRecibir, NULL, &RecibirMensaje, NULL);

    pthread_join(hiloEnviar, NULL);
    pthread_join(hiloRecibir, NULL);

    if(msgctl(msgID, IPC_RMID, NULL) == -1){
        perror("error en msgctl");
        return 1;
    }

    system("rm chat.txt");
    return 0;

}