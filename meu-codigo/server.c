#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "usuario.h"

/*Definindo a porta padrao*/
#define PORT 4242

/* Tamanho do Buffer */
#define BUFFER_LENGTH 4096
int main() 
{
    printf("Inicializando o socket com o servidor.\n");

    /*Adicionando as estruturas de socket do cliente e do servidor*/
    struct sockaddr_in clienteS, serverS;

    /*Criando o socket do cliente e do servidor*/
    int servidor, cliente;

    /*Criando a variavel de mensagem*/
    char buffer[BUFFER_LENGTH];

    while (1)
    {
        fprintf(stdout, "Iniciando a conexao do socket do servidor.\n");

        /*Cria um socket IPV4 para o servidor*/

        servidor = socket(AF_INET, SOCK_STREAM, 0);
        if (servidor == -1)
        {
            perror("Nao foi possivel inciar a conexao com o socker");
            return EXIT_FAILURE;
        }
        else
            fprintf(stdout, "Socket inicializado com sucesso");

        /*Define as propriedades do socket*/

        serverS.sin_family = AF_INET;
        serverS.sin_port   = htons(PORT);
        memset(serverS.sin_zero, 0x0, 8); 

        /*Associando o socket do servidor (o socket do usuario nao precisa de bind) a uma porta*/
        if (bind(servidor, (struct sockaddr *)&serverS, sizeof(serverS)) == -1)
        {
            perror("Erro para alocar o socket a uma porta.\n");
            return EXIT_FAILURE;
        }

        /*Espera a conexao com o usuario*/
        if (listen(servidor, 1) == -1)
        {
            perror("Erro na espera de conexoe.\n");
            return EXIT_FAILURE;
        }
        else
            fprintf(stdout, "Esperando na porta %d,\n", PORT);

        socklen_t clienteS_len = sizeof(clienteS_len);

        /*Tentando aceitar a conexao com o cliente*/
        if ((cliente = accept(servidor,
                              (struct sockaddr *)&clienteS, &clienteS_len)) == -1)
        {
            perror("Falha ao aceitar a conexao com o cliente.\n");
            return EXIT_FAILURE;
        }
        
        usuario *user;
        int nome_cliente = recv(cliente, buffer, BUFFER_LENGTH, 0);
        if (buffer == sizeof(user))
        {
            fprintf(stdout, "Usuario logado no momento: %s", user->nome_usuario);
        }
        
        while (strcmp(buffer, "close"))
        {
            /*Zera o buffer*/
            memset(buffer, 0x0, BUFFER_LENGTH);

            int received_bytes = recv(cliente, buffer, BUFFER_LENGTH, 0);
            if (received_bytes == -1)
            {
                perror("Falha ao receber dados do cliente.\n");
                return EXIT_FAILURE;
            }

            else if (received_bytes == 0)
            {
                /*Cliente desconectado*/
                fprintf(stdout, "Cliente desconectado");
                break;
                /*Volta ao loop de ouvir a proxima conexao*/
            }

            else
            {
                /*Tentando interpretar os dados enviados pelo usuario*/
                if (received_bytes == sizeof(user))
                {
                    fprintf(stdout, "%s says: %s", user->nome_usuario, user->mensagem_usuario);
                }
            }
        }

    }



    /*Fecha a conexao com o cliente*/
    close(cliente);

    /*Fecha o servidor*/
    close(servidor);

    printf("Conexao encerrada com sucessor");

    return EXIT_SUCCESS;
}