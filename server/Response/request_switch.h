#ifndef __REQUEST_SWITCH_H__
#define __REQUEST_SWITCH_H__

#include <stdlib.h>
#include <string.h>


typedef struct SocketRequest
{
    char *request;
    char *servidorName;
    char *query;
    int size;
} SocketRequest;

typedef SocketRequest * ptr_sockerRequest;

ptr_sockerRequest socketRequest_create( const char* request);
void socketRequest_destroy(ptr_sockerRequest);

#endif