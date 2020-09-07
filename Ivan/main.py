def F(n):
    if n > 0:
        F(n - 4)
        print(n)
        F(n // 3)
        print(n)

F(9)