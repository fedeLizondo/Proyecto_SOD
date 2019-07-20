#include "Servidor/servidor.h"
#include "Response/request_switch.h"
#include "Servidor/client.h"

#define DEFAULT_PORT 8083

void *onSuccess(void * arg) {
  //CONEXION CON OTRO SERVIDOR EN C++ U OTRO LENGUAJE FIREBASE NO ESTA PARA C
  int MAX_SIZE_SEND = 4096;
  int MAX_SIZE_RECIVE = 1024;
	int FD_USER = *((int *)arg);

  char mensaje[MAX_SIZE_RECIVE];
  char response[MAX_SIZE_SEND];

  if(recv(FD_USER, mensaje, sizeof(char) * MAX_SIZE_SEND, 0) > 0) {
      printf("%s\n",mensaje);
      printf("%s\n", "Enviando mensaje ...");
      send(FD_USER,"{ 1:{'NOMBRE':'MARCO MYSQL','APELLIDO':'POLO MYSQL'}}",sizeof(response),0);
  }
  return NULL;
}

int main(int argc, char *argv[]){
  int port = (argc > 2) ? atoi(argv[2]) : DEFAULT_PORT;
  char *ipAddress = (argc > 1) ? argv[1] : NULL;
  printf("Inciando servidor en IP: %s Puerto: %d\n", (ipAddress == NULL)?"localHost":ipAddress, port);

  ptr_Servidor servidor = servidor_create(ipAddress, port);
  servidor->onSuccess = onSuccess;
  servidor_run(servidor);

  return EXIT_SUCCESS;
}
