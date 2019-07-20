#include "Servidor/servidor.h"
#include "Response/request_switch.h"
#include "Servidor/client.h"

#define DEFAULT_PORT 8080
#define SERVIDOR_FACTURACION "FACTURACION"
#define SERVIDOR_RECURSOS "RECURSOS"
#define SERVIDOR_PERSONAL "PERSONAL"

char *delimitatorPuntoYComa = ";";
char *delimitatorDosPuntos = ":";

void *onSuccess(void *);

int main(int argc, char *argv[])
{
	int port = (argc > 2) ? atoi(argv[2]) : DEFAULT_PORT;
	char *ipAddress = (argc > 1) ? argv[1] : NULL;
	printf("Inciando servidor en IP: %s Puerto: %d\n", (ipAddress == NULL)?"localHost":ipAddress, port);

	ptr_Servidor servidor = servidor_create(ipAddress, port);
	servidor->onSuccess = onSuccess;
	servidor_run(servidor);

	return EXIT_SUCCESS;
}

void *onSuccess(void *arg)
{
	int MAX_SIZE_SEND = 1024;
	int MAX_SIZE_RECIVE = 4096;

	int FD_USER = *((int *)arg);
	char mensaje[MAX_SIZE_SEND];
	char response[MAX_SIZE_RECIVE];
	bzero(mensaje, MAX_SIZE_SEND);

	if(recv(FD_USER, mensaje, sizeof(char) * MAX_SIZE_SEND, 0) > 0)
	{
		//TODO MOVE TO REQUEST_SWITCH
		char *serverName = NULL;
		char *query = NULL;

		char *token = strtok(mensaje, delimitatorPuntoYComa);
		if (token != NULL)
		{
				serverName = token;
				token = strtok(NULL, delimitatorPuntoYComa);
				if (token != NULL)
				{
						query = token;
				}
		}

		strtok(serverName, delimitatorDosPuntos);
		serverName = strtok(NULL, delimitatorDosPuntos);

		if(serverName == NULL){
			printf("No existe el %s , no se puede enviar la query \"%s\"\n",serverName, query);
		}
		else if( strcmp(serverName,SERVIDOR_FACTURACION) == 0) {
			//send(FD_USER, "ENV", sizeof(char) *  3, 0);
			printf("%s\n","EN EL SERVIDOR DE FACTURA");
		}
		else if( strcmp(serverName,SERVIDOR_PERSONAL) == 0) {
			printf("%s\n", "ENVIANDO A PERSONAL");
		}
		else if( strcmp(serverName,SERVIDOR_RECURSOS) == 0) {
			printf("%s\n", "ENVIANDO A RECURSOS");
		}

	}
	// write(FD_USER, mensaje, sizeof(*mensaje) );
	// send(FD_USER, mensaje, sizeof(mensaje), 0);
	return NULL;
};
