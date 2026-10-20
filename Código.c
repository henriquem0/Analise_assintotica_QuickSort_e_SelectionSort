#include <stdio.h>
#include <stdlib.h> 
#include <time.h>

/*
Autores (N°USP):
Henrique Mohr (18130304)
Pedro Henrique Costa Teixeira (18131584)
*/

/* ============================================================
   VISÃO GERAL DO PROGRAMA
   ------------------------------------------------------------
   Compara o tempo de execução de dois métodos de ordenação:
     - Selection Sort (tipo == 0)
     - Quick Sort     (tipo != 0)
 
   Entrada (stdin):  <tipo> <n>
     tipo = 0  -> mede o Selection Sort
     tipo != 0 -> mede o Quick Sort
     n         -> quantidade de elementos do vetor
 
   Saída: tempo médio (em ms) de UMA ordenação, calculado
   como a média de 10 execuções.
   ============================================================ */
   
//Selectio Sort ou Selecao Direta
/*
Complexidade Do Metodo de Ordenacao
 
Melhor Caso: O(n²)
Pior Caso: O(n²)
Caso Medio: O(n²)
 
Metodo Instavel -- Tem saltos no vetor e por isso
nao e um algoritmo estavel
*/
// Ideia: a cada passo i, procura o MENOR elemento entre v[i..n-1]
// e o coloca na posição i. Ao final de cada passo, v[0..i] já está ordenado.
// Parâmetros: v = vetor a ordenar (ordenado in-place), n = tamanho do vetor.
void selection_sort(int v[], int n);

//Quick Sort
/*
Complexidade Do Metodo de Ordenacao
 
Melhor Caso: O(n*logn)
Pior Caso: O(n²)
Caso Medio: O(n*logn)
 
Metodo Instavel -- Tem saltos no vetor e por isso
nao e um algoritmo estavel
*/
// Ideia (dividir e conquistar): escolhe um pivô, rearranja o vetor de modo que
// os menores fiquem à esquerda e os maiores à direita, e ordena cada lado
// recursivamente.
// Parâmetros: v = vetor, inf = índice inicial do trecho, sup = índice final (inclusive).
// Para ordenar o vetor todo: quicksort(v, 0, n - 1).
void quicksort(int v[], int inf, int sup);

/*Funcao de impressao
- imprime o vetor ja ordenado
*/
// Parâmetros: n = tamanho do vetor, vet = vetor a ser impresso.
void printvet(int n, int vet[]);

/*Funcao que calula a diferenca entre o tempo inicio e o tempo final
- Fazendo a diferenca e retornando uma variavel do tipo double, a fim de ter precisao no resultado do tempo
*/
// struct timespec guarda o tempo em duas partes:
//   tv_sec  = segundos inteiros
//   tv_nsec = nanossegundos restantes
// A função devolve a diferença total em MILISSEGUNDOS.
double calcular_diferenca(struct timespec inicio, struct timespec fim);

int main(void){
    int n;
    int tipo;
    scanf("%d %d", &tipo, &n);
    if(n <= 0){
        return 1;
    }

    // vet_original: guarda a entrada lida, intacta, durante todo o programa.
    // vet_teste:    cópia de trabalho, que é efetivamente ordenada.
    int *vet_original = (int *) malloc(n * sizeof(int));
    int *vet_teste = (int *) malloc(n * sizeof(int));
    
    if (vet_original == NULL || vet_teste == NULL){
        return 1;
    }

    // Lê os n elementos do vetor. Se algum valor não puder ser lido, aborta.
    for(int i = 0; i < n; i++){
        if(scanf("%d", &vet_original[i]) != 1){
            return 1;
        }
    }

    // Acumula a soma dos tempos de todas as execuções (em ms).
    // Dividindo pelo número de execuções, obtém-se a média.
    double tempo_total = 0.0;  
    struct timespec inicio, fim;

    if(tipo == 0){
        // ---- Medição do SELECTION SORT ----
        for(int i = 0; i < 100; i++){
            // Restaura o vetor desordenado original (fora do trecho cronometrado).
            for(int k = 0; k < n; k++) vet_teste[k] = vet_original[k];

            // CLOCK_MONOTONIC: relógio que só avança (não sofre com ajustes de hora
            // do sistema), ideal para medir intervalos de tempo.
            clock_gettime(CLOCK_MONOTONIC, &inicio);
            selection_sort(vet_teste, n);
            clock_gettime(CLOCK_MONOTONIC, &fim);

            tempo_total += calcular_diferenca(inicio, fim);
        }
    }
    else{
        // ---- Medição do QUICK SORT ----
        for(int i = 0; i < 100; i++){
            for(int k = 0; k < n; k++) vet_teste[k] = vet_original[k];

            clock_gettime(CLOCK_MONOTONIC, &inicio);
            quicksort(vet_teste, 0, n - 1);
            clock_gettime(CLOCK_MONOTONIC, &fim);

            tempo_total += calcular_diferenca(inicio, fim);
        }
    }
    
    printvet(n, vet_teste);
    // Tempo médio de UMA ordenação, em milissegundos.
    //printf("Tempo de execução: \n%.5f ms\n", tempo_total/100);    

    free(vet_teste);
    free(vet_original);
    
    return 0;
}


void selection_sort(int v[], int n){

    // i marca o início da parte ainda NÃO ordenada, v[i..n-1].
    // Só vai até n-2: quando sobra um único elemento, ele já está no lugar certo.
    for(int i = 0; i < n-1; i++){

        // min guarda o índice do menor elemento encontrado até agora.
        // Começa supondo que o menor é o próprio v[i].
        int min = i;


        // Laço interno (operação dominante): percorre o restante do vetor
        // procurando um elemento menor que v[min]. Executa n-i-1 comparações
        // por passo, independentemente dos valores do vetor.
        for(int j = i+1; j < n; j++){
            if(v[j] < v[min]){
                min = j; // achou um menor: atualiza o índice (não troca ainda)
            }
        }

        // Só troca se o menor não já estiver na posição i.
        // Isso limita o número de trocas a no máximo n-1.
        if(min != i){
            int aux = v[min];
            v[min] = v[i];
            v[i] = aux;
        }
    }
}

void quicksort(int v[], int inf, int sup) {
    int aux;    // variável auxiliar para as trocas

    // Escolha do pivô: elemento do meio do trecho [inf, sup].
    // O valor do pivô é copiado, pois o elemento pode mudar de posição
    // durante as trocas.
    int meio = (inf + sup) / 2;
    int pivo = v[meio];


    // i varre o trecho da esquerda para a direita;
    // j varre da direita para a esquerda.
    int i = inf;
    int j = sup;

    // Particionamento: ao final, os elementos à esquerda de i são <= pivô
    // e os à direita de j são >= pivô.
    do {
        // Avança i até achar um elemento >= pivô (ele está do lado "errado").
        while (v[i] < pivo) i++;

        // Recua j até achar um elemento <= pivô (também do lado "errado").
        while (v[j] > pivo) j--;

        // Se os ponteiros ainda não se cruzaram, troca os dois elementos
        // e avança os ponteiros para continuar a varredura.
        if (i <= j) {
            aux = v[i];
            v[i] = v[j];
            v[j] = aux;
            i++;
            j--;
        }
    } while (i < j);

    // Após o particionamento, o trecho fica dividido em duas partes:
    //   [inf .. j]  -> elementos <= pivô
    //   [i .. sup]  -> elementos >= pivô
    // (se sobrar algum elemento entre j e i, ele é igual ao pivô e já está
    //  na posição final).
    // Cada parte é ordenada recursivamente, mas só se tiver mais de um
    // elemento (condições abaixo). Elas também são o caso base da recursão:
    // trechos vazios ou com um só elemento não geram novas chamadas.
    if (j > inf) quicksort(v, inf, j); // ordena a parte esquerda
    if (i < sup) quicksort(v, i, sup); // ordena a parte direita
}

void printvet(int n, int vet[]){
    for(int i = 0; i < n; i++){
        printf("%d ", vet[i]);
    }
}

double calcular_diferenca(struct timespec inicio, struct timespec fim){
    
    // Diferença de segundos convertida para ms + diferença de nanossegundos
    // convertida para ms (1 ms = 10^6 ns).
    return (fim.tv_sec - inicio.tv_sec) * 1000.0 + (fim.tv_nsec - inicio.tv_nsec) / 1e6;
}