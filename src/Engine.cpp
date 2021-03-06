#include "Engine.h"

#include <filesystem>
#include <fstream>
#include <random>
#include <utility>
#include <algorithm>
#include <numeric>
#include <string>

namespace SortVis
{

Engine::Engine(Coord windowSize, const char* windowTitle)
	: windowSize(windowSize)
{
	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error("Could not initialize SDL");
	}

	initWindow(windowSize, windowTitle);
	initRenderer();
}

Engine::~Engine()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
}

void Engine::run()
{
	// Sets render draw color to black
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	if (numbers.empty())
	{
		throw std::runtime_error("Vector is empty. Make sure to call either loadFile() or generateRandom()");
	}

	while (running)
	{
		handleEvents();
		if (!std::is_sorted(numbers.begin(), numbers.end()))
		{
			step();
		}
		draw();
	}
}

void Engine::step()
{
	switch (selectedSortAlgorithm)
	{
	case SortAlgorithm::bubbleSort:
		stepBubbleSort();
		break;

	case SortAlgorithm::insertionSort:
		stepInsertionSort();
		break;

	case SortAlgorithm::selectionSort:
		stepSelectionSort();
		break;

	case SortAlgorithm::cocktailSort:
		stepCocktailSort();
		break;

	case SortAlgorithm::gnomeSort:
		stepGnomeSort();
		break;

	default:
		break;
	}
}

void Engine::stepBubbleSort()
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

void Engine::stepInsertionSort()
{
	static int i = 1;
	for (int j = i; j > 0 && numbers[j - 1] > numbers[j]; --j)
	{
		std::swap(numbers[j - 1], numbers[j]);
	}
	++i;
}

void Engine::stepSelectionSort()
{
	static int i = 0;
	std::swap(numbers[i], numbers[std::min_element(numbers.begin() + i, numbers.end()) - numbers.begin()]);
	++i;
}

void Engine::stepCocktailSort()
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

	for (int j = size - i - 1; j > i; --j)
	{
		if (numbers[j] < numbers[j - 1])
		{
			std::swap(numbers[j], numbers[j - 1]);
		}
	}
}

void Engine::stepGnomeSort()
{
	static int i = 0;

	if (i == 0 || numbers[i] >= numbers[i - 1])
	{
		++i;
	}
	else
	{
		std::swap(numbers[i], numbers[i - 1]);
		--i;
	}
}

void Engine::draw()
{
	SDL_RenderClear(renderer);

	drawSelection();

	SDL_RenderPresent(renderer);
}

void Engine::drawSelection()
{
	switch (selectedDrawMethod)
	{
	case DrawMethod::line:
		drawColumns();
		break;

	case DrawMethod::point:
		drawPoints();
		break;

	default:
		break;
	}
}

void Engine::drawColumns()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

	SDL_Rect column;
	for (int i = numbers.size(); i > 0; --i)
	{
		column.x = (i - 1) * columnWidth;
		column.w = columnWidth;
		column.h = (numbers[i - 1] * windowSize.Y) / maxValue;
		column.y = windowSize.Y - column.h;
		SDL_RenderFillRect(renderer, &column);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
}

void Engine::drawPoints()
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

void Engine::handleEvents()
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

void Engine::generateRandom(int maxNumber)
{
	std::mt19937 seed(std::random_device{}());
	std::vector<int> num(maxNumber);
	std::iota(num.begin(), num.end(), 0);
	std::shuffle(num.begin(), num.end(), seed);

	numbers = num;
	calculateNumbers();

	std::cout << "Generated random number sequence.\n";
}

void Engine::calculateNumbers()
{
	columnWidth = windowSize.X / numbers.size();
	maxValue = *std::max_element(numbers.begin(), numbers.end());
}

void Engine::loadFile(const char* pathToNumbersFile)
{
	numbers.clear();

	std::ifstream numbersFile(pathToNumbersFile);

	std::string number;
	while (std::getline(numbersFile, number))
	{
		numbers.push_back(std::stoi(number));
	}

	if (!numbersFile.eof())
	{
		throw std::runtime_error("Error while reading numbers file.");
	}
	if (numbers.empty())
	{
		throw std::runtime_error("Numbers file is empty.");
	}

	calculateNumbers();

	std::cout << "Loaded numbers file.\n";
}

void Engine::initWindow(Coord windowSize, const char* windowTitle)
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

void Engine::initRenderer()
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

}