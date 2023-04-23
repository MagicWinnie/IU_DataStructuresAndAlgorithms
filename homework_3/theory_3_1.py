from typing import List
from copy import deepcopy
from pprint import pprint
from math import log

INF = 100000


def GetMaxLength(W: List[List[float]]) -> int:
    max_length = -1
    for i in range(len(W)):
        max_length = max(max_length, len(str(W[i])))
    return max_length


def IsNegativeCycle(W: List[List[float]]) -> bool:
    n = len(W)
    D = deepcopy(W)
    for i in range(n):
        for j in range(n):
            if D[i][j] != INF:
                D[i][j] = log(1.0 / D[i][j])
    for k in range(n):
        for i in range(n):
            for j in range(n):
                D[i][j] = min(D[i][j], D[i][k] + D[k][j])
    for i in range(n):
        if D[i][i] < 0:
            return True
    return False


adjMatrix: List[List[float]] = [
    [1, 64, INF],  # Apple share
    [INF, 1, 1.8],  # Indian rupee
    [0.009, INF, 1],  # Yen
]
print(IsNegativeCycle(adjMatrix))
