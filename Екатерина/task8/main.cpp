#include <iostream>
#include <stack>
#include <fstream>
#include <thread>
#include <mutex>

using namespace std;

mutex m;

void print( stack<string> &lines, int num, int &i ) {
    string cur;

    while (!lines.empty()) {
        m.lock();

        if (num % 2 == i % 2) {
            cur = lines.top();
            lines.pop();
            std::cout << cur << " thread " << num << ' ' << "size: " << lines.size() << "\n";
            i++;
        }
        m.unlock();
    }
}

int main()
{
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

    if (thread::hardware_concurrency() < 2)
        cerr << "Non-mutithreading processor\n";

    int i = 0;
    thread t1([&lines, &i]() {print(lines, 0, i); });
    thread t2([&lines, &i]() {print(lines, 1, i); });

    t1.join();
    t2.join();

    return 0;
}
