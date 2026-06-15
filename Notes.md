## Que es un cliente y un servidor?
### Cliente
Programa que inicia la comunicación y es dirigido por el usuario.
Sencillos, no requieren privilegios del sistema.

> **Cliente estándar**
> Invocan serviciós bien conocidos de TCP/IP (email, terminal remota, etc).
>
> **Cliente no estándar**
> Utilizan servicios localmente conocidos (automatricula, acceso a bibliotecas, etc)

### Servidor
Está ejecutándose indefinidamente a la espera de peticiones de servicio de los *clientes*
Necesitan acceder a recursos del sistema (privilegios de acceso), son mas complejos y robustos. Deben tener en cuenta distintos aspectos como:
> - Autenticación 
> - Autorización
> - Seguridad y privacidad de la información
> - Protección del sistema donde reside

### Servidor con conexión
Utilizan servicios de TCP

### Servidor sin conexión
Utilizan servicios de UDP

---
## ¿Como se obtiene la dirección del servidor?
- Ejemplo: telnet.
    El usuario usa el comando telnet y colocamos la dirección IP
    Ej: $ telnet 158.42.53.99
- La direccción del servidor se encuentra en un fichero local (fichero de configuración del cliente)
- Se utiliza otra aplicación para encontrar el servidor que requiere el cliente.

## ¿Si solo conoce el nombre del servidor?
- A cada dirección IP se le puede asignar un nombre.
    Ej: 158.42.53.99 <-> "zolter.redes.upv.es"
- El servicio que se utliza para traducción de nombres a direcciones IP es: DNS

### Resolución de nombres: Gethostbyname
Existen funciones que permiten invocar el servicio de nombres:
**Gethostbyname** y **Gethostbyaddr**
```c
#include <netdb.h>
struct hostent *gethostbyname (char *name);
struct hostent *gethostbyaddr (char *addr, int addrlen, int addrtype);

struct hostent {
    char *h_name; /* official name of host*/
    char **h_aliases; /* alias list*/
    int h_addrtype; /* host address type */
    int h_length; /* length of address */
    char **h_addr_list; /* address list */
};

#define h_addr h_addr_list[0]
```
Estas funciones buscan información en el fichero *"/etc/hosts"*. Si no la encuentra, inicia una búsqueda sobre un servidor de nombres (DNS).

### Información asociada a un puerto
Existen nombres de servicio asociados a *direcciones de puertos bien-conocidas*(usando protocolos).
Funciones: *Getserverbyname* y *Getserverbyport*
Sirve para realizar las conexiones utilizando el nombre del protocolo, en lugar de recordar el puerto por el que se comunica dicho protocolo.
```c
#include <netdb.h>
struct servent * getserverbyname (char *name, char *prot);
struct servent * getserverbyport (int port, char *prot);

struct servent {
    char *s_name; /* official server name */
    char **s_aliases; /* alias list */
    int s_port; /* port number */
    char *s_proto /* protocol to use */
}
```


# Algoritmos de clientes TCP y UDP

En socket, los protocolos TCP y UDP se tratan de manera diferente.
Porque un socket para un protocolo TCP _requiere establecer una conexión_, mientras que para UDP _no se debe implementar todo el proceso de conexión_ realizado en TCP.

## Algoritmo de un cliente TCP
> 1. Obtener la dirección IP y el número de puerto donde estará esperando el servidor con el que queremos conectar, y luego construir la dirección _**socket**_ remota.
> 2. Crear un _**socket**_ de tipo **SOCK_STREAM** _(SOCKET)_.
> 3. (*) Especificar la dirección del _**socket**_ local que acabamos de crear, dejando que el sistema seleccione un puerto disponible (asignación dinámica). _(BIND)_
> 4. Estableceer la conexión con el servidor. _(CONNECT)_
> 5. Protocolo de aplicación. Dialogo entre cliente y servidor basado en mensajes petición/respuesta. _(READ, WRITE, SEND, RECV)_
> 6. Cerrar la conexión, eliminando el _**socket**_. _(CLOSE, SHUTDOWN)_
>

>  ℹ️ **Información**
>
> El item 3 (*) es opcional.

## Algoritmo de un cliente UDP
> 1. Obtener la dirección IP y el número de puerto donde estará esperando el servidor con el que queremos conectar, y luego construir la dirección _**socket**_ remota.
> 2. Crear un _**socket**_ de tipo **SOCK_DGRAM** _(SOCKET)_.
> 3. Especificar la dirección del _**socket**_ local que acabamos de crear, dejando que el sistema seleccione un puerto disponible (asignación dinámica). _(BIND)_ 
> 4. (*) Especificar el servidor con el que queremos trabajar. _(CONNECT)_
> 5. Protocolo de aplicación. Dialogo entre cliente y servidor basado en mensajes petición/respuesta. _(SENDTO, RECVFROM, READ*, WRITE*, SEND*, RECV*)_.
> 6. Eliminar el _**socket**_. _(CLOSE)_
>

>  ℹ️ **Información**
>
> El item 4(*) es opcional.

Creamos el socket utilizando SOCK_DGRAM indicando que no hay que esperar que se inicie una conexión, luego de crear el socket y creamos un **bind** para vincular el puerto con el socket creado. Al finalizar, eliminamos el socket usando CLOSE.

### Consideraciones sobre el algoritmo UDP
- PASO 4: CONNECT sobre un socket UDP (Opcional)
    - Define la asociación entre un cliente y servidor UDP:
        - Comunicación restringida al servidor asociado.
        - No conlleva intercambio de mensajes.
        - Se pueden usar _READ, WRITE, SEND, RECV_, solamente porque se utilizo CONNECT.
- PASO 5: Transferencia compacta(SIN UTILIZAR CONNECT)
    - Por cada SENDTO se envía todo el mensaje en un solo datagrama, permitiendo asi obtenerlo con un solo RECVFROM.
- PASO 6: Fin de servicio:
    - Al eliminar un socket, el protocolo UDP no informa al otro extremo, rechazando asi los mensajes dirigidos al puerto que estaba vinculado a dicho socket.

> El hecho de usar CONNECT en UDP, me permite poder utilizar las mismas directivas(READ,WRITE,SEND,RECV) que se utilizan en TCP. Es una decision de software, no genera una ventaja.

---

## Servicio DAYTIME
Es un servicio estándar de internet que suministra la fecha y hora actual en una cadena de caracteres [RFC867].

Puerto de _**DAYTIME**_ bien-conocido: 13.

Servicio implementado sobre **TCP** y **UDP**.



