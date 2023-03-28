import subprocess
import random
from tqdm import tqdm

pbar = tqdm()
while True:
    N = random.randint(10**5, 10**6)
    inp = f'{N}\n'
    i = 0
    while i < N:
        inp += f"{random.randint(-2 ** 21, 2 ** 21)} {random.randint(-2 ** 21, 2 ** 21)} {random.randint(-2 ** 21, 2 ** 21)} {random.randint(-2 ** 21, 2 ** 21)}\n"
        i += 1
    try:
        result = subprocess.run(
            ['A.exe'],
            input=inp.encode(),
            stdout=subprocess.PIPE,
            timeout=5
        )
        if result.returncode != 0:
            raise Exception('ERROR')
    except Exception:
        with open('error.txt', 'w') as out:
            out.write(inp)
        break
    pbar.update(1)
