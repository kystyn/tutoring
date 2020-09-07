#include <iostream>
#include <iomanip>

static const int MAX_SIZE = 10;

using namespace std;

// d** -1..1
//CW - clockwise - по часовой стрелке
//CCW - counter-clockwise - против часовой стрелки

/*
 * xs, ys - start position
 * dirNum - number of starting direction, from northern
 * bool CW - true if CW, false if CCW
 * suppose that A is zero-filled
 */
void fillSpiral( int sizeX, int sizeY,
                 int xs, int ys,
                 bool CW,
                 int dirNum,
                 bool increment,
                 int (*A)[MAX_SIZE] )
{
    // from north
    int
            CWDirsX[] =  {0, 1, 0, -1},
            CWDirsY[] =  {-1, 0, 1, 0},

            CCWDirsX[] = {0, -1, 0, 1},
            CCWDirsY[] = {-1, 0, 1, 0};

    int *DirX, *DirY;

    if (CW)
    {
        DirX = CWDirsX;
        DirY = CWDirsY;
    }
    else
    {
        DirX = CCWDirsX;
        DirY = CCWDirsY;
    }

    int
            current,
            valToFinish;

    if (increment)
    {
        current = 1;
        valToFinish = sizeX * sizeY + 1;
    }
    else
    {
        current = sizeX * sizeY;
        valToFinish = 0;
    }

    int
            x = xs,
            y = ys;
    int curDirNum = dirNum;
    while (current != valToFinish)
    {
        A[y][x] = current;

        if (increment)
            current++;
        else
            current--;

        if (y + DirY[curDirNum] < 0 ||
            y + DirY[curDirNum] >= sizeY ||
            x + DirX[curDirNum] < 0 ||
            x + DirX[curDirNum] >= sizeX)
        {
            curDirNum = (curDirNum + 1) % 4;

            x = x + DirX[curDirNum];
            y = y + DirY[curDirNum];

            continue;
        }
        else if (A[y + DirY[curDirNum]][x + DirX[curDirNum]] != 0)
            curDirNum = (curDirNum + 1) % 4;

        x = x + DirX[curDirNum];
        y = y + DirY[curDirNum];
    }
}

void print( int (*A)[MAX_SIZE], int sizex, int sizey )
{
    for (int y = 0; y < sizey; y++)
    {
        for (int x = 0; x < sizex; x++)
            cout << setw(2) << A[y][x] << ' ';
        cout << endl;
    }
}

void menu( void )
{
    int sizex, sizey;
    bool live = true;

    cout << "Введите размер массива: ";
    cin >> sizex >> sizey;

    if (sizex > MAX_SIZE || sizey > MAX_SIZE)
    {
        cout << "Слишком большой размер массива" << endl;
        return;
    }

    while (live)
    {
        int A[MAX_SIZE][MAX_SIZE] = { { 0 } };

        cout <<  "0 - выход\n" <<
                 "1 - раскручиваться изнутри по часовой стрелке\n" <<
                 "2 - раскручиваться изнутри против часовой стрелки\n" <<
                 "3 - закручиваться в центр с левого верхнего края по часовой стрелке\n" <<
                 "4 - закручиваться в центр с левого верхнего края против часовой стрелки\n" <<
                 "Введите соответствующую команду: ";
        int c;
        cin >> c;
        switch (c)
        {
        case 0:
            live = false;
            break;
        case 1:
            fillSpiral(sizex, sizey, sizex - 1, 0, false, 1, false, A);
            break;
        case 2:
            fillSpiral(sizex, sizey, sizex - 1, 0, true, 2, false, A);
            break;
        case 3:
            fillSpiral(sizex, sizey, 0, 0, true, 1, true, A);
            break;
        case 4:
            fillSpiral(sizex, sizey, 0, 0, false, 2, true, A);
            break;
        default:
            cout << "Неправильная команда" << endl;
        }

        //system("cls");
        system("clear");
        print(A, sizex, sizey);
    }
}

int main()
{
    menu();
    return 0;
}

-5*((x+exp(-2x)-exp(-4x))*5exp(2x)+
((-0.3cos(x)-0.1sin(x))exp(-x)+(0.4cos(x)-0.7sin(x))exp(-5x)+(-0.3cos(x)+0.9sin(x))*exp(-3x))*(8cos(x)+sin(x))*exp(3x)+
((-0.7cos(x)-0.4sin(x))*exp(-5x)+(0.9cos(x)+0.3sin(x))*exp(-3x))*(8sin(x)-cos(x))*exp(3x))+
11*()+
6exp(2x)+10-11exp(-2x)
