#include "request_switch.h"
ptr_sockerRequest socketRequest_create(const char *request)
{
    ptr_sockerRequest socket_request = malloc(sizeof(SocketRequest));
    strcpy(socket_request->request, request);

    char *serverName = NULL;
    char *query = NULL;
    char *delimitatorPuntoYComa = ";";
    char *delimitatorDosPuntos = ":";

    char *token = strtok(socket_request->request, delimitatorPuntoYComa);
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
    strcpy(socket_request->servidorName, serverName);
    strcpy(socket_request->query, query);
    return socket_request;
};

void socketRequest_destroy(ptr_sockerRequest this)
{
    free(this);
};