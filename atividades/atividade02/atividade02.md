# Atividade 02
## Sequencial
### Correção: 1,0
```c
int total = 0;
int numeros[N];
for (int i = 0; i < N; i++) {
    total = total + numeros[i];
}
```
## Paralelo
```c++
int total = 0;
int total_parte_1 = 0;
int total_parte_2 = 0;
bool terminou_1 = false;
bool terminou_2 = false;
int numeros[N];

if (get_id() == 0) then
|   for (int i = 0; i < N/2; i++)
|   |   total_parte_1 = total_parte_1 + numeros[i];
|   terminou_1 = true;
else if (get_id() == 1) then
|   for (int j = (N/2)+1; j < N; j++)
|   |   total_parte_2 = total_parte_2 + numeros[j];
|   terminou_2 = true;

if (get_id() == 0) then
|   while (!(terminou_1 && terminou_2))
|   |   do_nothing();
|   total = total_parte_1 + total_parte_2;
```
## Questão Extra:

Considere o seguinte código serial:

```c++
int numeros[N];
for (int i = 1; i < N; i++)
   numeros[i] = numeros[i] + numeros[i-1];
```

Resolve o mesmo problema para o problema acima. Valendo 0,5 pontos extra na primeira. 

### O que importa é apenas o último resultado?
```c++
int numeros[N];
int acc1 = 0, acc2 = 0;
bool terminou1 = false, terminou2 = false;
if (get_id() == 0) then
|   for (int i = 0; i < N/2; i++)
|   |   acc1 += numeros[i];
else if (get_id() == 1) then
|   for (int j = (N/2)+1; j < N; j++)
|   |   acc2 += numeros[j];
if (get_id() == 1) then
|   while (!(terminou1 && terminou2))
|   |   do_nothing();
|   numeros[N-1] = acc1 + acc2;
```
### O que importa é o array?
Suponha que p0 e p1 executam um tarefa nas respectivas unidades de processamento.
Suponha ainda que existe uma função incremente e para soma um valor dado a todos os elementos de um conjunto num intervalo.
```c++
int p, r, p1, q, p2;
bool terminou1 = false;
    
p = 0;
r = N-1;

p1 = 0;
q = N/2;
p2 = q+1;
    
if (get_id() == 0) {
    while (p1 <= q) {
        arr[++p] += arr[p];
    }
} else {
    while (p2 <= r) {
        arr[++p2] += arr[p2];
    }
    
    while (!terminou1);
    
    p0(incremente(arr, 0, q, arr[q]));
    p1(incremente(arr, q+1, r, arr[q]))
}
```