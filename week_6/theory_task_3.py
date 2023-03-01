from typing import List
import random


def is_sorted(A: List[int]) -> bool:
    for i in range(1, len(A)):
        if A[i - 1] > A[i]:
            return False
    return True


def get_digit(n: int, i: int) -> int:
    return n // (10 ** i) % 10


def counting_sort(A: List[int], index: int, start: int, end: int) -> List[int]:
    count = [0 for _ in range(10)]
    accum = [0 for _ in range(10)]

    for i in range(start, end):
        count[get_digit(A[i], index)] += 1

    for i in range(len(count)):
        accum[i] = count[i] + accum[i - 1]

    output_arr = [0 for _ in range(start, end)]

    for i in range(end - 1, start - 1, -1):
        output_arr[accum[get_digit(A[i], index)] - 1] = A[i]
        accum[get_digit(A[i], index)] -= 1

    for i in range(start, end):
        A[i] = output_arr[i - start]
    
    return A


def counting_sort_length(A: List[int]) -> List[int]:
    count = [0 for _ in range(len(str(max(A))) + 1)]
    accum = [0 for _ in range(len(str(max(A))) + 1)]

    for i in range(len(A)):
        count[len(str(A[i]))] += 1

    for i in range(len(count)):
        accum[i] = count[i] + accum[i - 1]

    output_arr = [0 for _ in range(len(A))]

    for i in range(len(A) - 1, -1, -1):
        output_arr[accum[len(str(A[i]))] - 1] = A[i]
        accum[len(str(A[i]))] -= 1

    return output_arr


def radix_sort(A: List[int], d: int, start: int, end: int) -> List[int]:
    for i in range(d):
        A = counting_sort(A, i, start, end)
    return A


N = 100
ARR: List[int] = [random.randint(1, 512) for _ in range(N)]

print(ARR)
ARR = counting_sort_length(ARR) # Theta(m + n)
print(ARR)
prev = 0
for i in range(1, N):
    if len(str(ARR[i - 1])) < len(str(ARR[i])):
        ARR = radix_sort(ARR, len(str(ARR[prev])), prev, i) # i * (n_i + 9)
        prev = i
ARR = radix_sort(ARR, len(str(ARR[prev])), prev, N)
print(ARR)
print(is_sorted(ARR))
