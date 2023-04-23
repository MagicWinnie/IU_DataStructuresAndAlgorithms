from typing import List
from copy import deepcopy
from pprint import pprint

INF = 100000


def GetMaxLength(W: List[List[int]]) -> int:
    max_length = -1
    for i in range(len(W)):
        max_length = max(max_length, len(str(W[i])))
    return max_length


def FloydWarshall(W: List[List[int]], debug: bool = False) -> List[List[int]]:
    n = len(W)
    D = deepcopy(W)
    if debug:
        print("Iteration 0:")
        pprint(D, width=GetMaxLength(D) + 10)
    for k in range(n):
        for i in range(n):
            for j in range(n):
                D[i][j] = min(D[i][j], D[i][k] + D[k][j])
        if debug:
            print(f"Iteration {k + 1}:")
            pprint(D, width=GetMaxLength(D) + 10)
    return D


with open('theory_1_1') as infile:
    adjMatrix = [list(map(int, line.split())) for line in infile]

FloydWarshall(adjMatrix, True)

# print("TEST:")
# adjMatrix = [
#     [0, 3, 8, -4, INF],
#     [INF, 0, INF, 7, 1],
#     [INF, 4, 0, INF, INF],
#     [INF, INF, INF, 0, 6],
#     [2, INF, -5, INF, 0]
# ]
# FloydWarshall(adjMatrix, True)
