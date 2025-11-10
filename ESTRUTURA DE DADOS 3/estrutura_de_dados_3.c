#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Estrutura para representar um território no mapa
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Estrutura para representar um jogador
typedef struct {
    char nome[20];
    char cor[10];
    char* missao;  // Ponteiro para a missão do jogador
} Jogador;

// Vetor com as missões disponíveis no jogo
char* missoes[] = {
    "Conquistar 3 territorios seguidos",
    "Eliminar todas as tropas da cor vermelha",
    "Conquistar 5 territorios no total",
    "Acumular 15 tropas em um unico territorio",
    "Conquistar territorios em sequencia sem perder batalhas"
};

#define TOTAL_MISSOES 5
#define TOTAL_TERRITORIOS 10
#define TOTAL_JOGADORES 3

// Protótipos das funções
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador);
void exibirMissao(char* missao, char* nomeJogador);
void atacar(Territorio* atacante, Territorio* defensor);
void exibirMapa(Territorio* mapa, int tamanho);
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int numJogadores);
void inicializarMapa(Territorio* mapa);
void inicializarJogadores(Jogador* jogadores);

int main() {
    // Inicializa o gerador de números aleatórios
    srand(time(NULL));
    
    // Aloca memória dinamicamente para o mapa de territórios
    Territorio* mapa = (Territorio*)calloc(TOTAL_TERRITORIOS, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memoria para o mapa!\n");
        return 1;
    }
    
    // Aloca memória para os jogadores
    Jogador* jogadores = (Jogador*)calloc(TOTAL_JOGADORES, sizeof(Jogador));
    if (jogadores == NULL) {
        printf("Erro ao alocar memoria para os jogadores!\n");
        free(mapa);
        return 1;
    }
    
    // Inicializa o mapa e os jogadores
    inicializarMapa(mapa);
    inicializarJogadores(jogadores);
    
    // Atribui missões aos jogadores
    printf("=== INICIO DO JOGO WAR ===\n\n");
    for (int i = 0; i < TOTAL_JOGADORES; i++) {
        // Aloca memória para armazenar a missão do jogador
        jogadores[i].missao = (char*)malloc(100 * sizeof(char));
        if (jogadores[i].missao == NULL) {
            printf("Erro ao alocar memoria para a missao!\n");
            liberarMemoria(mapa, jogadores, i);
            return 1;
        }
        
        // Sorteia e atribui a missão
        atribuirMissao(jogadores[i].missao, missoes, TOTAL_MISSOES);
        exibirMissao(jogadores[i].missao, jogadores[i].nome);
    }
    
    printf("\n=== MAPA INICIAL ===\n");
    exibirMapa(mapa, TOTAL_TERRITORIOS);
    
    // Simula alguns turnos de jogo
    int turno = 1;
    int jogoAtivo = 1;
    
    while (jogoAtivo && turno <= 10) {
        printf("\n=== TURNO %d ===\n", turno);
        
        // Cada jogador realiza uma ação
        for (int i = 0; i < TOTAL_JOGADORES && jogoAtivo; i++) {
            printf("\nTurno de %s (cor: %s)\n", jogadores[i].nome, jogadores[i].cor);
            
            // Encontra territórios do jogador para atacar
            int territorioAtacante = -1;
            int territorioDefensor = -1;
            
            // Busca um território do jogador atual
            for (int j = 0; j < TOTAL_TERRITORIOS; j++) {
                if (strcmp(mapa[j].cor, jogadores[i].cor) == 0 && mapa[j].tropas > 1) {
                    territorioAtacante = j;
                    break;
                }
            }
            
            // Busca um território inimigo adjacente (simulação)
            if (territorioAtacante != -1) {
                for (int j = 0; j < TOTAL_TERRITORIOS; j++) {
                    if (strcmp(mapa[j].cor, jogadores[i].cor) != 0) {
                        territorioDefensor = j;
                        break;
                    }
                }
            }
            
            // Realiza o ataque se houver territórios válidos
            if (territorioAtacante != -1 && territorioDefensor != -1) {
                printf("%s ataca %s!\n", 
                       mapa[territorioAtacante].nome, 
                       mapa[territorioDefensor].nome);
                atacar(&mapa[territorioAtacante], &mapa[territorioDefensor]);
            }
            
            // Verifica se o jogador cumpriu sua missão
            if (verificarMissao(jogadores[i].missao, mapa, TOTAL_TERRITORIOS, jogadores[i].cor)) {
                printf("\n*** VITORIA! ***\n");
                printf("%s cumpriu sua missao: %s\n", jogadores[i].nome, jogadores[i].missao);
                jogoAtivo = 0;
                break;
            }
        }
        
        printf("\n=== MAPA APOS TURNO %d ===\n", turno);
        exibirMapa(mapa, TOTAL_TERRITORIOS);
        
        turno++;
    }
    
    if (jogoAtivo) {
        printf("\n=== FIM DO JOGO (limite de turnos) ===\n");
    }
    
    // Libera toda a memória alocada
    liberarMemoria(mapa, jogadores, TOTAL_JOGADORES);
    
    return 0;
}

/**
 * Função: atribuirMissao
 * Sorteia aleatoriamente uma missão do vetor de missões e copia para o destino
 * Parâmetros:
 *   - destino: ponteiro para onde a missão será copiada (passagem por referência)
 *   - missoes: vetor com as missões disponíveis
 *   - totalMissoes: número total de missões no vetor
 */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

/**
 * Função: verificarMissao
 * Verifica se a missão do jogador foi cumprida baseada no estado atual do mapa
 * Parâmetros:
 *   - missao: string da missão a verificar (passagem por valor)
 *   - mapa: ponteiro para o vetor de territórios
 *   - tamanho: número de territórios no mapa
 *   - corJogador: cor do jogador para verificação
 * Retorno: 1 se a missão foi cumprida, 0 caso contrário
 */
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador) {
    // Verifica: "Conquistar 3 territorios seguidos"
    if (strstr(missao, "3 territorios seguidos") != NULL) {
        int sequencia = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                sequencia++;
                if (sequencia >= 3) return 1;
            } else {
                sequencia = 0;
            }
        }
    }
    
    // Verifica: "Eliminar todas as tropas da cor vermelha"
    if (strstr(missao, "cor vermelha") != NULL) {
        int encontrouVermelho = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelho") == 0) {
                encontrouVermelho = 1;
                break;
            }
        }
        if (!encontrouVermelho && strcmp(corJogador, "vermelho") != 0) return 1;
    }
    
    // Verifica: "Conquistar 5 territorios no total"
    if (strstr(missao, "5 territorios no total") != NULL) {
        int contador = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                contador++;
            }
        }
        if (contador >= 5) return 1;
    }
    
    // Verifica: "Acumular 15 tropas em um unico territorio"
    if (strstr(missao, "15 tropas") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 15) {
                return 1;
            }
        }
    }
    
    return 0;
}

/**
 * Função: exibirMissao
 * Exibe a missão atribuída ao jogador (passagem por valor)
 */
void exibirMissao(char* missao, char* nomeJogador) {
    printf("Jogador %s recebeu a missao: %s\n", nomeJogador, missao);
}

/**
 * Função: atacar
 * Simula um ataque entre dois territórios usando dados de 6 faces
 * Parâmetros:
 *   - atacante: ponteiro para o território atacante (passagem por referência)
 *   - defensor: ponteiro para o território defensor (passagem por referência)
 */
void atacar(Territorio* atacante, Territorio* defensor) {
    // Valida se o ataque é possível
    if (atacante->tropas <= 1) {
        printf("Ataque invalido: territorio atacante precisa ter mais de 1 tropa!\n");
        return;
    }
    
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque invalido: nao pode atacar territorio proprio!\n");
        return;
    }
    
    // Simula rolagem de dados (1 a 6)
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;
    
    printf("Dados: Atacante=%d, Defensor=%d\n", dadoAtacante, dadoDefensor);
    
    if (dadoAtacante > dadoDefensor) {
        // Atacante venceu
        printf("Atacante venceu! %s conquistou %s\n", atacante->nome, defensor->nome);
        
        // Transfere metade das tropas do atacante
        int tropasMover = atacante->tropas / 2;
        defensor->tropas = tropasMover;
        atacante->tropas -= tropasMover;
        
        // Muda a cor do território conquistado
        strcpy(defensor->cor, atacante->cor);
    } else {
        // Defensor resistiu
        printf("Defensor resistiu! %s perde 1 tropa\n", atacante->nome);
        atacante->tropas--;
        
        if (atacante->tropas <= 0) {
            atacante->tropas = 1; // Garante mínimo de 1 tropa
        }
    }
}

/**
 * Função: exibirMapa
 * Exibe o estado atual de todos os territórios no mapa
 */
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n%-20s %-12s %s\n", "TERRITORIO", "COR", "TROPAS");
    printf("--------------------------------------------\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%-20s %-12s %d\n", mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/**
 * Função: liberarMemoria
 * Libera toda a memória alocada dinamicamente no jogo
 */
void liberarMemoria(Territorio* mapa, Jogador* jogadores, int numJogadores) {
    // Libera as missões dos jogadores
    for (int i = 0; i < numJogadores; i++) {
        if (jogadores[i].missao != NULL) {
            free(jogadores[i].missao);
        }
    }
    
    // Libera o vetor de jogadores
    if (jogadores != NULL) {
        free(jogadores);
    }
    
    // Libera o mapa
    if (mapa != NULL) {
        free(mapa);
    }
    
    printf("\n[Memoria liberada com sucesso]\n");
}

/**
 * Função: inicializarMapa
 * Inicializa os territórios do mapa com valores padrão
 */
void inicializarMapa(Territorio* mapa) {
    char* nomes[] = {"Brasil", "Argentina", "Peru", "Colombia", "Venezuela",
                     "Chile", "Uruguai", "Bolivia", "Equador", "Paraguai"};
    char* cores[] = {"azul", "vermelho", "verde"};
    
    for (int i = 0; i < TOTAL_TERRITORIOS; i++) {
        strcpy(mapa[i].nome, nomes[i]);
        strcpy(mapa[i].cor, cores[i % 3]);
        mapa[i].tropas = (rand() % 5) + 3; // Entre 3 e 7 tropas
    }
}

/**
 * Função: inicializarJogadores
 * Inicializa os dados dos jogadores
 */
void inicializarJogadores(Jogador* jogadores) {
    char* nomes[] = {"Napoleao", "Alexandre", "Cesar"};
    char* cores[] = {"azul", "vermelho", "verde"};
    
    for (int i = 0; i < TOTAL_JOGADORES; i++) {
        strcpy(jogadores[i].nome, nomes[i]);
        strcpy(jogadores[i].cor, cores[i]);
        jogadores[i].missao = NULL;
    }
}