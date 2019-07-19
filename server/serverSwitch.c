#include "Servidor/servidor.h"
#include "Response/request_switch.h"

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
	int MAX_SIZE_SEND = 1024;
	int FD_USER = *((int *)arg);
	printf("%d\n", FD_USER);
	char mensaje[MAX_SIZE_SEND];
	bzero(mensaje, MAX_SIZE_SEND);

	if (recv(FD_USER, mensaje, sizeof(char) * MAX_SIZE_SEND, 0) > 0)
	{
		ptr_sockerRequest request = socketRequest_create(mensaje);

		if (strcmp(request->servidorName, "FACTURACION") == 0)
		{
			printf("Servidor: %s\nQuery: %s", request->servidorName, request->query);
		}
		else
		{
			printf("NO ES UN SERVIDOR");
		}
	}
	//write(FD_USER, mensaje, sizeof(*mensaje) );
	printf("Enviando mensaje...\n\t%s", mensaje);
	send(FD_USER, mensaje, sizeof(mensaje), 0);
	return NULL;
};
