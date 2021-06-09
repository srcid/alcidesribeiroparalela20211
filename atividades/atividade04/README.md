# Atividade 04
## Compilação dos códigos
### openmp_some.c
`clang -fopenmp -lm openmp_soma.c`
### pthread_soma.c
`clang -lpthread -lm pthreads_soma.c`
## Como usar
Seja N um inteiro positivo de 32 bits e maior que o numero de threads disponíveis.
`./a.out N`