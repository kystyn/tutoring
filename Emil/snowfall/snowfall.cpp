#include <Windows.h>

#include <set>
#include <vector>

#include <iostream>
#include <fstream>

#include <string>
#include <algorithm>
#include <chrono>

#include <cassert>

const int rndPeriod = 100, snowfallPeriod = 250;

/* @ - wall symbol
 * X - cursor symbol
 * * - snow symbol
 */
const char wallSymbol = '@';
const char vertWall = '|';
const char horWall = '-';
const char snowflakeSymbol = '*';
const char cursorSymbol = 'X';


struct Field
{
	std::set<std::pair<int, int>> snowflakes; // indices of snowflakes
	std::set<std::pair<int, int>> walls; // indices of walls
	std::pair<int, int> cursor = { 0, 0 }; // index of cursor
	int w = 0, h = 0;
};


Field loadFieldFromFile(std::string const &fileName)
{
	std::ifstream ifs(fileName);

	if (!ifs)
	{
		std::cerr << "Bad file name " << fileName << "\n";
		return {};
	}

	Field f;
	
	// w is a field size without a FIELD wall!
	/*
	 |------|
	 |      |
	 |      |
	 |------|
	 w = 6, h = 2
	 NOT w = 8, h = 4
	*/
	ifs >> f.w >> f.h;

	std::string s;
	ifs >> s; // skip upper field wall

	for (int y = 0; y < f.h; y++)
	{
		char str[1000];
		ifs.getline(str, vertWall);
		// skip | (0) and | (w + 2) 
		// so bounds are 0 and w + 2 but not 0 and w
		for (int x = 0; x < f.w + 1; x++)
		{
			if (str[x] == wallSymbol)
				f.walls.insert(std::make_pair(x - 1, y));
		}
	}

	return f;
}


std::vector<std::vector<char>> fillSymbolicRepresentation(Field const &f)
{
	std::vector<std::vector<char>> symField;

	symField.resize(f.h + 2); //2 for FIELD walls

	for (auto &row : symField)
		row.resize(f.w + 2, ' '); // the same reason for + 2

	for (auto &row : symField) // fill vert walls
	{
		// first symbol in row
		row.front() = vertWall;
		// last symbol in row
		row.back() = vertWall;
	}

	for (auto &sym : symField.front()) // first row is a wall
		sym = horWall;

	for (auto &sym : symField.back()) // and the last also
		sym = horWall;

	// insert walls
	for (auto &w : f.walls)
		symField[w.second + 1][w.first + 1] = wallSymbol;

	return symField;
}


void saveFieldToFile(Field const &f, std::string const &fileName)
{
	std::ofstream ofs(fileName);

	if (!ofs)
	{
		std::cerr << "Couldn't create " << fileName << " file\n";
		return;
	}

	auto fieldToSave = fillSymbolicRepresentation(f);

	// field in symbolic representation is filled, so write to file!

	ofs << f.w << ' ' << f.h << "\n";
	for (auto &row : fieldToSave)
	{
		for (auto &col : row)
			ofs << col;
		ofs << "\n";
	}
}


void render(Field const &f)
{
	system("cls");
	auto output = fillSymbolicRepresentation(f);

	// common function to fill sumfield subset 
	// with mentioned symbol
	auto fill = [&output](std::set<std::pair<int, int>> const &indices,
		char symToFill)
	{
		for (auto &s : indices)
			output[s.second + 1][s.first + 1] = symToFill;
	};

	// fill snowflakes
	fill(f.snowflakes, snowflakeSymbol);
	// fill walls
	fill(f.walls, wallSymbol);

	output[f.cursor.second + 1][f.cursor.first + 1] = cursorSymbol;

	for (auto &row : output)
	{
		for (auto &x : row)
			std::cout << x;
		std::cout << '\n';
	}
}


void fillField(Field &f, int w, int h, int wallCount)
{
	srand(time(nullptr));
	f.w = w;
	f.h = h;

	for (int i = 0; i < wallCount; i++)
	{
		int idx = rand() % (w * (h - 2));
		f.walls.insert(std::make_pair(idx / (h - 2),
			2 + idx - idx / (h - 2) * (h - 2)));
	}
}


void emitSnowflake(Field &f, int xpos)
{
	// check that ther is no showflake just here
	auto p = std::make_pair(xpos, 1);
	if (f.snowflakes.find(p) == f.snowflakes.end())
		f.snowflakes.insert(p);
}

void snowfall(Field &f)
{
	for (auto &s : f.snowflakes)
		// if lower point is wall then stop, else go down
		if (f.walls.find(std::make_pair(s.first, s.second + 1))
			== f.walls.end() && // no wall
			s.second < f.h - 1 && // no field bound
			f.snowflakes.find(std::make_pair(s.first, s.second + 1))
			== f.snowflakes.end()) // no snowflake lower
		{
			auto saved = s;
			f.snowflakes.erase(s);
			f.snowflakes.insert({ saved.first, saved.second + 1 });
		}
}


void runFromField( Field &f )
{
	render(f);
	bool live = true;

	int dirX = 0;
	auto timeRnd = std::chrono::steady_clock::now(),
		timeSnowfall = timeRnd;
	while (live)
	{
		// check time elapsed since previous render
		if (std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::steady_clock::now() - timeRnd).count() < rndPeriod)
			continue;

		timeRnd = std::chrono::steady_clock::now();
		// move cursor
		if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
			dirX = 1;
		else if (GetAsyncKeyState(VK_LEFT) & 0x8000)
			dirX = -1;
		else if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
			live = false;
		else if (GetAsyncKeyState(VK_RETURN) & 0x8000)
			emitSnowflake(f, f.cursor.first);

		// % f.w because should stay in field
		f.cursor.first = (f.cursor.first + dirX + f.w) % f.w;
		dirX = 0;

		// check time elapsed since previous snowfall iteration
		if (std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::steady_clock::now() - timeSnowfall).count() > snowfallPeriod)
		{
			timeSnowfall = std::chrono::steady_clock::now();
			snowfall(f);
		}
		render(f);
	}
}

void run()
{
	bool live = true;

	while (live)
	{
		std::cout << "0 - exit\n1 - input field size and walls count\n" <<
			"2 - input file name to load from\n"
			"To stop snowfall, press ESC\n";
		
		Field f;
		int w, h, wallCount;
		int n;
		std::string name;
		std::cin >> n;
		getchar();

		switch (n)
		{
		case 0:
			live = false;
			break;
		case 1:
			std::cout << "Input width and height: ";
			std::cin >> w >> h;
			std::cout << "Input wall count: ";
			std::cin >> wallCount;
			fillField(f, w, h, wallCount);
			runFromField(f);
			break;
		case 2:
			std::cout << "Input file name: ";
			std::cin >> name;
			f = loadFieldFromFile(name);
			runFromField(f);
			break;
		default:
			break;
		}
		std::cout << "If you wish to save field configuration, input '3'. Any digit otherwise: ";
		std::cin >> n;
		if (n == 3)
		{
			std::cout << "Input file name: ";
			std::cin >> name;
			saveFieldToFile(f, name);
		}
	}
}

int main(void)
{
	run();
	system("pause");
	return 0;
}
