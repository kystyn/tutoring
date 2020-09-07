#include <Windows.h>
#include <vector>
#include <deque>
#include <iostream>
#include <string>
#include <algorithm>
#include <chrono>

const int period = 100;

using Snake = std::deque<std::pair<int, int>>;

struct Field
{
  std::vector<std::pair<int, int>> appleCells; // indices in cells
  int w, h;
};

void fillField( Field &f, int w, int h, int applesCount )
{
  srand(time(nullptr));
  f.w = w;
  f.h = h;

  f.appleCells.resize(applesCount);

  for (auto &x : f.appleCells)
  {
    int idx = rand() % (w * h);
    x = std::make_pair(idx / h, idx - idx / h * h);
    /// TODO: check random different values
  }
  /// TODO: let snake not lay over apple

}

void initSnake( Snake &snake, int startSize, Field const &f )
{
  // from (x=y=0)
  if (startSize >= f.h && startSize >= f.w)
  {
    std::cout << "Too big start size\n";
    return;
  }

  int 
    dirX = f.w > f.h,
    dirY = 1 - dirX;

  int x = 0, y = 0;

  snake.resize(startSize);

  for (int i = 0; i < startSize; i++)
    snake[startSize - 1 - i] = std::make_pair(dirX * i, dirY * i);
}

bool isValidCell( int x, int y, Field const &f )
{
  return x < f.w && x >= 0 && y < f.h && y >= 0;
}

bool moveSnake( Snake &s, Field &f, int moveToX, int moveToY )
{
  if (f.appleCells.empty())
    return false;

  static auto t = std::chrono::high_resolution_clock::now();

  if (
	  std::chrono::duration_cast<std::chrono::milliseconds>(
	  std::chrono::high_resolution_clock::now() - t).count() <
	  period)
    return true;

  t = std::chrono::high_resolution_clock::now();

  auto head = s.front();

  if (
    // wall
    isValidCell(head.first + moveToX, head.second + moveToY, f) &&
    // head does not interrupt snake itself
    std::find(s.begin(), s.end(), 
      std::make_pair(head.first + moveToX, head.second + moveToY)) == s.end())
    s.push_front(std::make_pair(head.first + moveToX, head.second + moveToY));
  else
    return false;

  // not apple cell
  auto cell = std::find(f.appleCells.begin(), f.appleCells.end(), head);
  if (cell == f.appleCells.end())
    s.pop_back();
  // apple cell
  else
  {
    f.appleCells.erase(cell);
    /*
    auto tail = s.back();
    int dirToAddX, dirToAddY;
    bool found = false;

    for (dirToAddY = -1; !found && dirToAddY <= 1; dirToAddY++)
      for (dirToAddX = -1; !found && dirToAddX <= 1; dirToAddX++)
      {
        if (dirToAddX * dirToAddY != 0)
          continue;
        if (isValidCell(tail.first + dirToAddX, tail.second + dirToAddY, f) &&
            std::find(s.begin(), s.end(), std::make_pair(tail.first + dirToAddX, tail.second + dirToAddY)) == s.end())
          found = true;
      }

    if (found == false)
      return false;

    s.push_back(std::make_pair(tail.first + dirToAddX, tail.second + dirToAddY));*/
  }

  return true;
}

void render( Snake const &s, Field const &f )
{
  system("cls");
  std::vector<std::string> output;

  output.resize(f.h + 2);

  for (auto &s : output)
    s.resize(f.w + 2);

  for (auto &y : output)
    for (auto &x : y)
      x = ' ';

  // hor
  for (int i = 0; i < f.w + 2; i++)
  {
    output[0][i] = '-';
    output[f.h + 1][i] = '-';
  }

  // ver
  for (int i = 0; i < f.h + 2; i++)
  {
    output[i][0] = '|';
    output[i][f.w + 1] = '|';
  }

  for (auto x : f.appleCells)
    output[x.second + 1][x.first + 1] = '@';

  for (auto x : s)
    output[x.second + 1][x.first + 1] = 'X';

  for (auto str : output)
    std::cout << str << std::endl;
}

void run()
{
  Field f;
  fillField(f, 50, 20, 5);

  Snake s;
  initSnake(s, 5, f);

  bool live = true;

  int dirX = 1, dirY = 0;
  while (live)
  {
      if (GetAsyncKeyState(VK_UP) & 0x8000)
      {
        if (!dirX)
          continue;
        dirX = 0;
        dirY = -1;
      }
      else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
      {
        if (!dirX)
          continue;
        dirX = 0;
        dirY = 1;
      }
      else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
      {
        if (!dirY)
          continue;
        dirX = 1;
        dirY = 0;
      }
      else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
      {
        if (!dirY)
          continue;
        dirX = -1;
        dirY = 0;
      }

      live = moveSnake(s, f, dirX, dirY);
      render(s, f);
  }
  if (!f.appleCells.empty())
    std::cout << "Game over\n";
  else
    std::cout << "You won\n";
}

int main( void )
{
  run();
  system("pause");
  return 0;
}