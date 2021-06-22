# Traveling Salesman Problem (TSP)
The Traveling Salesman Problem (TSP) it's a traditional optimization problem in the literature. The problem consists of telling the shortest possible route to visit a series of cities exactly once and then returns to the original city. 

That being so, this repository contains different approaches to solve TSP.

## Different approaches

### 1. Metaheuristics:
For instances which the number of vertices that we need to cover is very big, it's necessary the use of a metaheuristic to solve the issue, which doesn't guarantee the optimal solution, but in general results on good anwsers, otherwise, the computer can't give the optimal solution in a feasible time. Therefore, it was used the metaheuristic GILS-RVND, which combines components of Greedy Randomized Adaptive Search Procedure (GRASP), Iterated Local Search (ILS) and Variable Neighborhood Descent with Random neighborhood ordering (RVND).

* **Results:** Once implemented correctly, the algorithm finds optimal solutions to instances with up to 300 customers most of the times it's executed. All the execution results were registered on the folder [benchmark](https://github.com/renatamendesc/TSP/blob/main/GILS-RVND/benchmark/bm_final.txt).

### 2. Branch and Bound:
**Branch and Bound** consists in an algorithm capable of solving optimization problems in an **exact method**. In this case, we consider a TSP version less restricted, which by a metaheuristc (it was used the **Hungarian Algoritm**) previously implemented, creates unfeasable solutions. By that results, we build a tree with solutions, and gradually we add constraints until we get feasible results. Once we search the entire tree, we can finally find the optimal solution.

* **Results:** Branch and Bound is an incredibly inefficient method. This happens because to find the optimal solution for the problem, we need to search the entire tree that was built, which usually gets very extense, making it impossible for the computer to find the optimal value in a good execution time for bigger instances.

