#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int server_socket, new_socket, mensagem, user;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[1024] = {0};
    char usuario[1024] = {0};
    char response[1024] = {0};
    int responder = 0;
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Falha ao criar o socket");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(SERVER_PORT);

    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Falha ao realizar o bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, 3) < 0) {
        perror("Falha ao ouvir as conexões");
        exit(EXIT_FAILURE);
    }

    if ((new_socket = accept(server_socket, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
        perror("Falha ao aceitar a conexão");
        exit(EXIT_FAILURE);
    }

   while (1) {
        memset(buffer, 0, sizeof(buffer));
        memset(usuario, 0, sizeof(usuario));

        int mensagem = read(new_socket, buffer, sizeof(buffer));
        int user = read(new_socket, usuario, sizeof(usuario));

        if (mensagem > 0 && user > 0) {
            printf("Mensagem de %s: %s\n", usuario, buffer);

            // Lógica para responder ao cliente
            strcpy(response, "Resposta do servidor!");
            if (send(new_socket, response, strlen(response), 0) < 0) {
                perror("Falha no envio da resposta");
                exit(EXIT_FAILURE);
            }
        }
    }

    printf("Encerrando a conexao...");
    close(new_socket);
    close(server_socket);
    return 0;
}