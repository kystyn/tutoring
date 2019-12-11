#include <iostream>
#include <string>
#include <cmath>
#include <algorithm>

using namespace std;

bool checkCorrect( std::string const &n, int sys )
{
    if (sys <= 10)
    {
        for (size_t i = 0; i < n.length(); i++)
            if (!(n[i] >= '0' && n[i] < '0' + sys))
                return false;
    }
    else if (sys <= 36)
    {
        for (size_t i = 0; i < n.length(); i++)
            if (!((n[i] >= '0' && n[i] <= '9') ||
                  (n[i] >= 'a' && n[i] <= 'a' + sys - 11)) ||
                  (n[i] >= 'A' && n[i] <= 'A' + sys - 11))
                        return false;
    }
    else
        return false;

    return true;
}

int toDecimalDigit( char s, int sys )
{
    if (sys <= 10)
        return s - '0';
    else
    {
        if (s >= '0' && s <= '9')
            return s - '0';
        if (s >= 'a' && s <= 'z')
            return s - 'a' + 10;
        if (s >= 'A' && s <= 'Z')
            return s - 'A' + 10;
    }
    return 0;
}

char fromDecimalDigit( int digit )
{
    if (digit < 10)
        return char('0' + digit);

    return char('A' + digit - 10);
}

int toDecimal( std::string const &n, int sys )
{
    int res = 0;


    for (int i = 0; i < int(n.length()); i++)
        res += toDecimalDigit(n[size_t(i)], sys) * int(pow(sys, int(n.length()) - 1 - i));

    return res;
}

std::string fromDecimal( int n, int sys )
{
    std::string res;

    if (n < 0)
        return "Negative number";

    while (n != 0)
    {
        int digit = n % sys;

        res += fromDecimalDigit(digit);
        n /= sys;
    }

    std::reverse(res.begin(), res.end());

    return res;
}

void menu( void )
{
    string n1, n2;
    int sys;
    bool live = true;

    cout << "Введите число 1: ";
    cin >> n1;
    cout << "Введите число 2: ";
    cin >> n2;
    cout << "Введите систему счисления: ";
    cin >> sys;

    if (!checkCorrect(n1, sys) ||
        !checkCorrect(n2, sys))
        std::cout << "Некорректный формат числа в заданной системе счисления\n";

    std::string res;
    while (live)
    {
        cout <<  "0 - выход\n" <<
                 "1 - Сложение\n" <<
                 "2 - Вычитание\n" <<
                 "3 - Умножение\n" <<
                 "4 - Деление\n" <<
                 "5 - Правка числа\n" <<
                 "Введите соответствующую команду: ";
        char c;
        //cin >> c;

        int cursorPos = 0;
        getc(stdin);
        c = getc(stdin);
        switch (c)
        {
        case '1':
            res = fromDecimal(toDecimal(n1, sys) + toDecimal(n2, sys), sys);
            break;
        case '2':
            res = fromDecimal(toDecimal(n1, sys) - toDecimal(n2, sys), sys);
            break;
        case '3':
            res = fromDecimal(toDecimal(n1, sys) * toDecimal(n2, sys), sys);
            break;
        case '4':
            res = fromDecimal(toDecimal(n1, sys) / toDecimal(n2, sys), sys);
            break;
        case '5':
            if (res.empty())
                cout << "Сначала примените оператор" << endl;
            else
            {
                bool inLive = true;
                while (inLive)
                {
                    getc(stdin);
                    char c = getc(stdin);
                    switch (c)
                    {
                    case '\n':
                        inLive = false;
                        break;
                    case '/':
                        if (cursorPos > 0)
                            cursorPos--;
                        break;
                    case '*':
                        if (cursorPos <= int(res.length()))
                            cursorPos++;
                        break;
                    default:
                        if (cursorPos < int(res.length() - 1))
                            res[cursorPos] = c;
                        else
                            res += c;
                        cursorPos = min(cursorPos + 1, int(res.length() - 1));
                    }
                    if (cursorPos < int(res.length()))
                        cout << res.substr(0, cursorPos) << '_' << res.substr(cursorPos + 1, res.length());
                    else
                        cout << res << '_';
                }
            }
            break;
        case 0:
            live = false;
            break;
        default:
            cout << "Неправильная команда" << endl;
        }
        cout << "Результат: " << res << endl;
    }
}

int main()
{
    menu();
    return 0;
}
