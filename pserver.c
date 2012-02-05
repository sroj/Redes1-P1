#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include "pserver.h"

//Tamaño de los buffers de lectura y escritura en sockets
#define BUFFER_SIZE 8192

#define TRUE 1
#define FALSE 0

void exit_error(char error_msg[])
{
    perror(error_msg);
    exit(EXIT_FAILURE);
}

//Elimina todos los headers del request http del cliente
void eliminarHeaders(char request[])
{
    char* token = strtok(request, "\r\n");
    strcat(request, "\r\n");
}

int processClientRequest(int p_remoto, int timeout, const char hostname[],
                         int client_socket_fd)
{
    struct sockaddr_in server_address;
    struct in_addr ip_address;
    struct hostent* server_description = NULL;
    int server_socket_fd;
    char in_buffer[BUFFER_SIZE];
    char out_buffer[BUFFER_SIZE];

    if (inet_aton(hostname, &ip_address))
        server_description = gethostbyaddr((char *) &ip_address,
                                           sizeof(ip_address), AF_INET);
    else
        server_description = gethostbyname(hostname);

    if (!server_description)
    {
        error("pserver.c: processClientRequest: error al identificar el host");
        exit(EXIT_FAILURE);
    }

    if((server_socket_fd = socket (PF_INET, SOCK_STREAM, 0)) < 0)
        exit_error("pserver.c: processClientRequest: Error abriendo el socket");

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(p_remoto);

    memcpy(&server_address.sin_addr, server_description->h_addr_list[0],
           sizeof(server_address.sin_addr));

    if (connect(server_socket_fd, (struct sockaddr *)&server_address,
                sizeof(server_address)))
        exit_error("pserver.c: processClientRequest: error al conectar al host");

    memset(in_buffer, 0, BUFFER_SIZE);

    if (read(client_socket_fd, in_buffer, BUFFER_SIZE - 1) < 0)
        exit_error("pserver.c: processClientRequest: error al leer del socket");

    printf("Request antes de eliminar headers:\n%s\n", out_buffer);

    eliminarHeaders(in_buffer);

    memcpy(out_buffer, in_buffer, BUFFER_SIZE);

    printf("Request enviado:\n%s\n", out_buffer);

    if (write(server_socket_fd, out_buffer, BUFFER_SIZE - 1) < 0)
        exit_error("pserver.c: processClientRequest: error al escribir al socket");

    if (read(server_socket_fd, out_buffer, BUFFER_SIZE - 1) < 0)
        exit_error("pserver.c: processClientRequest: error al leer del socket");

    printf("Response recibida:\n%s\n", out_buffer);

    close(server_socket_fd);
    return 0;
}


int serve (int pLocal, int p_remoto, int timeout, const char hostname[])
{
    int sockfd, newsockfd, i = 1, pid;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;

    //Apertura del socket
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0 )
        exit_error("pserver.c: Error abriendo el socket");

    //Permite reusar el socket en poco tiempo
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));

    //Inicializacion de la estructura de datos del servidor
    memset((char *) &serv_addr, 0, sizeof(struct sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(pLocal);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,
             sizeof(struct sockaddr_in)) < 0)
        exit_error("pserver.c: Error asociando el socket");

    if (listen(sockfd, 5) < 0)
        exit_error("pserver.c: Error haciendo listening");

    clilen = sizeof(cli_addr);

    //Ciclo de servicio a conexiones
    while(TRUE)
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0)
            exit_error("pserver.c: Error al aceptar una conexion");
        pid = fork();
        if (pid == 0)
        {
            close(sockfd);
            processClientRequest(p_remoto, timeout, hostname, newsockfd);
            close(newsockfd);
            exit(EXIT_SUCCESS);
        }
        else if (pid > 0)
        {
            close(newsockfd);
        }
        else
        {
            exit_error("pserver.c: Error en llamada a fork()");
        }
    }
    return 0;
}
