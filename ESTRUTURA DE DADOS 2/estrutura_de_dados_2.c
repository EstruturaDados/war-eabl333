#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura base do jogo: representa um território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// ==== PROTÓTIPOS DAS FUNÇÕES ====
void cadastrarTerritorios(Territorio *mapa, int n);
void exibirTerritorios(Territorio *mapa, int n);
void atacar(Territorio *atacante, Territorio *defensor);
void liberarMemoria(Territorio *mapa);

// ==== FUNÇÃO PRINCIPAL ====
int main() {
    srand(time(NULL)); // Garante resultados aleatórios diferentes a cada execução

    int n;
    Territorio *mapa;
    int indiceAtacante, indiceDefensor;

    printf("=========================================\n");
    printf("      SISTEMA DE ATAQUE - WAR ESTRUTURADO\n");
    printf("=========================================\n\n");

    printf("Digite o número total de territórios: ");
    scanf("%d", &n);
    getchar(); // limpa o buffer

    // Alocação dinâmica do vetor de territórios
    mapa = (Territorio *) calloc(n, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);
    exibirTerritorios(mapa, n);

    printf("\n=== SIMULAÇÃO DE ATAQUE ===\n");
    printf("Escolha o número do território atacante (0 a %d): ", n - 1);
    scanf("%d", &indiceAtacante);
    getchar();

    printf("Escolha o número do território defensor (0 a %d): ", n - 1);
    scanf("%d", &indiceDefensor);
    getchar();

    // Validação: não pode atacar a si mesmo nem um território da mesma cor
    if (indiceAtacante == indiceDefensor) {
        printf("Um território não pode atacar a si mesmo!\n");
    } else if (strcmp(mapa[indiceAtacante].cor, mapa[indiceDefensor].cor) == 0) {
        printf("Você não pode atacar um território do mesmo exército!\n");
    } else {
        atacar(&mapa[indiceAtacante], &mapa[indiceDefensor]);
    }

    printf("\n=== ESTADO FINAL DO MAPA ===\n");
    exibirTerritorios(mapa, n);

    liberarMemoria(mapa);
    return 0;
}

// ==== FUNÇÃO: CADASTRO DOS TERRITÓRIOS ====
void cadastrarTerritorios(Territorio *mapa, int n) {
    printf("\n=== CADASTRO DE TERRITÓRIOS ===\n\n");
    for (int i = 0; i < n; i++) {
        printf(">>> Território %d\n", i);

        printf("Nome: ");
        fgets(mapa[i].nome, sizeof(mapa[i].nome), stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exército: ");
        fgets(mapa[i].cor, sizeof(mapa[i].cor), stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar();

        printf("-----------------------------------------\n");
    }
}

// ==== FUNÇÃO: EXIBIÇÃO DOS TERRITÓRIOS ====
void exibirTerritorios(Territorio *mapa, int n) {
    printf("\n-----------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] %s | Exército: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("-----------------------------------------\n");
}

// ==== FUNÇÃO: ATAQUE ENTRE TERRITÓRIOS ====
void atacar(Territorio *atacante, Territorio *defensor) {
    printf("\n%s (Exército %s) está atacando %s (Exército %s)!\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    // Simula dados de batalha
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("Dado do atacante: %d\n", dadoAtacante);
    printf("Dado do defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("O atacante venceu a batalha!\n");

        // O defensor muda de dono e perde metade das tropas
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;

        // O atacante perde 1 tropa como custo da batalha
        atacante->tropas -= 1;
    } else {
        printf("O defensor resistiu ao ataque!\n");
        atacante->tropas -= 1; // perde uma tropa
    }

    if (atacante->tropas < 0)
        atacante->tropas = 0;

    if (defensor->tropas < 0)
        defensor->tropas = 0;

    printf("\nResultado pós-ataque:\n");
    printf("%s -> Tropas: %d | Cor: %s\n", atacante->nome, atacante->tropas, atacante->cor);
    printf("%s -> Tropas: %d | Cor: %s\n", defensor->nome, defensor->tropas, defensor->cor);
}

// ==== FUNÇÃO: LIBERAÇÃO DE MEMÓRIA ====
void liberarMemoria(Territorio *mapa) {
    free(mapa);
    printf("\nMemória liberada com sucesso!\n");
}
