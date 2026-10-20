# Análise Empírica e Assintótica: Selection Sort × Quick Sort

Projeto Prático 1 da disciplina **Introdução à Ciência de Computação II (SCC0201)**, do Instituto de Ciências Matemáticas e de Computação (**ICMC-USP**, São Carlos).

- **Docente:** Marcelo Garcia Manzato
- **Autores:** Henrique Mohr e Pedro Henrique Costa Teixeira

## Sobre o projeto

O objetivo é implementar dois algoritmos de ordenação, um com complexidade `O(n²)` e outro com `O(n log n)`, e estudá-los de forma **empírica** (medindo tempos de execução) e **assintótica** (análise teórica), apresentando os resultados em tabelas e gráficos.

| Algoritmo | Classe de complexidade | Observação |
|---|---|---|
| **Selection Sort** (seleção direta) | `O(n²)` em todos os casos | Sempre faz `n(n-1)/2` comparações |
| **Quick Sort** | `O(n log n)` no melhor caso e no caso médio; `O(n²)` no pior caso | Pivô no elemento central, particionamento por dois ponteiros |

Ambos foram implementados em **C**, com vetores recebidos por parâmetro e sem uso de bibliotecas de ordenação prontas.

## Metodologia

- **Medição:** `clock_gettime` com `CLOCK_MONOTONIC` (biblioteca `time.h`), medindo **somente** a chamada da função de ordenação (sem leitura ou escrita de dados). Tempos em milissegundos.
- **Repetições:** cada configuração (algoritmo × tamanho × tipo de entrada) foi executada **100 vezes**, e o tempo reportado é a **média aritmética**.
- **Entrada:** o vetor de cada configuração é gerado uma única vez e **copiado** antes de cada execução. Assim, as repetições capturam principalmente variações do ambiente de execução, e não de sequências aleatórias diferentes.
- **Tamanhos de entrada:** 100, 500, 1000, 5000 e 10000 elementos.
- **Tipos de entrada:**
  1. Vetor ordenado crescentemente
  2. Vetor ordenado decrescentemente
  3. Vetor em ordem aleatória
  4. Vetor em ordem aleatória com valores repetidos

Trecho usado para medir o tempo:

```c
struct timespec inicio, fim;
clock_gettime(CLOCK_MONOTONIC, &inicio);

// função a ser medida

clock_gettime(CLOCK_MONOTONIC, &fim);
double tempo_ms = (fim.tv_sec - inicio.tv_sec) * 1000.0 +
                  (fim.tv_nsec - inicio.tv_nsec) / 1e6;
```

## Implementações

### Selection Sort

```c
void selection_sort(int v[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min = i;
        for (int j = i + 1; j < n; j++) {
            if (v[j] < v[min]) {
                min = j;
            }
        }
        if (min != i) {
            int aux = v[min];
            v[min] = v[i];
            v[i] = aux;
        }
    }
}
```

### Quick Sort

```c
void quicksort(int v[], int inf, int sup) {
    int aux;
    int meio = (inf + sup) / 2;
    int pivo = v[meio];

    int i = inf;
    int j = sup;
    do {
        while (v[i] < pivo) i++;
        while (v[j] > pivo) j--;
        if (i <= j) {
            aux = v[i];
            v[i] = v[j];
            v[j] = aux;
            i++;
            j--;
        }
    } while (i < j);

    if (j > inf) quicksort(v, inf, j);
    if (i < sup) quicksort(v, i, sup);
}
```

## Resultados

Tempo médio de execução em **milissegundos (ms)**.

### Selection Sort

| Tipo de entrada | 100 | 500 | 1000 | 5000 | 10000 |
|---|---|---|---|---|---|
| Ordenados | 0,01627 | 0,39031 | 1,59293 | 35,91076 | 128,61006 |
| Ordenados decrescentemente | 0,01627 | 0,39248 | 1,60923 | 36,22206 | 131,35413 |
| Aleatorizados | 0,01624 | 0,45804 | 1,60736 | 35,09706 | 130,85628 |
| Aleatorizados c/ repetição | 0,01628 | 0,39638 | 1,56349 | 34,95176 | 129,77752 |

### Quick Sort

| Tipo de entrada | 100 | 500 | 1000 | 5000 | 10000 |
|---|---|---|---|---|---|
| Ordenados | 0,00349 | 0,01943 | 0,04027 | 0,30838 | 0,61654 |
| Ordenados decrescentemente | 0,00348 | 0,02062 | 0,04034 | 0,31576 | 0,62811 |
| Aleatorizados | 0,00351 | 0,02235 | 0,04188 | 0,31030 | 0,60993 |
| Aleatorizados c/ repetição | 0,00350 | 0,01874 | 0,04360 | 0,32185 | 0,61611 |

### Principais observações

- **Selection Sort:** o tempo praticamente **não depende da organização dos dados**, apenas do tamanho. Dobrar `n` faz o tempo quase quadruplicar, e de `n = 100` para `n = 10000` (100× mais elementos) o tempo cresce cerca de **8000×**, compatível com `O(n²)`.
- **Quick Sort:** para `n = 10000`, os quatro tipos de entrada ficaram entre ~0,61 ms e ~0,63 ms (diferença de ~3%). De `n = 100` para `n = 10000` o tempo cresceu cerca de **175×**, um pouco acima do linear (100×) e muito abaixo do quadrático (10.000×), característico de `O(n log n)`.
- **Comparação direta:** com 10000 elementos, o Quick Sort levou cerca de 0,6 ms contra cerca de 130 ms do Selection Sort, ou seja, **mais de 200× mais rápido**, e a diferença tende a crescer com `n`.
- **Pior caso do Quick Sort:** não apareceu nos testes. Com o pivô no elemento central, entradas já ordenadas e decrescentes geram partições equilibradas.

### Gráficos

> Adicione aqui as imagens dos gráficos gerados (por exemplo, em uma pasta `imagens/`):
>
> ```markdown
> ![Selection Sort](imagens/selection_sort.png)
> ![Comparação entre os métodos](imagens/comparacao.png)
> ![Intervalo com melhor observação](imagens/intervalo_100_500.png)
> ```

## Análise teórica

**Selection Sort**
- Operação dominante: a comparação `v[j] < v[min]` no laço interno, executada `n - i - 1` vezes para cada `i`, totalizando `n(n-1)/2` comparações em **qualquer** entrada.
- Melhor, médio e pior caso: `O(n²)`. A quantidade de trocas varia, mas não altera a ordem de complexidade.
- Espaço adicional: `O(1)`.

**Quick Sort**
- Pior caso (partições muito desbalanceadas): `T(n) = T(n-1) + O(n)`, logo `O(n²)`.
- Melhor caso (partições equilibradas): `T(n) = 2T(n/2) + O(n)`, logo `O(n log n)`.
- Caso médio: `O(n log n)`.
- Espaço adicional (pilha de recursão): `O(log n)` no melhor caso e até `O(n)` no pior caso.

## Análise do baseline de dados

O relatório também analisa, a partir dos tempos fornecidos pelo professor, o comportamento de quatro algoritmos desconhecidos (A, B, C e D), estimando a ordem de crescimento de cada um pela razão entre os tempos de `n = 1000` e `n = 10000`. Os detalhes estão na seção 3.6 do relatório.

## Compilação e execução

> Ajuste os nomes dos arquivos conforme a estrutura do repositório.

```bash
gcc -o projeto main.c -Wall
./projeto
```

## Estrutura do repositório

> Ajuste conforme os arquivos que você anexar.

```
.
├── README.md
├── Relatorio_projeto.pdf      # Relatório completo do projeto
├── main.c                     # Programa principal e medição de tempos
├── selection_sort.c           # Implementação do Selection Sort (se separado)
├── quicksort.c                # Implementação do Quick Sort (se separado)
└── imagens/                   # Gráficos usados no relatório
```

## Relatório completo

O relatório em PDF contém a metodologia detalhada, as análises empírica e teórica, as discussões propostas no enunciado, as dificuldades encontradas e a conclusão.

## Referências

- MANZATO, Marcelo Garcia. *Material de aula – Introdução à Ciência de Computação II (SCC0201)*. Instituto de Ciências Matemáticas e de Computação, Universidade de São Paulo, 2026.

## Uso de LLM

Foi utilizada a LLM Claude como ferramenta de apoio à revisão e complementação das análises, após a elaboração das análises iniciais pelo grupo. As conclusões e interpretações foram avaliadas pelo grupo, e as respostas da LLM não foram consideradas automaticamente corretas.
