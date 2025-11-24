#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>

/* :=====: |CORES TERMINAL| :=====: */

#define Sem_Cor   "\033[0m" // Parar ou Remover a coloração.
#define Verde   "\033[1;32m" 
#define Amarelo "\033[1;33m"
#define Azul    "\033[1;34m"
#define Vermelho "\033[1;31m"
#define Ciano  "\033[1;36m"

/* :=====: |DEFINIÇÕES| :=====: */

#define MAX_ABELHAS 50      // Número Máximo de Abelhas.
#define MAX_SENSORES 100    // Número Máximo de Sensores. 

typedef struct { 
    int id;                     // Campo do ID;
    char nomePopular[40];       // Campo do Nome Popular;
    char nomeCientifico[50];    // Campo de Nome Científico; 
    char regiao[30];            // Campo da Região;
    float producaoMel;          // Campo da Produção de Mel.
} Abelha; 

Abelha abelhas[MAX_ABELHAS];    // "Apelelido" do struct a partir do typedef e a 
int TotalAbelhas = 0;           // abertura do número máximo de informações que serão guardadas (50);

/* :-=-=-: |PROTÓTIPOS DAS FUNÇÕES AUXILIARES| :-=-=-: */

void LimparTela(); // Limpa a Tela/Terminal.
void Pausar(); // Espera o Usuário utilizar o enter para prosseguir ou voltar.
void LerString(char *buffer, int tamanho); // Pede textos e armazena-os, quantidade especificada na frente.
void LimpaBuffer(); // Limpa o Buffer
int OpcoesValidas(int min, int max); // Limita o usuário em escolhas.

/* :-=-=-: |PROTÓTIPOS DAS FUNÇÕES DE ABELHA| :-=-=-: */

void GerenciarAbelhas();
void CadastrarAbelhas();
void ListarAbelhas();
void BuscarAbelhaPorNome();
void AlterarAbelha();
void RemoverAbelha();

int EncontrarAbelhaPorId(int id);

/* :-=-=-: |FUNÇÃO PRINCIPAL: "MAIN"| :-=-=-: */

int main(void) {
    SetConsoleOutputCP(CP_UTF8);     // Acentua palavras no printf;
    SetConsoleCP(CP_UTF8);           // Ler os acentos que o usuário digita no scanf;
    setlocale(LC_ALL, "pt_BR.utf8"); // O "pt_BR.utf8" diz ao computador para usar o padrão português na hora de acentuar palavras
    // Sem esse sistema após abrir o arquivo .exe, o computador não vai conseguir ler corretamente 
    int opcao;                       // e vai substituir por outros caracteres EX: ç → Ã§ ou é → Ã©.
    
    do {
        LimparTela(); 
        
        printf(Azul "|:=====:| SISTEMA BEE MONITOR |:=====:|\n\n" Sem_Cor);
        
        printf("1. Gerenciar Abelhas\n");
        printf("2. Gerenciar Sensores\n");
        printf("3. Relatórios\n");
        printf("4. Sair\n\n");
        printf("Escolha uma opção: ");
        opcao = OpcoesValidas(1, 4); // Utiliza as opções atribuídas a função. (Mínimo, Máximo).
        
        switch (opcao) {             // Switch que percorre as funções para cada opção.
            case 1:
            GerenciarAbelhas();
            break;
            case 2:
            
            break;
            case 3:
            
            break;
            case 4:
            printf(Verde "\nSaindo do sistema. Até logo!\n" Sem_Cor);
            break;
        }
        if (opcao == 4) Pausar();
        
    } while (opcao != 4);
    
    return 0;
}

/* :=====: |PRODUÇÃO DAS FUNÇÕES AUXILIARES| :=====: */

void LimparTela() { // Essa função vai ser usadas em todas as outras.
    
    #ifdef _WIN32 // Limpa terminal Windows. Para uma "melhor experiência" do usuário.
    system("cls");
    #else
    system("clear"); // Limpa terminal caso for MacOS / Linux.
    #endif 
}

void LimpaBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
        ; // apenas descarta os caracteres
    }
}

void Pausar() { // Fica parecendo como se fosse uma "confirmação" do usuário.
    
    printf("\nAperte ENTER para continuar...");
    LimpaBuffer(); // Limpa o buffer de possíveis lixos de memória.
    getchar(); // Espera o Enter do usuário para proseguir.
}

void LerString(char *buffer, int tamanho) {  // Função Boa para não ter que ficar repetindo o fgets toda hora.
    
    fflush(stdin); // Força a limpeza do buffer de entrada (geralmente o que foi digitado no teclado.).
    fgets(buffer, tamanho, stdin); // fgets: Uma forma boa para incluir espaços em entradas de dados.
    buffer[strcspn(buffer, "\n")] = '\0'; // Troca o '\n' que sobra no buffer por '\0' que indica 'fim'.
}


int OpcoesValidas(int min, int max) { // Limita as escolhas do usuário para que ele siga as rotas corretas.
    int opcao;
    char enter;
    
    while (scanf("%d%c", &opcao, &enter) != 2 || enter != '\n' || opcao < min || opcao > max) {
        printf(Vermelho "Opção inválida! Digite um número entre %d e %d: " Sem_Cor, min, max);
        /* scanea em while as duas possibilidades "opção e enter", se conseguir ler esses dois itens o retorno é 2.
        Se o usuário digitar algo que não é um número (ex: "abc"), scanf não conseguirá ler o %d, retornará 0, 
        e a condição ficará: 0 != 2 , será verdadeira, fazendo o laço repetir.
        */
       LimpaBuffer(); // Limpa o buffer.
    }
}

int EncontrarAbelhaPorId(int id) {
    if (id > 0 && id <= TotalAbelhas) return id - 1;
    return -1;
}

/* :=====: |PRODUÇÃO DAS FUNÇÕES DO MENU DAS ABELHAS| :=====: */

void GerenciarAbelhas() {  // Menu de Abelhas. 
    int opcao;
    do {
        LimparTela();
        
        printf(Ciano "|:-=-=-:| GERENCIAR ABELHAS |:-=-=-:|\n\n" Sem_Cor);
        
        printf("1. Cadastrar abelha\n");
        printf("2. Listar todas\n");
        printf("3. Buscar por nome popular\n");
        printf("4. Alterar dados\n");
        printf("5. Remover\n");
        printf("6. Voltar\n\n");
        printf("Escolha uma opção: ");
        opcao = OpcoesValidas(1, 6);
        
        switch (opcao) {
            case 1: CadastrarAbelhas(); break;  //Switch Case para cada funcionalidade do Menu de Abelhas.
            case 2: ListarAbelhas(); break;
            case 3: BuscarAbelhaPorNome(); break;
            case 4: AlterarAbelha(); break;
            case 5: RemoverAbelha(); break;
            case 6: break; // Volta ao menu principal
        }
        if (opcao != 6) Pausar();  // Espera o usuário apertar o Enter (Pausa Simples) para seguir para outra função.
    
    } while (opcao != 6);
}

void CadastrarAbelhas() {  
    LimparTela();
    
    printf(Amarelo "|:-=-=-:| CADASTRAR ABELHA |:-=-=-:|\n\n" Sem_Cor);
    
    if (TotalAbelhas >= MAX_ABELHAS) {  // Condição Simples que evita erro de capacidade!!
        printf(Vermelho "\nErro: Limite máximo de abelhas atingido!\n" Sem_Cor);
        return;
    }
    
    Abelha NovaAbelha;  //Variável Criada do "tipo" Abelha.
    NovaAbelha.id = TotalAbelhas + 1; // Pega o total já existente e soma 1, assim criando e guardando um novo ID.
    
    printf("Nome Popular: ");
    LerString(NovaAbelha.nomePopular, 40); // Guarda os valores nos respectivos Campos da struct.
    printf("Nome Científico: ");
    LerString(NovaAbelha.nomeCientifico, 50);
    printf("Região: ");
    LerString(NovaAbelha.regiao, 30);
    printf("Produção Média de Mel (kg/mês): ");
    scanf("%f", &NovaAbelha.producaoMel);
    
    abelhas[TotalAbelhas] = NovaAbelha; // abelhas abre um array que armazena todas as abelhas do programa.(Declarada no topo).
    TotalAbelhas++;                     // [TotalAbelhas] acessa uma posição específica (posição livre) e posteriormente faz uma "cópia" de 'NovaAelha'
                                        // que foi criada no passo anterior para dentro dessa posição livre no array 'abelhas'.
    printf(Verde "\nAbelha cadastrada com sucesso! (ID: %d)\n" Sem_Cor, NovaAbelha.id);
}

void ListarAbelhas() {
    LimparTela(); 
    
    printf(Amarelo "|:-=-=-:| LISTA DE ABELHAS |:-=-=-:|" Sem_Cor);
    if (TotalAbelhas == 0) { // Condicional de verificação simples.
        printf(Vermelho "\n\nNenhuma abelha foi cadastrada!\n" Sem_Cor); 
        return;
    }
    
    for (int k = 0; k < TotalAbelhas; k++) {  // Usa um 'for' simples que percorre o array 'abelhas' até o limite definido por 'TotalAbelhas'. 
        printf(Ciano "\nID: %d\n" Sem_Cor, abelhas[k].id);   // Mostrar as respectivas informações armazenadas em cada 'campo'.
        printf("Nome Popular: %s\n", abelhas[k].nomePopular);
        printf("Nome Científico: %s\n", abelhas[k].nomeCientifico);
        printf("Região: %s\n", abelhas[k].regiao);
        printf("Produção de Mel: %.2f kg/mês.\n", abelhas[k].producaoMel);
        printf("-------------------------------------\n");
    }
}

void BuscarAbelhaPorNome() {
    LimparTela();
    
    printf(Amarelo "|:-=-=-:| BUSCAR ABELHA POR NOME |:-=-=-:|\n\n" Sem_Cor);
    if (TotalAbelhas == 0) {
        printf(Vermelho "\nNenhuma abelha cadastrada para buscar.\n" Sem_Cor);
        return;
    }
    
    char nomeBusca[40];
    printf("Digite o nome popular da abelha: ");
    LerString(nomeBusca, 40);

    int encontrada = 0; // Criando 'Flag'simples, onde 0 indica "Não Encontrado" e 1 indica "Encontrado".
    for (int k = 0; k < TotalAbelhas; k++) { // 'for' simples.
        
        if (stricmp(abelhas[k].nomePopular, nomeBusca) == 0) {   // O 'stricmp' é uma função do #include <string,h> que compara valores string, onde se a função
            printf("\n|:-=-=-:| Abelha Encontrada |:-=-=-:|\n"); //  retornar o valor 0, indica que as duas strings comparadas são 'IGUAIS'. E se a função ao comparar 
                                                                 // as strings e retornar um valor '!= 0', indica que elas NÃO SÃO IGUAIS.
            printf(Ciano "ID: %d\n" Sem_Cor, abelhas[k].id);
            printf("Nome Popular: %s\n", abelhas[k].nomePopular);           // Se na comparação os nomes forem iguais. Ele mostra as informações de cada 'campo'.
            printf("Nome Científico: %s\n", abelhas[k].nomeCientifico);
            printf("Região: %s\n", abelhas[k].regiao);
            printf("Produção de Mel: %.2f kg/mês\n", abelhas[k].producaoMel);
            printf("-------------------------------------\n");
            encontrada = 1;
        }
    }

    if (encontrada == 0) { // O uso nobre da flag.
        printf(Vermelho "\nNenhuma abelha encontrada com o nome '%s'.\n" Sem_Cor, nomeBusca);
    }
}

void AlterarAbelha() {
    LimparTela();
    
    printf(Amarelo "|:-=-=-:| ALTERAR DADOS DA ABELHA |:=--=-:|\n\n" Sem_Cor);
    if (TotalAbelhas == 0) {
        printf("Nenhuma abelha cadastrada para alterar.\n"); // Condicional simples.
        return;
    }

    int id; // Variável Local Criada.
    printf("Digite o ID da abelha que deseja alterar: ");
    id = OpcoesValidas(1, TotalAbelhas); // Ela recebe a função das opções que compara de 1 até "TotalAbelhas".

    int indice = EncontrarAbelhaPorId(id); //Nova variável que recebe uma função de encontrar, e utilizamos para procurar o id.
    
    printf("\n|:-=-=-:| Dados Atuais |:-=-=-:|\n");
    
    printf("Nome Popular: %s\n", abelhas[indice].nomePopular);  // Agora só mostrar a partir de 'indice' e trocar logo abaixo.
    printf("Nome Científico: %s\n", abelhas[indice].nomeCientifico);
    printf("Região: %s\n", abelhas[indice].regiao);
    printf("Produção de Mel: %.2f kg/mês\n", abelhas[indice].producaoMel);
    printf("\n--- Digite os Novos Dados ---\n");

    printf("Novo Nome Popular: ");
    LerString(abelhas[indice].nomePopular, 40);  // Novos valores que serão atribuidos ao id do 'indice'.
    printf("Novo Nome Científico: ");
    LerString(abelhas[indice].nomeCientifico, 50);
    printf("Nova Região: ");
    LerString(abelhas[indice].regiao, 30);
    printf("Nova Produção Média de Mel (kg/mês): ");
    scanf("%f", &abelhas[indice].producaoMel);

    printf(Verde "\nDados da abelha (ID: %d) alterados com sucesso!\n" Sem_Cor, id);
}

void RemoverAbelha() {
    LimparTela();
    
    printf(Amarelo "|:-=-=-:| REMOVER ABELHA |:-=-=-:|\n\n" Sem_Cor);
    if (TotalAbelhas == 0) {
        printf("Nenhuma abelha cadastrada para remover.\n");
        return;
    }

    int id;
    printf("Digite o ID da abelha que deseja remover: "); // Mesma resolução do Alterar Abelha.
    id = OpcoesValidas(1, TotalAbelhas);

    int indice = EncontrarAbelhaPorId(id);

    printf(Vermelho "\nTem certeza que deseja remover a abelha '%s' (ID: %d)?\n" Sem_Cor, abelhas[indice].nomePopular, id);
    printf(Vermelho "Todos os sensores associados a ela também serão removidos.\n" Sem_Cor);
    printf("Digite 'S' para confirmar: "); // Confirmação para o usuário.
    
    char confirmacao;
    scanf(" %c", &confirmacao);
    LimpaBuffer(); // Limpa o buffer.
    
    if (toupper(confirmacao) == 'S') { // Condição da "confirmação". O 'toupper' para transformar o caratere em maiúsculo.
        printf(Verde "\nAbelha (ID: %d) removida com sucesso!\n" Sem_Cor, id); 

        // Remover a abelha e reorganizar o vetor
        for (int i = indice; i < TotalAbelhas - 1; i++) {
            abelhas[i] = abelhas[i + 1];
            abelhas[i].id = i + 1; // Atualiza o ID da abelha movida
        }
        TotalAbelhas--;
    } else {
        printf(Amarelo "\nRemoção cancelada.\n" Sem_Cor);
    } 
}
