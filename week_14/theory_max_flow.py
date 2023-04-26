from typing import List, Dict, Tuple, Union
from pprint import pprint


def Edmonds_Karp(C: List[List[int]], s: int, t: int) -> int:
    n = len(C)
    F: List[List[int]] = [[0] * n for i in range(n)]  # residual network
    path = bfs(C, F, s, t)
    it_count = 1
    while path != None:
        flow = min(C[u][v] - F[u][v] for u, v in path)
        for u, v in path:
            F[u][v] += flow
            F[v][u] -= flow

        print(f"-------Iteration {it_count}-------")
        print("Path:", path)
        print("Current flow:", sum(F[s][i] for i in range(n)))
        print("Residual network:")
        pprint(F)

        path = bfs(C, F, s, t)
        it_count += 1
    return sum(F[s][i] for i in range(n))


def bfs(C: List[List[int]], F: List[List[int]], s: int, t: int)\
        -> Union[List[Tuple[int, int]], None]:
    queue: List[int] = [s]
    paths: Dict[int, List[Tuple[int, int]]] = {s: []}
    if s == t:
        return paths[s]
    while queue:
        u = queue.pop(0)
        for v in range(len(C)):
            if (C[u][v] - F[u][v] > 0) and v not in paths:
                paths[v] = paths[u] + [(u, v)]
                if v == t:
                    return paths[v]
                queue.append(v)
    return None


def main():
    # make a capacity graph
    #     A  B  C  D  E  F  G  H
    C = [[0, 0, 0, 0, 5, 0, 0, 0],  # A
         [2, 0, 9, 6, 7, 1, 0, 0],  # B
         [0, 0, 0, 5, 0, 0, 0, 0],  # C
         [0, 0, 0, 0, 0, 0, 0, 2],  # D
         [0, 0, 0, 0, 0, 6, 2, 0],  # E
         [0, 0, 0, 3, 0, 0, 0, 2],  # F
         [0, 0, 0, 0, 0, 4, 0, 7],  # G
         [0, 0, 0, 0, 0, 0, 0, 0]]  # H

    source = 1  # B
    sink = 7    # H
    max_flow_value = Edmonds_Karp(C, source, sink)
    print("Maximum flow:", max_flow_value)


if __name__ == "__main__":
    main()
