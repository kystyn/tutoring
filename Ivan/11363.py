s = str()
pairs = [[]]
sum = 0
while True:
    s = input().split()
    if len(s) != 2:
        break
    a = int(s[0])
    b = int(s[1])
    pairs.append([a, b])
    if a > b:
        sum += a
    else:
        sum += b

print(sum)