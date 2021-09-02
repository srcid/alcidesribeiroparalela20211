# Atividade 13

> Correção: 2,0
> 
> Como havíamos discutido, acho que os tempos de linemajor ficaram piores porque na versão "normal" você utilizou o vector do C++, que é um muito bom em alocação.
> Mas como não falei nada, está OK.

## Comparação
Processador intel i5-8300H
|N|NORMAL|LINEMAJOR|
|---|---|---|
64|0:00.00|0:00.00|
256|0:00.27|0:00.15|
1024|0:17.93|0:27.77|
2048|3:04.31|6:02.26|

## Compilação da questão 02
>`mpicc -mrdrnd questao02.c`
