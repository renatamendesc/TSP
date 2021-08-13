# Traveling Salesman Problem (TSP)
The Traveling Salesman Problem (TSP) is a traditional optimization problem in the literature. The problem consists on telling the shortest possible route to visit a series of cities exactly once and then returns to the original city. 

This repository contains different approaches to solve TSP.

## Different approaches

### 1. Metaheuristics:
For instances which the number of vertices that we need to cover is very big, it's necessary the use of a metaheuristic to solve the issue, which doesn't guarantee the optimal solution, but in general results on good anwsers, otherwise, the computer can't give the optimal solution in a feasible time. Therefore, it was used the metaheuristic GILS-RVND, which combines components of Greedy Randomized Adaptive Search Procedure (GRASP), Iterated Local Search (ILS) and Variable Neighborhood Descent with Random neighborhood ordering (RVND).

* **Results:** Once implemented correctly, the algorithm finds optimal solutions to instances with up to 300 customers most of the times it's executed. All the execution results were registered on the folder [benchmark](https://github.com/renatamendesc/TSP/blob/main/GILS-RVND/benchmark/bm_final.txt).

### 2. Branch and Bound:
**Branch and Bound** consists on an algorithm capable of solving optimization problems in an **exact method**. In this case, we consider a TSP version less restricted, which by a metaheuristc (it was used the **Hungarian Algorithm**) previously implemented, creates unfeasable solutions. By that results, we build a tree with solutions, and gradually add constraints until we get feasible results. Once we search the entire tree, we can finally find the optimal solution.

* **Results:** Branch and Bound is an incredibly inefficient method. This happens because to find the optimal solution for the problem, we need to search the entire tree that was built, which usually gets very extense, making it impossible for the computer to find the optimal value in a good execution time for bigger instances.

### 3. Lagrangian Relaxion:

Another exact method that can be used when solving optimization problems is the **Lagrangian Relaxation**. This time we're are still going to consider a less restricted version of the problem, but at the same time, the method penalizes violations that were made when ignoring constraints. In practice, this relaxed problem can often be solved more easily than the original problem.

* **Results:** Compared to heuristics, the Lagrangian Relaxation is still not a very efficient method, even though that it can give us the optimal solution to smaller instances in a few seconds. But when we're trying to solve bigger instances, from about 100 customers, we may have problems with finding the optimal solution in a good execution time.