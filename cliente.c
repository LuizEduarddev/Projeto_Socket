#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("Falha ao criar o socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_address.sin_addr)) <= 0) {
        perror("Endereço inválido/indisponível");
        exit(EXIT_FAILURE);
    }

    if (connect(sock, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Falha ao conectar-se ao servidor");
        exit(EXIT_FAILURE);
    }

    char message[1024];
    strcpy(message, "Olá, servidor!");

    if (send(sock, message, strlen(message), 0) < 0) {
        perror("Falha no envio");
        exit(EXIT_FAILURE);
    }
    printf("Mensagem enviada para o servidor\n");

    close(sock);
    return 0;
}