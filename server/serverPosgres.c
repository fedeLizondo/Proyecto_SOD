#include "Servidor/servidor.h"
#include "Response/request_switch.h"
#include "Servidor/client.h"

#define DEFAULT_PORT 8082

#include <postgresql/libpq-fe.h>
    
static void
exit_nicely(PGconn *conn, PGresult   *res)
{
    PQclear(res);
    PQfinish(conn);
}

void *onSuccess(void * arg) {
  int MAX_SIZE_SEND = 4096;
  int MAX_SIZE_RECIVE = 1024;
	int FD_USER = *((int *)arg);

  char mensaje[MAX_SIZE_RECIVE];
  char response[MAX_SIZE_SEND];

  if(recv(FD_USER, mensaje, sizeof(char) * MAX_SIZE_SEND, 0) > 0) {
      printf("%s\n",mensaje);
      const char *conninfo = "host=pg-docker hostaddr=172.17.0.2 user=postgres password=docker dbname=personal sslmode=disable";
      PGconn     *conn;
      PGresult   *res;
      conn = PQconnectdb(conninfo);
      if (PQstatus(conn) != CONNECTION_OK) {
          fprintf(stderr, "Connection to database failed: %s", PQerrorMessage(conn));
          strcat(response, "Connection to database failed: ");
          strcat(response, PQerrorMessage(conn));
          send(FD_USER, response, sizeof(response), 0);
          PQfinish(conn);
          return NULL;
      }

    res = PQexec(conn, mensaje);
    if (PQresultStatus(res) != PGRES_COMMAND_OK && PQresultStatus(res) != PGRES_TUPLES_OK )
    {
        fprintf(stderr, "CREATE TABLE failed: %s", PQerrorMessage(conn));
        strcat(response, "Connection to database failed: ");
        strcat(response, PQerrorMessage(conn));
        send(FD_USER, response, sizeof(response), 0);
    } else {
      int ncols = PQnfields(res);
      for (int i=0; i<ncols; i++) {
          strcat(response ,PQfname(res, i));
          if(i < ncols-1){
            strcat(response,", ");
          }
      }
      strcat(response,"\n");
      int rows = PQntuples(res);
      for(int i=0; i<rows; i++) {
          for(int j=0; j<ncols; j++){
            strcat(response, PQgetvalue(res, i, j));
            if(j < ncols-1){
              strcat(response,", ");
            }
          }
          strcat(response,"\n");
      }
    }

    exit_nicely(conn,res);
    send(FD_USER, response, sizeof(response), 0);
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
