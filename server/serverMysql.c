#include "Servidor/servidor.h"
#include "Response/request_switch.h"
#include "Servidor/client.h"
#include <mysql/mysql.h>

void finish_with_error(MYSQL *con)
{
  fprintf(stderr, "%s\n", mysql_error(con));
  mysql_close(con);
  //exit(1);        
}


#define DEFAULT_PORT 8083

void *onSuccess(void * arg) {
  if(arg == NULL){
    return NULL;
  }

  int MAX_SIZE_SEND = 4096;
  int MAX_SIZE_RECIVE = 1024;
	int FD_USER = *((int *)arg);
  
  char mensaje[MAX_SIZE_RECIVE];
  char response[MAX_SIZE_SEND];
  bzero(response,MAX_SIZE_SEND);
  if (recv(FD_USER, mensaje, sizeof(char) * MAX_SIZE_SEND, 0) > 0) {
    printf("Recivido: %s\n", mensaje);   
    MYSQL *con;
    MYSQL_ROW row;
    char *server = "172.17.0.2";
    char *user = "username";
    char *password = "user";
    char *database = "GESTION_DE_RECURSOS";
    con = mysql_init(NULL);
    /* Connect to database */
    if (!mysql_real_connect(con, server,
      user, password, database, 0, NULL, 0)) {
      fprintf(stderr, "%s\n", mysql_error(con));
      //exit(1);
      return NULL;
    }
    strcat(mensaje,";");
    if (mysql_query(con, mensaje)) 
    {
      finish_with_error(con);
      return NULL;
    }
    
    MYSQL_RES *result = mysql_store_result(con);
    
    if (result == NULL) 
    {
      finish_with_error(con);
      return NULL;
    }

    int num_fields = mysql_num_fields(result);
    
    while ((row = mysql_fetch_row(result))) 
    { 
      for(int i = 0; i < num_fields; i++) 
      { 
        strcat(response,row[i] ? row[i] : "NULL");
        if (i < ( num_fields -1 ))
        {
          strcat(response,", ");
        }
      } 
      strcat(response,"\n");
    }

    mysql_free_result(result);
    mysql_close(con);
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
