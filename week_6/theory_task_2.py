# arr = [2, 5, 3, 0, 2, 3, 0, 3]
# sattelite = ['a', 'b', 'c', 'd', 'e', 'f', 'g', 'h']
arr = [6, 0, 6, 3, 0, 6, 1, 3, 2, 0]
sattelite = ['S', 'I', 'A', 'Y', 'S', '!', 'U', 'D', 'D', 'T']

count = [0 for _ in range(max(arr) + 1)]
accum = [0 for _ in range(max(arr) + 1)]

for i in range(len(arr)):
    count[arr[i]] += 1

for i in range(len(count)):
    accum[i] = count[i] + accum[i - 1]

print(count)
print(accum)

output_arr = [0 for _ in range(len(arr))]
output_sattelite = ['' for _ in range(len(arr))]

for i in range(len(arr) - 1, -1, -1):
    output_arr[accum[arr[i]] - 1] = arr[i]
    output_sattelite[accum[arr[i]] - 1] = sattelite[i]
    accum[arr[i]] -= 1

print(count)
print(accum)

print(output_arr)
print(output_sattelite)

