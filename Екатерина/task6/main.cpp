#include <iostream>
#include <sstream>
#include <list>

using namespace std;

using queue = list<int>;

void push( queue &q, int el )
{
    q.push_back(el);
}

//void pop( queue &q, int &el )
//{
//    el = q.front();
//    q.pop_front();
//}

void scanQueue( queue &q )
{
    string str;
    getline(cin, str);

    istringstream iss(str);
    string el;

    while (iss >> el)
        push(q, stoi(el));

}

void printQueue( queue const &q )
{
    for (auto e : q)
        cout << e << ' ';
    cout << '\n';
}

int main()
{
    queue q1, q2;

    scanQueue(q1);
    scanQueue(q2);

    q1.merge(q2);
    q1.sort([]( int v1, int v2 )
    {
        return v1 > v2;
    });

    printQueue(q1);

    return 0;
}
