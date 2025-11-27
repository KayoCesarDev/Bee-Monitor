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

#define MAX_ABELHAS 50     // Número Máximo de Abelhas.
#define MAX_SENSORES 100    // Número Máximo de Sensores. 
#define ARQUIVO_ABELHAS "abelhas.dat"
#define ARQUIVO_SENSORES "sensores.dat"

// Estrutura das Abelhas.
typedef struct { 
    int id;                     // Campo do ID;
    char nomePopular[40];       // Campo do Nome Popular;
    char nomeCientifico[50];    // Campo de Nome Científico; 
    char regiao[30];            // Campo da Região;
    float producaoMel;          // Campo da Produção de Mel.
} Abelha; 

Abelha abelhas[MAX_ABELHAS];    // "Apelido" do struct criado a partir do typedef e a 
int TotalAbelhas = 0;           // abertura do contador de Abelhas.

// Estrutura dos Sensores.
typedef struct { 
    int id;                     // Campo do ID;
    char tipo[30];              // Campo Tipo do Sensor (Luminosidade, Temperatura e Umidade);
    float valor;                // Campo do Valor do "Tipo";
    int idAbelha;               // Campo do ID da abelha.
} SENSOR;

SENSOR sensores [MAX_SENSORES]; // "Apelido" do struct criado a partir do typedef e a 
int SensoresTotais = 0;         // abertura do contador dos Sensores.

/* :-=-=-: |PROTÓTIPOS DAS FUNÇÕES AUXILIARES| :-=-=-: */

void LimparTela(); // Limpa a Tela/Terminal.
void Pausar(); // Espera o Usuário utilizar o enter para prosseguir ou voltar.
void LerString(char *buffer, int tamanho); // Pede textos e armazena-os, quantidade especificada na frente.
void LimpaBuffer(); // Limpa o Buffer
int OpcoesValidas(int min, int max); // Limita o usuário em escolhas.

// Funções de persistência de dados
void salvarDados();
void carregarDados();

/* :-=-=-: |PROTÓTIPOS DAS FUNÇÕES DE ABELHA| :-=-=-: */

void GerenciarAbelhas();
void CadastrarAbelhas();
void ListarAbelhas();
void BuscarAbelhaPorNome();
void AlterarAbelha();
void RemoverAbelha();

int EncontrarAbelhaPorId(int id);

/* :-=-=-: |PROTÓTIPOS DAS FUNÇÕES DOS SENSORES| :-=-=-: */

void GerenciarSensores();
void CadastrarSensor();
void ListarSensores();
void BuscarID();
void AlterarSensor();
void RemocaodeSensor();

/*:-=-=-: |PRÓTÓTIPOS DAS FUNÇÕES DOS SENSORES| :-=-=-: */

void menuRelatorios();
void rel_mediaProducaoMel();
void rel_mediaTemperaturaSensores();
void rel_qtdAbelhasPorRegiao(); 

/* :-=-=-: |FUNÇÃO PRINCIPAL: "MAIN"| :-=-=-: */

int main(void) {
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);     // Acentua palavras no printf;
        SetConsoleCP(CP_UTF8);           // Ler os acentos que o usuário digita no scanf;
    #endif
    setlocale(LC_ALL, "pt_BR.utf8"); // O "pt_BR.utf8" diz ao computador para usar o padrão português na hora de acentuar palavras
    // Sem esse sistema após abrir o arquivo .exe, o computador não vai conseguir ler corretamente 
    int opcao;                       // e vai substituir por outros caracteres EX: ç → Ã§ ou é → Ã©.
    
    carregarDados(); // Carrega os dados dos arquivos ao iniciar.

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
                GerenciarSensores();
                break;
            case 3:
                menuRelatorios();
            break;
            case 4:
                printf(Verde "\nSaindo do sistema. Até logo!\n" Sem_Cor);
                break;
        }
        
    } while (opcao != 4);
    
    return 0;
}

/* :=====: |PRODUÇÃO DAS FUNÇÕES DE PERSISTÊNCIA| :=====: */

void salvarDados() {
    FILE *f_abelhas, *f_sensores;

    // Salvar Abelhas
    f_abelhas = fopen(ARQUIVO_ABELHAS, "wb"); // A função 'fopen' é usada para abrir um arquivo em C.
    if (f_abelhas == NULL) {
        printf(Vermelho "Erro ao abrir o arquivo de abelhas para escrita.\n" Sem_Cor);
        Pausar();
        return;
    }
    fwrite(&TotalAbelhas, sizeof(int), 1, f_abelhas); // A função 'fwrite' é usada para escrever dados binários em um arquivo.
    fwrite(abelhas, sizeof(Abelha), TotalAbelhas, f_abelhas); // Ela escreve blocos de memória diretamente no arquivo. 
    fclose(f_abelhas); // A função 'fclose' fecha um arquivo que foi aberto com a função 'fopen'. 

    // Salvar Sensores
    f_sensores = fopen(ARQUIVO_SENSORES, "wb");
    if (f_sensores == NULL) {
        printf(Vermelho "Erro ao abrir o arquivo de sensores para escrita.\n" Sem_Cor);
        Pausar();
        return;
    }
    fwrite(&SensoresTotais, sizeof(int), 1, f_sensores);
    fwrite(sensores, sizeof(SENSOR), SensoresTotais, f_sensores);
    fclose(f_sensores);
}

void carregarDados() {
    FILE *f_abelhas, *f_sensores;

    // Carregar Abelhas
    f_abelhas = fopen(ARQUIVO_ABELHAS, "rb");
    if (f_abelhas != NULL) {
        fread(&TotalAbelhas, sizeof(int), 1, f_abelhas); // Se o 'fwrite' é usado para "Escrever", a função fread é usada para "Ler" dados 
        fread(abelhas, sizeof(Abelha), TotalAbelhas, f_abelhas); // diretamente do arquivo e coloca dentro de uma variável que você indicar.
        fclose(f_abelhas);
    }

    // Carregar Sensores
    f_sensores = fopen(ARQUIVO_SENSORES, "rb");
    if (f_sensores != NULL) {
        fread(&SensoresTotais, sizeof(int), 1, f_sensores);
        fread(sensores, sizeof(SENSOR), SensoresTotais, f_sensores);
        fclose(f_sensores);
    }
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
    getchar();     // Apenas espera o Enter. A limpeza do buffer deve ser feita após a leitura de dados (scanf).
}

void LerString(char *buffer, int tamanho) {  // Função Boa para não ter que ficar repetindo o fgets toda hora.
    fgets(buffer, tamanho, stdin); // fgets: Uma forma boa para incluir espaços em entradas de dados.
    buffer[strcspn(buffer, "\n")] = '\0'; // Troca o '\n' que sobra no buffer por '\0' que indica 'fim'.
}

int OpcoesValidas(int min, int max) { // Limita as escolhas do usuário para que ele siga as rotas corretas.
    int opcao;
    
    while (scanf("%d", &opcao) != 1 || opcao < min || opcao > max) {
        LimpaBuffer(); // Limpa a entrada inválida
        printf(Vermelho "Opção inválida! Digite um número entre %d e %d: " Sem_Cor, min, max);
    }
    LimpaBuffer(); // Limpa o '\n' que o scanf deixou
    return opcao;
}

int EncontrarAbelhaPorId(int id) {
    for (int k = 0; k < TotalAbelhas; k++) {
        if (abelhas[k].id == id) {
            return k; // Retorna o índice do array
        }
    }
    return -1; // Retorna -1 se não encontrar
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
    
    } while (opcao != 6);
}

void CadastrarAbelhas() {
    LimparTela();
    
    printf(Amarelo "|:-=-=-:| CADASTRAR ABELHA |:-=-=-:|\n\n" Sem_Cor);
    
    if (TotalAbelhas >= MAX_ABELHAS) {  // Condição Simples que evita erro de capacidade!!
        printf(Vermelho "\nErro: Limite máximo de abelhas atingido!\n" Sem_Cor);
        Pausar();
        return;
    }
    
    Abelha NovaAbelha;  //Variável Criada do "tipo" Abelha.
    
    // Encontra o maior ID existente para garantir que o novo ID seja único
    int maiorID = 0;
    for (int k = 0; k < TotalAbelhas; k++) {
        if (abelhas[k].id > maiorID) {
            maiorID = abelhas[k].id;
        }
    }
    NovaAbelha.id = maiorID + 1;

    printf("Nome Popular: ");
    LerString(NovaAbelha.nomePopular, 40); // Guarda os valores nos respectivos Campos da struct.
    printf("Nome Científico: ");
    LerString(NovaAbelha.nomeCientifico, 50);
    printf("Região: ");
    LerString(NovaAbelha.regiao, 30);
    printf("Produção Média de Mel (kg/mês): ");
    while (scanf("%f", &NovaAbelha.producaoMel) != 1) { // O código força o usuário a digitar um número válido. Ele fica "preso" em um loop.
        LimpaBuffer();
        printf(Vermelho "Valor inválido. Digite um número: " Sem_Cor);
    }
    LimpaBuffer();
    
    abelhas[TotalAbelhas] = NovaAbelha;
    TotalAbelhas++;
    
    salvarDados(); // Salva os dados após o cadastro

    printf(Verde "\nAbelha cadastrada com sucesso! (ID: %d)\n" Sem_Cor, NovaAbelha.id);
    Pausar();
}

void ListarAbelhas() {
    LimparTela(); 
    
    printf(Amarelo "|:-=-=-:| LISTA DE ABELHAS |:-=-=-:|" Sem_Cor);
    if (TotalAbelhas == 0) { // Condicional de verificação simples.
        printf(Vermelho "\n\nNenhuma abelha foi cadastrada!\n" Sem_Cor); 
        Pausar();
        return;
    }
    
    for (int k = 0; k < TotalAbelhas; k++) {  // Usa um 'for' simples que percorre o array 'abelhas' até o limite definido por 'TotalAbelhas'. 
        printf(Ciano "\nID: %d\n" Sem_Cor, abelhas[k].id);  // Mostrar as respectivas informações armazenadas em cada 'campo'.
        printf("-------------------------------------\n");
        printf("Nome Popular: %s\n", abelhas[k].nomePopular);
        printf("Nome Científico: %s\n", abelhas[k].nomeCientifico);
        printf("Região: %s\n", abelhas[k].regiao);
        printf("Produção de Mel: %.2f kg/mês.\n", abelhas[k].producaoMel);
        printf("-------------------------------------\n");
    }
    Pausar();
}

void BuscarAbelhaPorNome() {
    LimparTela();
    
    printf(Amarelo "|:-=-=-:| BUSCAR ABELHA POR NOME |:-=-=-:|\n\n" Sem_Cor);
    if (TotalAbelhas == 0) {
        printf(Vermelho "\nNenhuma abelha cadastrada para buscar.\n" Sem_Cor);
        Pausar();
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
    Pausar();
}

void AlterarAbelha() {
    LimparTela();
    
    printf(Amarelo "|:-=-=-:| ALTERAR DADOS DA ABELHA |:=--=-:|\n\n" Sem_Cor);
    if (TotalAbelhas == 0) {
        printf(Vermelho"Nenhuma abelha cadastrada para alterar.\n"Sem_Cor); // Condicional simples.
        Pausar();
        return;
    }

    int id; // Variável Local Criada.
    printf("Digite o ID da abelha que deseja alterar: ");
    scanf("%d", &id);
    LimpaBuffer();

    int indice = EncontrarAbelhaPorId(id); //Nova variável que recebe uma função de encontrar, e utilizamos para procurar o id.
    
    if (indice == -1) {
        printf(Vermelho "\nAbelha com ID %d não encontrada.\n" Sem_Cor, id);
        Pausar();
        return;
    }

    printf("\n|:-=-=-:| Dados Atuais |:-=-=-:|\n");
    
    printf("Nome Popular: %s\n", abelhas[indice].nomePopular);  // Agora só mostrar a partir de 'indice' e trocar logo abaixo.
    printf("Nome Científico: %s\n", abelhas[indice].nomeCientifico);
    printf("Região: %s\n", abelhas[indice].regiao);
    printf("Produção de Mel: %.2f kg/mês\n", abelhas[indice].producaoMel);
    printf("\n--- Digite os Novos Dados (deixe em branco para não alterar) ---\n");

    char buffer[50];

    printf("Novo Nome Popular: ");
    LerString(buffer, 40);
    if (strlen(buffer) > 0){
        strcpy(abelhas[indice].nomePopular, buffer);
    } 

    printf("Novo Nome Científico: ");
    LerString(buffer, 50);
    if (strlen(buffer) > 0) {
        strcpy(abelhas[indice].nomeCientifico, buffer);
    }  

    printf("Nova Região: ");
    LerString(buffer, 30);
    if (strlen(buffer) > 0) {
        strcpy(abelhas[indice].regiao, buffer);
    }  

    printf("Nova Produção Média de Mel (kg/mês): ");
    LerString(buffer, 20);
    if (strlen(buffer) > 0) {
        sscanf(buffer, "%f", &abelhas[indice].producaoMel); // Lê como "string", mas já entrega os valores no tipo correto ("%f");
    }

    salvarDados(); // Salva os dados após a alteração

    printf(Verde "\nDados da abelha (ID: %d) alterados com sucesso!\n" Sem_Cor, id);
    Pausar();
}

void RemoverAbelha() {
    LimparTela();
    
    printf(Amarelo "|:-=-=-:| REMOVER ABELHA |:-=-=-:|\n\n" Sem_Cor);
    if (TotalAbelhas == 0) {
        printf(Vermelho"Nenhuma abelha cadastrada para remover.\n"Sem_Cor);
        Pausar();
        return;
    }

    int id;
    printf("Digite o ID da abelha que deseja remover: "); 
    scanf("%d", &id);
    LimpaBuffer();

    int indice = EncontrarAbelhaPorId(id);

    if (indice == -1) {
        printf(Vermelho "\nAbelha com ID %d não encontrada.\n" Sem_Cor, id);
        Pausar();
        return;
    }

    printf(Vermelho "\nTem certeza que deseja remover a abelha '%s' (ID: %d)?\n" Sem_Cor, abelhas[indice].nomePopular, id);
    printf("Digite 'S' para confirmar ou 'N' para Cancelar: "); // Confirmação para o usuário.
    
    char confirmacao;
    scanf(" %c", &confirmacao);
    LimpaBuffer(); // Limpa o buffer.
    
    if (toupper(confirmacao) == 'S') {
        
        // Remove a abelha e reorganiza o vetor.
        for (int k = indice; k < TotalAbelhas - 1; k++) { 
            abelhas[k] = abelhas[k + 1];
            abelhas[k].id = k + 1;
        }
        TotalAbelhas--;

        // Remove sensores associados
        int j = 0;
        while (j < SensoresTotais) {
            if (sensores[j].idAbelha == id) {
                for (int k = j; k < SensoresTotais - 1; k++) {
                    sensores[k] = sensores[k + 1];
                }
                SensoresTotais--;
            } else {
                j++;
            }
        }

        // Realoca o ID dos sensores associados.
        for (int i = 0; i < SensoresTotais; i++) {
            
            if (sensores[i].idAbelha > id) {
                sensores[i].idAbelha--; 
            }
            
            sensores[i].id = i + 1; 
        }
        
        salvarDados(); // Salva os dados após a remoção
        printf(Verde "\nAbelha (ID: %d) e seus sensores foram removidos com sucesso!\n" Sem_Cor, id); 
    } else {
        printf(Amarelo "\nRemoção cancelada.\n" Sem_Cor);
    } 
    Pausar();
}

/* :=====: |PRODUÇÃO DAS FUNÇÕES DO MENU DE SENSORES| :=====: */

void GerenciarSensores() { // Inicio do Menu.
    int op;
    do {
        LimparTela();
        printf(Ciano "\n :-=-=-: |GERENCIAR SENSORES| :-=-=-:" Sem_Cor);
        printf("\n1. Cadastrar um Sensor");
        printf("\n2. Listar Sensores");
        printf("\n3. Buscar por ID de uma abelha");
        printf("\n4. Alterar um Sensor");
        printf("\n5. Remover um Sensor");
        printf("\n6. Voltar \n\n");
        printf("Escolha uma opção: ");
        op = OpcoesValidas(1, 6);
        switch(op){
            case 1:
            CadastrarSensor();
            break;
                case 2:
                ListarSensores();
                break;
                        case 3:
                        BuscarID();
                        break;
                            case 4:
                            AlterarSensor();
                            break;
                        case 5:
                        RemocaodeSensor();
                        break;
                case 6:
                break;
            default:
            printf("\nOpção inválida! Por favor selecione a opção correta!\n");
            Pausar();
            break;
        }
    }while (op != 6);
}

void CadastrarSensor() {
    LimparTela();
    
    if (TotalAbelhas == 0) {
        printf(Vermelho "É necessário cadastrar uma abelha antes de poder cadastrar um sensor.\n" Sem_Cor);
        Pausar();
        return;
    }
    
    if (SensoresTotais >= MAX_SENSORES){
        printf(Vermelho "O Número de Sensores atingiu o seu Limite.\n" Sem_Cor);
        Pausar();
        return;
    }

    printf(Amarelo " ----- Cadastrar um Sensor -----" Sem_Cor);
    
    SENSOR NovoSensor;
    
    do{
        printf(Ciano "\n\nTIPO DE SENSOR:\n" Sem_Cor);
        printf(Vermelho "ATENÇÃO: USE SOMENTE LETRAS MINÚSCULAS:\n" Sem_Cor ); 
        printf("|temperatura, umidade ou luminosidade|:\n");
        LerString(NovoSensor.tipo, sizeof(NovoSensor.tipo));
    
        if (strcmp(NovoSensor.tipo , "temperatura" ) != 0 &&
            strcmp(NovoSensor.tipo , "umidade") != 0 &&
            strcmp(NovoSensor.tipo , "luminosidade") != 0 ) {
            printf(Vermelho "Tipo de sensor inválido! Tente Novamente!\n" Sem_Cor); 
        }
        
    }while 
    (strcmp(NovoSensor.tipo , "temperatura") != 0 &&
    strcmp(NovoSensor.tipo , "umidade") != 0 &&
    strcmp(NovoSensor.tipo , "luminosidade") != 0 );

    printf("\nValor da leitura: ");
    while (scanf("%f",  &NovoSensor.valor) != 1) {
        LimpaBuffer();
        printf(Vermelho "Valor inválido. Digite um número: " Sem_Cor);
    }
    LimpaBuffer();

    int id_abelha_valido;
    do{
        printf("\nDigite o ID da abelha para vincular o sensor:\n");
        scanf("%d", &NovoSensor.idAbelha);
        LimpaBuffer();
    
        id_abelha_valido = EncontrarAbelhaPorId(NovoSensor.idAbelha);

        if(id_abelha_valido == -1){
            printf(Vermelho "ID de abelha não encontrado! Tente Novamente\n" Sem_Cor);
        }

    } while (id_abelha_valido == -1);

    int maiorID = 0;
    for (int i = 0; i < SensoresTotais; i++) {
        if (sensores[i].id > maiorID) {
            maiorID = sensores[i].id;
        }
    }
    NovoSensor.id = maiorID + 1;

    sensores[SensoresTotais] = NovoSensor;
    SensoresTotais++;
    
    salvarDados();
        
    printf(Verde "\nSensor de ID: %d , foi registrado com sucesso!\n\n" Sem_Cor, NovoSensor.id);
    Pausar();
}

void ListarSensores() {
    LimparTela();
    
    if (SensoresTotais == 0){
        printf(Vermelho "\nAinda não há sensores cadastrados!\n" Sem_Cor);
        Pausar();
        return;
    }
   
    printf(Amarelo "|:-=-=-:| LISTA DE SENSORES |:-=-=-:|" Sem_Cor);
    for (int i = 0; i < SensoresTotais; i++){
        printf("\n\n------------------------------\n");
        printf(Ciano "ID do Sensor: %d\n" Sem_Cor, sensores[i].id);
        printf("Tipo: %s\n", sensores[i].tipo);
        printf("Valor cadastrado: %.2f\n", sensores[i].valor);
        printf("Vinculado à Abelha de ID: %d", sensores[i].idAbelha);
        printf("\n------------------------------\n\n");
    }
    Pausar();
}

void BuscarID() { // Buscar Sensor por ID da Abelha.
    LimparTela();
    
    if(TotalAbelhas == 0){
        printf(Vermelho "\nNão há abelhas registradas, então não há sensores para buscar.\n" Sem_Cor);
        Pausar();
        return;
    }
    
    printf(Amarelo "\n ----- Buscar Sensor por ID da Abelha -----\n" Sem_Cor);
    printf("Digite o ID da abelha para encontrar sensores vinculados:\n");
    
    int IDprabusca;
    scanf("%d", &IDprabusca);
    LimpaBuffer();

    int encontrado = 0;
    for(int i = 0; i < SensoresTotais; i++){
    
        if(sensores[i].idAbelha == IDprabusca){
            if (encontrado == 0) { // Mostra o cabeçalho apenas uma vez
                printf(Ciano "\n--- Sensores Encontrados para a Abelha ID %d ---\n" Sem_Cor, IDprabusca);
            }
            printf("\nSensor ID: %d\n", sensores[i].id);
            printf("Tipo: %s\n", sensores[i].tipo);
            printf("Valor: %.2f\n", sensores[i].valor);
            printf("------------------------------------\n");
            encontrado = 1;
        }
    }
    
    if (encontrado == 0){
        printf(Vermelho "Nenhum sensor encontrado para a abelha com ID %d.\n" Sem_Cor, IDprabusca);
    }
    Pausar();
}

void AlterarSensor() {
    LimparTela();
    
    if (SensoresTotais == 0){
        printf(Vermelho "\nAinda não há sensores cadastrados!\n" Sem_Cor);
        Pausar();
        return;
    }
    
    printf(Amarelo "\n-------- Alterar Dados de um Sensor --------\n\n" Sem_Cor);
  
    int idParaAlterar;
    printf("Qual sensor você deseja alterar? (Digite o ID do SENSOR)\n");
    scanf("%d", &idParaAlterar);
    LimpaBuffer();

    int indice_sensor = -1;

    for(int i = 0; i < SensoresTotais; i++){
        if(sensores[i].id == idParaAlterar){
            indice_sensor = i;
            break; 
        }
    }

    if (indice_sensor == -1) {
        printf(Vermelho "ID de sensor não encontrado!\n\n" Sem_Cor);
        Pausar();
        return;
    }
    
    int i = indice_sensor;
    printf("\n\n--- Dados Atuais do Sensor ---\n");
    printf(Ciano "ID do Sensor: %d\n" Sem_Cor, sensores[i].id);
    printf("Tipo: %s\n", sensores[i].tipo);
    printf("Valor cadastrado %.2f\n", sensores[i].valor);
    printf("Vinculado à Abelha de ID: %d\n", sensores[i].idAbelha);
    printf("------------------------------\n\n");
    
    printf("Digite o novo valor para o sensor: ");
    while(scanf("%f", &sensores[i].valor) != 1) {
        LimpaBuffer();
        printf(Vermelho "Valor inválido. Digite um número: " Sem_Cor);
    }
    LimpaBuffer();

    salvarDados();

    printf(Verde "\nSensor (ID: %d) alterado com sucesso!\n" Sem_Cor, idParaAlterar);
    printf("O novo valor é: %.2f \n", sensores[i].valor);
    Pausar();
}

void RemocaodeSensor() {
    LimparTela();

    if (SensoresTotais == 0){
        printf(Vermelho "\nAinda não há sensores cadastrados!\n" Sem_Cor);
        Pausar();
        return;
    }
    
    printf(Amarelo "\n-------- Remover um Sensor --------\n\n" Sem_Cor);
    printf("Qual Sensor você quer remover? (Digite o ID do SENSOR)\n");
    
    int remover_id;
    scanf("%d", &remover_id);
    LimpaBuffer();

    int indice_sensor = -1;
    for(int i = 0; i < SensoresTotais; i++){
        if(sensores[i].id == remover_id){
            indice_sensor = i;
            break;
        }
    }

    if (indice_sensor == -1) {
        printf(Vermelho "\nSensor com ID %d não foi encontrado.\n" Sem_Cor, remover_id);
        Pausar();
        return;
    }

    printf("\n\n--- Dados do Sensor a ser Removido ---\n");
    printf(Ciano "ID do Sensor: %d\n" Sem_Cor, sensores[indice_sensor].id);
    printf("Tipo: %s\n", sensores[indice_sensor].tipo);
    printf("Valor: %.2f\n", sensores[indice_sensor].valor);
    printf("---------------------------------------\n");
    
    printf(Vermelho "\nVocê tem certeza que deseja remover este sensor?\n" Sem_Cor);
    printf("Digite 'S' para confirmar ou 'N' para cancelar: ");
    
    char confirma;
    scanf(" %c", &confirma);
    LimpaBuffer();

    if(toupper(confirma) == 'S'){
        for(int i = indice_sensor; i < SensoresTotais - 1; i++){
            sensores[i] = sensores[i + 1];
            sensores[i].id = i + 1;
        }
        SensoresTotais--;
        salvarDados();
        printf(Verde "\nSensor de ID: %d, foi removido com sucesso!\n" Sem_Cor, remover_id);
    } else {
        printf(Amarelo "\nRemoção cancelada.\n" Sem_Cor);
    }
    Pausar();
}

/* :=====: |PRODUÇÃO DAS FUNÇÕES DO MENU DE RELATÓRIOS| :=====: */

void menuRelatorios() {

    int opt;
    do {
        LimparTela();
        printf(Ciano"\n=== Relatórios ===\n"Sem_Cor);
        printf("1. Média geral de produção de mel;\n");
        printf("2. Média de temperatura dos sensores;\n");
        printf("3. Quantidade de abelhas por região.\n");
        printf("4. Voltar\n");
        printf("Escolha uma opção: ");
        opt = OpcoesValidas(1, 4);
        switch (opt) {
            case 1: 
                rel_mediaProducaoMel(); 
                break;
            case 2: 
                rel_mediaTemperaturaSensores(); 
                break;
            case 3: 
                rel_qtdAbelhasPorRegiao(); 
                break;
            case 4: 
                break;
            default: 
                printf(Vermelho "Opção inválida.\n" Sem_Cor);
        }
        
    } while (opt != 4);
}

void rel_mediaProducaoMel() {
    LimparTela();
    printf(Amarelo "--- MÉDIA GERAL DE PRODUÇÃO DE MEL ---\n\n" Sem_Cor);
    if (TotalAbelhas == 0) {
        printf(Vermelho "Nenhuma abelha cadastrada.\n" Sem_Cor);
        Pausar();
        return;
    }
    
    float soma = 0.0f;
    for (int i = 0; i < TotalAbelhas; ++i) {
        soma += abelhas[i].producaoMel;
    }   
    
    printf("Média geral de produção de mel: %.2f kg/mês\n", soma / TotalAbelhas);
    Pausar();
}

void rel_mediaTemperaturaSensores() {
    LimparTela();
    printf(Amarelo "--- MÉDIA DE TEMPERATURA DOS SENSORES ---\n\n" Sem_Cor);
    if (SensoresTotais == 0) {
        printf(Vermelho "Nenhum sensor cadastrado.\n" Sem_Cor);
        Pausar();
        return;
    }
    
    float soma = 0.0f;
    int countTemp = 0;
    for (int i = 0; i < SensoresTotais; ++i) {
        if (strcasecmp(sensores[i].tipo, "temperatura") == 0) {
            soma += sensores[i].valor;
            countTemp++;
        }
    }
    
    if (countTemp == 0) {
        printf(Vermelho "Não há sensores de temperatura cadastrados.\n" Sem_Cor);
    } else {
        printf("Média das leituras dos sensores de temperatura: %.2f\n", soma / countTemp);
    }
    Pausar();
}

void rel_qtdAbelhasPorRegiao() {
    LimparTela();
    printf(Amarelo "--- QUANTIDADE DE ABELHAS POR REGIÃO ---\n\n" Sem_Cor);
    if (TotalAbelhas == 0) {
        printf(Vermelho "Nenhuma abelha cadastrada.\n" Sem_Cor);
        Pausar();
        return;
    }
    
    char regioes[MAX_ABELHAS][30];
    int contagens[MAX_ABELHAS] = {0};
    int regioesCount = 0;

    for (int i = 0; i < TotalAbelhas; ++i) {
        char *reg = abelhas[i].regiao;
        int found = -1;
        
        for (int j = 0; j < regioesCount; ++j) {
            if (strcasecmp(regioes[j], reg) == 0) {
                found = j;
                break;
            }
        }
        
        if (found == -1) {
            if (regioesCount < MAX_ABELHAS) {
                strncpy(regioes[regioesCount], reg, sizeof(regioes[0]) - 1);
                regioes[regioesCount][sizeof(regioes[0]) - 1] = '\0';
                contagens[regioesCount] = 1;
                regioesCount++;
            }
        } else {
            contagens[found]++;
        }
    }
    
    for (int i = 0; i < regioesCount; ++i) {
        printf("Região: %s -> %d abelha(s)\n", regioes[i], contagens[i]);
    }
    Pausar();
} 