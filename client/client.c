#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define DEFAULT_PORT 8080

int main(int argc, char const *argv[])
{
    printf("Cliente de test A\n");

    struct sockaddr_in Servidor_sock;
    Servidor_sock.sin_family = AF_INET;
    Servidor_sock.sin_addr.s_addr = INADDR_ANY;
    Servidor_sock.sin_port = htons(DEFAULT_PORT);

    int id_servidor = socket(AF_INET, SOCK_STREAM, 0);
    if (id_servidor < 0)
    {
        printf("Error Iniciando el servidor");
        return EXIT_FAILURE;
    }

    if (connect(id_servidor, (struct sockaddr *)&Servidor_sock, sizeof(struct sockaddr_in)) < 0)
    {
        printf("Error conectado con el servidor");
        return EXIT_FAILURE;
    }

    char mensaje[4096];
    char mensajeEnviado[] = "DB:PERSONAL;SELECT * FROM personal;";
    send(id_servidor, mensajeEnviado,sizeof(char) * sizeof(mensajeEnviado),0);
    int ok = read(id_servidor, mensaje, sizeof(char) * 4096);
    //bzero(mensaje,0,);
    //sprintf(mensaje,"%s","ESTO ES UN MENSAJE")ex;
    printf("%s\n", mensaje);
    return 0;
}
