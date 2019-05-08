#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define DEFAULT_PORT 8080

int main( int argc, char *argv[] ){
	
	struct sockaddr_in direccion_servidor;
	direccion_servidor.sin_family = AF_INET;
	direccion_servidor.sin_addr.s_addr = INADDR_ANY;
	direccion_servidor.sin_port = htons(DEFAULT_PORT);

	int servidor = socket(AF_INET, SOCK_STREAM, 0);
	int activado = 1;	
	setsockopt(servidor, SOL_SOCKET, SO_REUSEADDR, &activado, sizeof(activado));
	
	if(bind(servidor,(void* ) &direccion_servidor, sizeof(direccion_servidor)) != 0){
		perror("Falo el bind");
		return 1;
	}
	
	listen(servidor,100);
	struct sockaddr_in direccionCliente;
	unsigned int len;
	int cliente = accept(servidor, (void*) &direccionCliente, &len);
	printf("Recibí una conexión en %d!!\n", cliente);
	
    char mensaje[100];
    sprintf(mensaje,"%s","ESTO ES UN MENSAJE DESDE EL SERVIDOR");

	write(cliente,mensaje,100);
	return 0;		
}
