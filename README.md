# Traveling Salesman Problem (TSP)
O Traveling Salesman Problem (TSP), também conhecido como Problema do Caixeiro Viajante, se trata de um clássiso problema de otimização combinatória existente na literatura. Esse problema consiste em determinar a menor rota possível para percorrer uma série de cidades, visitando cada uma delas uma única vez, e por fim, retornando à cidade de inicial.

Sendo assim, este repósitório contém diferentes formas de resolução para o TSP.

## Abordagens de resolução

### 1. Meta-Heurística:
Para casos onde o número de destinos que se deseja percorrer é muito grande, faz-se necessário o uso de uma meta-heurística para resolver a situação, que não garante a solução ótima, porém, no geral, retorna soluções satisfatórias. Caso contrário, o computador não é capaz de determinar a melhor solução em um tempo viável. Dessa maneira, na resolução do de forma meta-heurística, foi utilizado a **GILS-RVND**, que une componentes das heurísticas GRASP (Greedy Randomized Adaptive Search Procedure), ILS (Iterated Local Search) e RVND (Randomized Variable Neighborhood Descent).

* **Resultados:** Uma vez implementado corretamente, o algoritmo é capaz de obter o valor ótimo para instâncias de até 300 cidades na maioria das vezes em que é executado. Todos os resultados da execução do programa foram registrados na pasta [benchmark](https://github.com/renatamendesc/TSP/blob/main/GILS-RVND/benchmark/bm_final.txt).

### 2. Branch and Bound:
O **Branch and Bound** consiste em um algoritmo capaz de resolver problemas combinatórios de **maneira exata**. Nesse caso, considera-se uma versão menos restrita do TSP, onde a partir de uma meta-heurística (nesse caso, foi utilizado o **Algoritmo Húngaro**) já previamente implementada, geramos soluções inviáveis. A partir desses resultados, construimos uma árvore de soluções e gradativamente adicionamos restrições, até obtermos soluções viáveis. Uma vez tendo analisado toda a árvore, conseguimos encontrar a solução ótima.

* **Resultados:** O Branch and Bound é considerado um método de resolução extremamente ineficaz e custoso. Isso ocorre porque para acharmos o melhor resultado para o problema, é preciso varrer toda a árvore que foi gerada, que costuma ficar muito extensa, impossibilitando o computador de retornar o valor ótimo em um tempo de execução razoável, quando se trata de instâncias maiores.

