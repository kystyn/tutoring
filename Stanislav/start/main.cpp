#include <iostream>

void readIntoArray( int *a, int N )
{
    // for (начальное условие; условие выполнения цикла; команда в конце итерации)
    // a[i] - доступ к i-й ячейке массива по имени a
    std::cout << "Введите массив из " << N << " элементов: ";
    for (int i = 0; i < N; i++)
        std::cin >> a[i];
}

void outputArray( int *a, int N )
{
    for (int i = 0; i < N; i++)
        std::cout << a[i] << ' ';

    std::cout << std::endl; // endl - end of line
}

void change( int *a, int N )
{
    for (int i = 0; i < N; i++)
        if (a[i] % 2 == 0)
            a[i] = 0;
        else // if (a[i] % 2 == 1)
            a[i] = 50;
}

int evalZeroCount( int *a, int N )
{
    int zeroCount = 0;

    for (int i = 0; i < N; i++)
        if (a[i] == 0)
            zeroCount++;

    return zeroCount;
}

int main()
{
    int
            array1[4],
            array2[8];

    readIntoArray(array1 + 1, 6);
    readIntoArray(array2, 8);

    change(array1, 6);
    change(array2, 8);

    std::cout << "Array of 6: ";
    outputArray(array1, 6);
    std::cout << "Zero count: " << evalZeroCount(array1, 6) << std::endl;

    std::cout << "Array of 8: ";
    outputArray(array2, 8);
    std::cout << "Zero count: " << evalZeroCount(array2, 8) << std::endl;

    return 0;
}
