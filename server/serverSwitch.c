#include "Server/servidor.h"

#define DEFAULT_PORT 8080

void *onSuccess(void *);

int main(int argc, char *argv[])
{
	int port = (argc > 2) ? atoi(argv[2]) : DEFAULT_PORT;
	char *ipAddress = (argc > 1) ? argv[1] : NULL;
	printf("Inciando servidor en IP: %s Puerto: %d\n", ipAddress, port);

	ptr_Servidor servidor = servidor_create(ipAddress, port);
	servidor->onSuccess = onSuccess;
	servidor_run(servidor);

	return EXIT_SUCCESS;
}

void *onSuccess(void *arg)
{
	int FD_USER = *((int *)arg);
	printf("%d\n", FD_USER);
	char mensaje[] = "ES UN MENSAJE DEL SERVIDOR";
	//write(FD_USER, mensaje, sizeof(*mensaje) );
	send(FD_USER,mensaje,sizeof(mensaje),0);
	return NULL;
};
