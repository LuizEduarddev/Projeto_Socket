#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "usuario.h"

#define PORT 4242
#define LEN 4096
#define SERVER_ADDR "127.0.0.1"
int main()
{
    // define a struct do servidor (é uma funcao propria do C)
    usuario *User = (usuario *)malloc(sizeof(usuario));

    struct sockaddr_in sockddr = 
    {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = inet_addr(SERVER_ADDR)
    };

    // Vamos usar isto aqui para ser o servidor
    int server;
    // Pegamos aqui o tamanho da struct
    int len_socket = sizeof(sockddr);
    int slen;

    /* Buffer de recebimento (Mensagem recebida)
    char buffer_in[LEN];
    */

    /* Buffer de envio  (Mensagem enviada) */
    char buffer_out[LEN];

    fprintf(stdout, "Inicializando o servidor do cliente");

    /*
    * A funcao abaixo cria o socket do cliente
    */

    if ((server = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Erro na criacao do socket do cliente.\n");
        return EXIT_FAILURE;
    }
    else
        fprintf(stdout, "Socket criado com sucesso.\nTentando estabelecer conexao com o servidor.\n");

    /* Zerando o cache de memoria para nao ocorrer nenhum erro */
    memset(sockddr.sin_zero, 0x0, 8);

    /* Tentando estabelecer conexao com o servidor */
    if (connect(server, (struct sockaddr *)&sockddr, len_socket) == -1)
    {
        perror("Nao foi possivel conectar-se ao servidor\n");
        return EXIT_FAILURE;
    }
    else
    {
        fprintf(stdout, "Conexao estabelecida com o servidor.");
        fprintf(stdout, "Por favor, digite seu nome de usuario: ");
        fgets(&User->nome_usuario, LEN, stdin);
    }
    /* While para manter a conexao enquanto o usuario nao digitar algo*/

    while(1)
    {

        /* Limpa os buffers de memoria para que nao ocorra nenhum problema */
        memset(buffer_out, 0x0, LEN);

        /*Pergunta ao usuario o que ele deseja fazer*/
        fprintf(stdout, "1 - Para enviar uma mensagem ao servidor.\n2 - encerrar conexao\n: ");
        fgets(&User->mensagem_usuario, LEN, stdin);

        if (strcmp(User->mensagem_usuario, "2") == 0)
        {
            send(server, "close", 5, 0);
        }
        
        else
        {
            fprintf(stdout, "Digite uma mensagem para ser enviada ao servidor: ");
            fprintf(stdout, "Enviando mensagem para o servidor....");
            if ((send(server, User, sizeof(User), 0)) != sizeof(User->mensagem_usuario))
            {
                fprintf(stdout, "A mensagem nao foi enviada com exito. Tente novamente mais tarde.");
                return EXIT_FAILURE;
            }
            else
                fprintf(stdout, "Mensagem enviada com sucesso.\n");
        }
    }
    close(server);

    fprintf(stdout, "Conexao encerrada.\n");
}