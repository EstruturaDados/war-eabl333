#include <stdio.h>
#include <string.h>

/*
    Estrutura: Territorio
    Armazena as informações de um território no mapa.
    Campos:
    - nome: nome do território
    - cor: cor do exército dominante
    - tropas: número de tropas no território
*/
struct Territorio {
    char nome[30];
    char cor[15];
    int tropas;
};

int main() {
    struct Territorio territorios[5]; // Vetor estático com 5 territórios
    int i;

    printf("===========================================\n");
    printf("        CADASTRO INICIAL DOS TERRITÓRIOS   \n");
    printf("===========================================\n\n");

    // Cadastro dos 5 territórios
    for (i = 0; i < 5; i++) {
        printf(">>> Cadastro do território %d\n", i + 1);

        printf("Nome do território: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0'; // remove o \n

        printf("Cor do exército: ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';

        printf("Número de tropas: ");
        scanf("%d", &territorios[i].tropas);
        getchar(); // consome o \n deixado pelo scanf

        printf("-------------------------------------------\n");
    }

    // Exibição do estado atual do mapa
    printf("\n\n=== ESTADO ATUAL DO MAPA ===\n\n");
    for (i = 0; i < 5; i++) {
        printf("Território %d:\n", i + 1);
        printf("  Nome: %s\n", territorios[i].nome);
        printf("  Cor do Exército: %s\n", territorios[i].cor);
        printf("  Tropas: %d\n", territorios[i].tropas);
        printf("-------------------------------------------\n");
    }

    return 0;
}
