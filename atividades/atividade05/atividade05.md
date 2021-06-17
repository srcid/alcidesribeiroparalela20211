# 1ª Questão
```c++
double **mtxMul (double **c, double **a, double **b, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++) {
            c[i][j] = 0.0;
            for (int k = 0; k < n; k++)
                c[i][j] = c[i][j] + a[i][k] * b[k][j];
        }
    return c;
}
```
Tomando como base para nossos calculos uma matriz $100 \times 100$ quantos produtos escalares cada thread executará usando as seguintes abordagens de paralelização

## Apenas o laço 'for' mais externo

$$100 \times 100 \times 100$$

Queremos, então dividir o multiplacador mais a esquerda pelo número de threads. Que nos dá

$$\frac{100}{8} \times 100 \times 100$$

O resultado dessa fração tem resto e esse resto deverá ser distribuido em uma ou mais threads. Assim, podemos concluir que uma threads executara no minimo 120.000 ou no máximo 140.000 produtos escalares, à depender da metódologia usada para distribuir o trabalho.

## Usando a diretiva collapse(2)

Desse modo queremos, então, dividir os dois multiplicadores mais a esquerda pelo número de threads

$$\frac{100 \times 100}{8} \times 100$$

Dessa vez, não há resto na divisão e o trabalho pode ser dividido igualmente entre as threads. Podemos concluir então que cada uma executará 125.00 produtos escalares.

## Regiões paralelas aninhadas

Desse modo, fluxo de threads se bifurca duas vezes

$$\frac{100}{2} \times \frac{100}{4} \times 100$$

Que nos dá, também, 125.000 produtos escalares.

# Comparação de tempo das abordagens acima

Fazendo uso de uma matriz quadrada de $2000 \times 2000$

|  Abordagem  | Tempo (real) | Tempo (sys) |
|:-----------:|:------------:|:-----------:|
|Mais externo |15,387s       |0,148s       |
|Collapse     |15,777s       |0,065s       |
|Aninhados    |18,802s       |0,259s       |

Usando matrizes quadradas de $5000 \times 5000$

|  Abordagem  |  Tempo  |Tempo (sys)|
|:-----------:|:-------:|:---------:|
|Mais externo |4m37,263s|0,819s     |
|Collapse     |4m48,959s|1,114s     |
|Aninhados    |5m39,828s|2,988s     |


# Compilação e uso do programa

## Compilação
```bash
clang++ -fompenmp main.cpp
```
## Uso
Seja A ou 0, 1 ou 2 os algoritimos de paralelos das aborgagens proposta na questão anterior na ordem que foram dispostas e N tamanho da matriz
```bash
./a.out N A
```