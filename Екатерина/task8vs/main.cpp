#include <windows.h>

#include <iostream>
#include <fstream>
#include <string>
#include <stack>

using namespace std;

HANDLE mtx;

struct pData {
  stack<string> *lines;
  int num;
  int *i_ptr;
};

void print( stack<string> *lines, int num, int &i ) {
    string cur;

    while (!lines->empty()) {
        if (num % 2 == i % 2) {
            WaitForSingleObject(mtx, 0);

            cur = lines->top();
            lines->pop();
            std::cout << cur << " thread " << num << ' ' << "size: " << lines->size() << "\n";
            i++;

            ReleaseMutex(mtx);
        }
    }
}


DWORD WINAPI ThreadProc( CONST LPVOID lpParam ) {

    pData *d = (pData *)lpParam;

    print(d->lines, d->num, *d->i_ptr);
    ExitThread(0);
}

int main( void ) {

    std::string fileName;
    std::cout << "Enter a file name: ";
    std::cin >> fileName;
    ifstream f(fileName);

    if (!f)
        cerr << "Bad file name\n";

    stack<string> lines;
    string cur;

    while (f >> cur)
        lines.push(cur);

    mtx = CreateMutex(NULL, FALSE, "mtx");

    int i = 0;
    pData d1, d2;
    d1.i_ptr = &i;
    d1.lines = &lines;
    d1.num = 1;

    d2.i_ptr = &i;
    d2.lines = &lines;
    d2.num = 2;

    HANDLE
        thread1 = CreateThread(NULL, 0, &ThreadProc, &d1, 0, NULL),
        thread2 = CreateThread(NULL, 0, &ThreadProc, &d2, 0, NULL);

    WaitForSingleObject(thread1, INFINITE);
    WaitForSingleObject(thread2, INFINITE);

    CloseHandle(mtx);
    CloseHandle(thread1);
    CloseHandle(thread2);

    return 0;
}