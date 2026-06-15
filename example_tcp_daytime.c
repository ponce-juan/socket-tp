#include <stdio.h>
#include <sys/socket.h>
#define LINELEN 128

char *service = "daytime";
char *host = "localhost";

int main (int argc, char *argv[]){
    int s,n;
    char buff[LINELEN];

    switch (argc)
    {
    case 1:
        host = "localhost";
        break;
    case 2: 
        host = argv[1];
        break;
    case 3:
        service = argv[2];
        break;

    default:
        fprintf(stderr, "Uso: DayTime [host[port]]\n");
        exit(1);
    }

    s = connectTCP(host, service);
    printf("\n Fecha y Hora: ");
    while((n = read(s, buff, LINELEN)) > 0)
        printf("%s", buff);

    close(s);
}

int connectTCP(char *host, char *service){
    return connectsock(host, service, "tcp");
}
