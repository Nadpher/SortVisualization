#include "Engine.h"

#include <filesystem>
#include <fstream>
#include <random>
#include <utility>
#include <algorithm>
#include <numeric>
#include <string>

// --- CONSTRUCTORS --- (fml)

SortVis::Engine::Engine(Coord windowSize, int maxNumber)
	: windowSize(windowSize), numbers(generateRandom(maxNumber))
{
	calculateNumbers();

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error("Could not initialize SDL");
	}

	initWindow(windowSize, "Sort visualizer");
	initRenderer();	
}

SortVis::Engine::Engine(Coord windowSize, int maxNumber, SortAlgorithm algorithm)
	: windowSize(windowSize), numbers(generateRandom(maxNumber)), selectedSortAlgorithm(algorithm)
{
	calculateNumbers();

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error("Could not initialize SDL");
	}

	initWindow(windowSize, "Sort visualizer");
	initRenderer();
}

SortVis::Engine::Engine(Coord windowSize, int maxNumber, SortAlgorithm algorithm, DrawMethod method)
	: windowSize(windowSize), numbers(generateRandom(maxNumber)),
	selectedSortAlgorithm(algorithm), selectedDrawMethod(method)
{
	calculateNumbers();

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error("Could not initialize SDL");
	}

	initWindow(windowSize, "Sort visualizer");
	initRenderer();
}

SortVis::Engine::Engine(Coord windowSize, int maxNumber, const char* windowTitle)
	: windowSize(windowSize), numbers(generateRandom(maxNumber))
{
	calculateNumbers();

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error("Could not initialize SDL");
	}

	initWindow(windowSize, windowTitle);
	initRenderer();
}

SortVis::Engine::Engine(Coord windowSize, const char* pathToNumbersFile)
	: windowSize(windowSize)
{
	if (!std::filesystem::exists(pathToNumbersFile))
	{
		throw std::runtime_error("That file does not exist. Make sure the path is correct.");
	}
	else
	{
		loadFile(pathToNumbersFile);
	}
	calculateNumbers();

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error("Could not initialize SDL");
	}

	initWindow(windowSize, "Sort visualizer");
	initRenderer();
}

SortVis::Engine::Engine(Coord windowSize, const char* pathToNumbersFile, const char* windowTitle)
	: windowSize(windowSize)
{
	if (!std::filesystem::exists(pathToNumbersFile))
	{
		throw std::runtime_error("That file does not exist. Make sure the path is correct.");
	}
	else
	{
		loadFile(pathToNumbersFile);
	}
	calculateNumbers();

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error("Could not initialize SDL");
	}

	initWindow(windowSize, windowTitle);
	initRenderer();
}

// --- END OF CONSTRUCTORS ---

SortVis::Engine::~Engine()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

void SortVis::Engine::run()
{
	// Sets render draw color to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	while (running)
	{
		handleEvents();
		if (!std::is_sorted(numbers.begin(), numbers.end()))
		{
			stepSelectionSort();
		}
		draw();
	}
}

void SortVis::Engine::stepBubbleSort()
{	
	static int i = 0;
	static int size = numbers.size();
	for (int j = 0; j < size - i - 1; ++j)
	{
		if (numbers[j] > numbers[j + 1])
		{
			std::swap(numbers[j], numbers[j + 1]);
		}			
	}
	++i;
}

void SortVis::Engine::stepInsertionSort()
{
	static int i = 1;	
	for (int j = i; j > 0 && numbers[j - 1] > numbers[j]; --j)
	{
		std::swap(numbers[j - 1], numbers[j]);
	}
	++i;
}

void SortVis::Engine::stepSelectionSort()
{
	static int i = 0;
	std::swap(numbers[i], numbers[std::min_element(numbers.begin() + i, numbers.end()) - numbers.begin()]);
	++i;
}

void SortVis::Engine::draw()
{
	SDL_RenderClear(renderer);

	drawColumns();
	// drawPoints();

	SDL_RenderPresent(renderer);
}

void SortVis::Engine::drawColumns()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Rect column;
	for (int i = numbers.size(); i > 0; --i)
	{
		column.x = (i-1) * columnWidth;
		column.w = columnWidth;
		column.h = (numbers[i - 1] * windowSize.Y) / maxValue;
		column.y = windowSize.Y - column.h;
		SDL_RenderFillRect(renderer, &column);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void SortVis::Engine::drawPoints()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	// SDL_Point point;
	for (int i = numbers.size(); i > 0; --i)
	{
		// point.x = i - 1;
		// point.y = windowSize.Y - ((numbers[i - 1] * windowSize.Y) / maxValue);
		SDL_RenderDrawPoint(renderer, i - 1, windowSize.Y - ((numbers[i - 1] * windowSize.Y) / maxValue));
	}
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void SortVis::Engine::handleEvents()
{
	SDL_Event Event;

	while (SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
		case SDL_QUIT:
			running = false;
			break;

		default:
			break;
		}
	}
}

std::vector<int> SortVis::Engine::generateRandom(int maxNumber)
{
	std::mt19937 seed(std::random_device{}());
	std::vector<int> num(maxNumber);
	std::iota(num.begin(), num.end(), 0);
	std::shuffle(num.begin(), num.end(), seed);

	std::cout << "Generated random number sequence.\n";

	return num;
}

void SortVis::Engine::calculateNumbers()
{
	columnWidth = windowSize.X / numbers.size();
	maxValue = *std::max_element(numbers.begin(), numbers.end());
}

void SortVis::Engine::loadFile(const char* pathToNumbersFile)
{
	std::ifstream NumbersFile(pathToNumbersFile);
	if (NumbersFile.is_open())
	{
		std::string Number;
		while (std::getline(NumbersFile, Number))
		{
			numbers.push_back(std::stoi(Number));
		}
	}
	else
	{
		throw std::runtime_error("Couldn't open numbers file.");
	}

	if (numbers.empty())
	{
		throw std::runtime_error("Numbers file is empty.");
	}

	std::cout << "Loaded numbers file.\n";
}

void SortVis::Engine::initWindow(Coord windowSize, const char* windowTitle)
{
	window = SDL_CreateWindow(
		windowTitle,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		windowSize.X,
		windowSize.Y,
		SDL_WINDOW_SHOWN
	);

	if (window == nullptr)
	{
		throw std::runtime_error("Could not initialize SDL window");
	}
}

void SortVis::Engine::initRenderer()
{
	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
	);

	if (renderer == nullptr)
	{
		throw std::runtime_error("Could not initialize SDL renderer");
	}
}