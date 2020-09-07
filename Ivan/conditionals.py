x = float(input('Введите x: '))
z = int(input())

if x > 10.0 and z == 20:
    print('Правда')
    y = int(input())

    if y > 10:
        print('Много')
    elif y > 0:
        print('Средне')
    elif y > -10:
        print('Маловато')
    elif y > -20:
        print('Совсем маловато')
    else:
        print('Конкретно мало')
else:
    print('Ложь')

    y = int(input())

    if y > 0:
        print('Положительное')