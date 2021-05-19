# Atividade 02
## Sequencial
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