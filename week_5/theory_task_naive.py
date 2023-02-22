def solver(t, M, R, E):
    if sum(t) == M:
        return sum(E[t[i]][i] for i in range(R))
    maxx = 0
    for i in range(R):
        new_t = tuple(t[j] + 1 * (i == j) for j in range(R))
        result = solver(new_t, M, R, E)
        maxx = max(maxx, result)
    return maxx


M = 5
R = 3
E = [
    [0, 0, 0],
    [45, 50, 20],
    [70, 70, 45],
    [90, 80, 75],
    [105, 100, 110],
    [120, 130, 150]
]
print(solver(tuple(0 for _ in range(R)), M, R, E))