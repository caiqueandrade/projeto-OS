#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
#define MAX 100

void LerString(char s[])
{
    setbuf(stdin, 0);
    fgets(s, MAX, stdin);

    if(s[strlen(s)-1] == '\n')
        s[strlen(s)-1] = '\0';
}

void menu1()
{
    printf("===========================================\n");
    printf("===============   Menu 1   ================\n");
    printf("===========================================\n\n");
    printf("1 - Novo Chamado\n");
    printf("2 - Consultas\n");
    printf("3 - Alterar\n");
    printf("4 - Exibir Tabela\n");
    printf("5 - Sair\n\n");
    printf("===========================================\n");
    printf("===========================================\n");
}

void menu2()
{
    printf("===========================================\n");
    printf("===============   Menu 2   ================\n");
    printf("===========================================\n\n");
    printf("1 - Por Setor\n");
    printf("2 - Por status em aberto\n");
    printf("3 - Por status finalizados\n\n");
    printf("===========================================\n");
    printf("===========================================\n");
}

void ExibirTabela(sqlite3_stmt *stmt)
{
    int i;

    printf("===============   Tabela   ================\n");
    // escreveo cabeçalhocom o nomedas colunas
    for(i= 0; i< sqlite3_column_count(stmt); i++)
        printf("%s\t", sqlite3_column_name(stmt, i));
    printf("\n"); // terminaa exibiçãodo cabeçalho

    // para cadalinhada tabelaresultado
    while(sqlite3_step(stmt) != SQLITE_DONE)
    {
        // para cadacoluna
        for(i= 0; i< sqlite3_column_count(stmt); i++)
            printf("%s\t", sqlite3_column_text(stmt, i));
        printf("\n"); // terminaa exibiçãode umalinha
    }

    printf("===========================================\n");
}

int main()
{
    sqlite3 *db = NULL;

    sqlite3_stmt *stmt = NULL;

    int conexao, op1, op2, id, tempo;
    char setor[15], tipo[15], status[15], sql[255];

    printf("Criando conexao com o banco...\n");
    conexao = sqlite3_open("OS.db", &db);
    if(conexao != SQLITE_OK)
    {
        printf("Erro ao conectar ao banco\n");
        exit(-1);
    }
    printf("Sucesso na conexao!\n\n");

    do
    {
        menu1();

        printf("Opcao desejada do Menu 1: ");
        scanf("%i", &op1);

        switch(op1)
        {
        case 1:
            system("CLS");
            printf("===========================================\n");
            printf("============   Novo Chamado   =============\n");
            printf("===========================================\n");
//            printf("Novo chamado:\n");
            sprintf(sql, "SELECT MAX(id) FROM Manutencao");
            sqlite3_prepare(db, sql, -1, &stmt, NULL);
            //ExibirTabela(stmt);

            if(sqlite3_step(stmt) == SQLITE_ROW)
            {
                id = sqlite3_column_int(stmt, 0) + 1;
            }
            else
            {
                id = 1;
            }
//            printf("Id: %i\n\n", id);

            printf("Digite o setor: ");
            LerString(setor);
            printf("Digite o tipo da manutencao: ");
            LerString(tipo);

            sprintf(sql, "INSERT INTO Manutencao (id, setor, tipo, tempo, status) VALUES (%i, '%s', '%s', 0, 'aberta')", id, setor, tipo);
            sqlite3_prepare(db, sql, -1, &stmt, NULL);
            sqlite3_step(stmt);

            printf("\nChamado inserido com sucesso.\n\n");
            system("PAUSE");
            system("CLS");
            break;
        case 2:
            system("cls");
            menu2();

            printf("Opcao desejada do Menu 2: ");
            scanf("%i", &op2);

            switch(op2)
            {
            case 1:
                system("CLS");
                printf("===========================================\n");
                printf("=========   Consulta por Setor   ==========\n");
                printf("===========================================\n");

                printf("Digite o setor: ");
                LerString(setor);
                // prepara a consulta ao banco
                sprintf(sql, "SELECT * FROM Manutencao WHERE setor = '%s'", setor);
                sqlite3_prepare(db, sql, -1, &stmt, NULL);
                printf("\n");
                ExibirTabela(stmt);
                printf("\n");
                system("PAUSE");
                system("CLS");
                break;
            case 2:
                system("CLS");
                printf("===========================================\n");
                printf("====   Consulta por Status em Aberto   ====\n");
                printf("===========================================\n");

                sprintf(sql, "SELECT * FROM Manutencao WHERE status LIKE 'aberta'");
                sqlite3_prepare(db, sql, -1, &stmt, NULL);
                printf("\n");
                ExibirTabela(stmt);
                printf("\n");
                system("PAUSE");
                system("CLS");
                break;
            case 3:
                system("CLS");
                printf("===========================================\n");
                printf("====  Consulta por Status Concluidas   ====\n");
                printf("===========================================\n");

                sprintf(sql, "SELECT * FROM Manutencao WHERE status LIKE 'concluida'");
                sqlite3_prepare(db, sql, -1, &stmt, NULL);
                printf("\n");
                ExibirTabela(stmt);
                printf("\n");
                system("PAUSE");
                system("CLS");
                break;
            }
            break;
        case 3:
            system("CLS");
            printf("===========================================\n");
            printf("==========   Alterar Chamados   ===========\n");
            printf("===========================================\n");

            printf("Digite o id desejado para a busca: ");
            scanf("%i", &id);
            printf("Digite o tempo gasto para o termino do chamado: ");
            scanf("%i", &tempo);

            sprintf(sql, "UPDATE Manutencao SET tempo = %i, status = 'concluida' WHERE id = %i", tempo, id);
            sqlite3_prepare(db, sql, -1, &stmt, NULL);
            sqlite3_step(stmt);

            printf("\nChamado alterado com sucesso.\n\n");
            system("PAUSE");
            system("CLS");
            break;
        case 4:
            system("CLS");
            sprintf(sql, "SELECT * FROM Manutencao");
            sqlite3_prepare(db, sql, -1, &stmt, NULL);
            printf("\n");
            ExibirTabela(stmt);
            printf("\n");
            system("PAUSE");
            system("CLS");
            break;
        case 5:
            printf("\n");
            printf("Programa finalizado com sucesso!");
            printf("\n");
            break;
        }
    }
    while(op1 != 5);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return 0;
}
