from random import randint
import time

def foil(a, b):
    # n = max(len(a), len(b))
    c = [0] * (len(a) + len(b))
    for i in range(len(a)):
        for j in range(len(b)):
            c[i + j] += a[i] * b[j]
    return c

def karatsuba(a, b):
    def poly_sum(x, y, u=1, v=1):
        if len(x) < len(y):
            x, y = y, x
            u, v = v, u
        return [u * i + v * j for i, j in zip(x, y)] + [u * i for i in x[len(y):]]
    n = max(len(a), len(b))

    if len(a) == 0 or len(b) == 0:
        return []
    if n == 1:
        return [a[0] * b[0]]
    m = n // 2
    a0, a1 = a[:m], a[m:]
    b0, b1 = b[:m], b[m:]
    aa = poly_sum(a0, a1)
    bb = poly_sum(b0, b1)
    c0 = karatsuba(a0, b0)
    c1 = karatsuba(a1, b1)
    cc = karatsuba(aa, bb)
    t = poly_sum(poly_sum(c0, c1), cc, -1)
    return poly_sum(c0, poly_sum([0] * m + t, [0] * (2 * m) + c1))

n0 = 1024

def karatsuba_modified(a, b):
    def poly_sum(x, y, u=1, v=1):
        if len(x) < len(y):
            x, y = y, x
            u, v = v, u
        return [u * i + v * j for i, j in zip(x, y)] + [u * i for i in x[len(y):]]
    n = max(len(a), len(b))

    if len(a) == 0 or len(b) == 0:
        return []
    
    if len(a) <= n0 and len(b) <= n0:
        return foil(a, b)

    if n == 1:
        return [a[0] * b[0]]
    m = n // 2
    a0, a1 = a[:m], a[m:]
    b0, b1 = b[:m], b[m:]
    aa = poly_sum(a0, a1)
    bb = poly_sum(b0, b1)
    c0 = karatsuba_modified(a0, b0)
    c1 = karatsuba_modified(a1, b1)
    cc = karatsuba_modified(aa, bb)
    t = poly_sum(poly_sum(c0, c1), cc, -1)
    return poly_sum(c0, poly_sum([0] * m + t, [0] * (2 * m) + c1))

def poly_eq(a, b):
    if len(a) < len(b):
        a, b = b, a
    for i in range(len(a)):
        t = b[i] if i < len(b) else 0
        if a[i] != t:
            return False
    return True

for i in range(0, 17):
    a = [randint(0, 1) for _ in range(2 ** i)]
    b = [randint(0, 1) for _ in range(2 ** i)]
    st = time.time()
    c = karatsuba_modified(a, b)
    ed = time.time()
    print(ed - st)
