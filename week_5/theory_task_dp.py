def dp(R, M, A):
    DP = [[0 for j in range(M + 1)] for i in range(R + 1)]
    for i in range(1, R + 1):
        for j in range(1, M + 1):
            for k in range(j + 1):
                DP[i][j] = max(DP[i][j], DP[i - 1][j - k] + A[k][i - 1])
    return DP[R][M]


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
print(dp(R, M, E))