#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUFFSIZE 512

int main(){
    int sh, len;
    struct sockaddr_in addr; 
    char msgsend[] = "Hola Mundo\n\r";
    char msgrecv[BUFFSIZE];

    /* La primitiva socket(_domain, _type, _protocol) retorna un file descriptor (entero), -1 si falla.

        _domain -> PF_INET -> Indicar que queremos usar internet
        _type -> SOCK_STREAM -> Indicar que quiero una comincación TCP. (Para UDP usar SOCK_DGRAM)
        _protocol -> IPPROTO_TCP -> Indicar que quiero usar el protocolo TCP
        */
    if((sh = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        printf("Failed to create socket");
        exit(1);
    }

    addr.sin_family = AF_INET; /* Constante para indicar que quiero usar INTERNET */
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); /* Convierte la IP de string a una direccion valida */
    addr.sin_port = htons(3333); /* Convierte el PUERTO en un formato valido para la estrucutra de comunicacion */

    /* Realizo la conexion al servidor 
        sh -> socket creado
        (struct sockaddr *) -> casteo la conversion de sockaddr_in addr en el tipo necesario sockaddr *
        sizeof(addr) -> tamaño de la estructura del socket

        retorna -1 si no puedo conectar al servidor
    */
    if(connect(sh, (struct sockaddr *) &addr, sizeof(addr)) < 0){
        printf("Failed to connect echo server\n");
        close(sh);
        exit(1);
    }
    
    /* Send retorna el numero de bytes enviados, por lo que comparo con el tamaño del mensaje enviado. 
    Si son distintos, algo paso por lo que cierro el socket y termino la ejecucion. */
    if(send(sh, msgsend, sizeof(msgsend), 0) != sizeof(msgsend)){
        printf("Failed to send %s\n", msgsend);
        close(sh);
        exit(1);
    }
    
    /* recv copia byte a byte del socket desde el inicio de buffsize 
    retorna el tamaño de bytes leido o -1 si falla.*/
    if(((len=recv(sh, msgrecv, BUFFSIZE, 0)) < 0)){
        printf("Failed to receive echo\n");
        close(sh);
        exit(1);
    }

    printf("Echo %s", msgrecv);
    close(sh);
    exit(0);

}