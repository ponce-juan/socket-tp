#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define RCVBUFFSIZE 512

/* Programa SERVIDOR SOCKET SECUENCIAL, 
    Escucha de a un CLIENTE a la vez.
*/

int main() {
    int sh, csh, l, msglen;
    struct sockaddr_in addr, caddr;
    char msg[RCVBUFFSIZE];

    /* Creacion del socket */
    if((sh=socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
        printf("Failed to create socket in server.\n");
        exit(1);
    }

    /* Configuración del socket */
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(3333);

    /* Creo bind del puerto del servicio */
    if(bind(sh, (struct sockaddr *) &addr, sizeof(addr)) <0){
        printf("Failed to bind socket to port");
        close(sh);
        exit(1);
    }

    /* Preparo el servicio para aceptar una conexion, con una cola de 5 como maximo
        las demas solicitudes son rechazadas.
        retorna -1 si falla, 0 si esta OK
    */
    if(listen(sh, 5) <0){
        printf("Filed to listen at port  %d\n", 3333);
        close(sh);
        exit(1);
    }


    /* Este programa es MONOTAREA, por mas que encole conexiones de clientes, resuelve una a una.
        Porque acepta una conexion, no maneja procesos.
        Para manejar procesos, deberia introducir un fork luego de aceptar la conexion del cliente.
        Indicando que el hijo procese esa conexion y el padre se queda esperando mas accept (conexiones).
    */
    while(1){
        
        l = sizeof(caddr);

        /* Aceptar conexion de cliente.
        
        Recibe el socket maestro del servidor y retorna un nuevo socket por cada cliente */
        if((csh = accept(sh, (struct sockaddr *) &caddr, &l)) < 0){
            printf("Failed to accept client.\n");
            close(sh);
            exit(1);
        }

        /* Mensaje no recibido del socket del cliente */
        if((msglen=recv(csh, msg, RCVBUFFSIZE, 0)) < 0){
            printf("Failed to receive.\n");
            close(csh);
        }else{

            /* Mensaje recibido */
            while(msglen > 0){

                /* Respondo el mensaje del cliente, si falla, salgo del bucle */
                if(send(csh, msg, msglen, 0) < 0){
                    printf("Failed to respond.\n");
                    close(csh);
                    break;
                }

                /* Recibo mensaje del cliente, si falla, salgo del bucle. */
                if((msglen=recv(csh, msg, RCVBUFFSIZE, 0)) < 0){
                    printf("Failed to receive.\n");
                    close(csh);
                    break;
                }
            }
        }
    }

    exit(0);

}