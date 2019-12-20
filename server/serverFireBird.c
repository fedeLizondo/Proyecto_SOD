#include "Servidor/servidor.h"
#include "Response/request_switch.h"
#include "Servidor/client.h"
#include "libfb.h"
#include <string.h>

#define DEFAULT_PORT 8081
extern int FB_SHOW_MESSAGES; 

void *onSuccess(void * arg) {
  int MAX_SIZE_SEND = 4096;
  int MAX_SIZE_RECIVE = 1024;
	int FD_USER = *((int *)arg);
  FB_SHOW_MESSAGES = 0;

  char mensaje[MAX_SIZE_RECIVE];
  char response[MAX_SIZE_SEND];

  if(recv(FD_USER, mensaje, sizeof(char) * MAX_SIZE_SEND, 0) > 0) {
    fb_db_info dbinfo;
    strcpy(dbinfo.user,"root");
    strcpy(dbinfo.passw,"root");
    strcpy(dbinfo.dbname,"172.17.0.3:facturacion");
    strcpy(dbinfo.role,"sysdb");
    if (fb_do_connect(&dbinfo)) { 
      query myquery;
      fb_init(&myquery); 
      if ( fb_do_query(&dbinfo, 1, mensaje, onDoGenericQuery, &myquery) ) {
        rquery *q = myquery.top; // primer tupla del query
        int CANT_COLUMNAS = myquery.cols;
        char **nombreColumna = myquery.colname;
        char **cols;
        for (int i = 0; i < CANT_COLUMNAS; i++)
        {
          strcat(response, *(nombreColumna+i));
          if(i < (CANT_COLUMNAS-1)){
            strcat(response, ", ");
          }
        }
        strcat(response,"\n");

        while (q) {
          cols = (char **) q->col;
          for(int i = 0; i < CANT_COLUMNAS; i++){
            strcat(response, *(cols+i));
            if(i < (CANT_COLUMNAS-1)){
              strcat(response, ", ");
            }
          }
          strcat(response,"\n");
          q = q->next;  
        }
        fb_free(&myquery);
      } else { 
        char mensajeError[400];
        sprintf(mensajeError, "Error en ejecucion de Store Procedure!\n\
                Error FB [%d] mensaje [%s] sql code [%ld]\n",myquery.fb_error,myquery.errmsg,myquery.SQLCODE);
        strcat(response, mensajeError);  
      }
      fb_do_disconnect(&dbinfo);
    } else {
      strcat(response, "ERROR IN CONECT\n");
      fb_do_disconnect(&dbinfo);
    }
    send(FD_USER,response,sizeof(response),0);
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
