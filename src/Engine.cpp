#include "Engine.h"

#include <filesystem>
#include <fstream>
#include <random>
#include <utility>
#include <algorithm>
#include <string>

SortVis::Engine::Engine(Coord pWindowSize, int pMaxNumber)
	: m_WindowSize(pWindowSize)
{
	GenerateRandom(pMaxNumber);

	CalculateNumbers();

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error("Could not initialize SDL");
	}

	InitWindow(pWindowSize, "Sort visualizer");
	InitRenderer();	
}

SortVis::Engine::Engine(Coord pWindowSize, int pMaxNumber, const char* pWindowTitle)
	: m_WindowSize(pWindowSize)
{
	GenerateRandom(pMaxNumber);

	CalculateNumbers();

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error("Could not initialize SDL");
	}

	InitWindow(pWindowSize, "Sort visualizer");
	InitRenderer();
}

SortVis::Engine::Engine(Coord pWindowSize, const char* pPathToNumbersFile)
	: m_WindowSize(pWindowSize)
{
	if (!std::filesystem::exists(pPathToNumbersFile))
	{
		throw std::runtime_error("That file does not exist. Make sure the path is correct.");
	}
	else
	{
		LoadFile(pPathToNumbersFile);
	}
	CalculateNumbers();

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error("Could not initialize SDL");
	}

	InitWindow(pWindowSize, "Sort visualizer");
	InitRenderer();
}

SortVis::Engine::Engine(Coord pWindowSize, const char* pPathToNumbersFile, const char* pWindowTitle)
	: m_WindowSize(pWindowSize)
{
	if (!std::filesystem::exists(pPathToNumbersFile))
	{
		throw std::runtime_error("That file does not exist. Make sure the path is correct.");
	}
	else
	{
		LoadFile(pPathToNumbersFile);
	}
	CalculateNumbers();

	if (SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
	{
		throw std::runtime_error("Could not initialize SDL");
	}

	InitWindow(pWindowSize, pWindowTitle);
	InitRenderer();
}

SortVis::Engine::~Engine()
{
	SDL_DestroyWindow(m_Window);
	SDL_DestroyRenderer(m_Renderer);

	SDL_Quit();
}

void SortVis::Engine::Run()
{
	// Sets render draw color to black
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
	Draw();

	while (m_Running)
	{
		HandleEvents();
		if (!m_Sorted)
		{
			BubbleSort();
		}
	}
}

void SortVis::Engine::BubbleSort()
{	
	for (int i = 0, Size = m_Numbers.size(); i < Size - 1; ++i)
	{
		for (int j = 0; j < Size - i - 1; ++j)
		{
			HandleEvents();
			if (!m_Running)
			{
				return;
			}

			if (m_Numbers[j] > m_Numbers[j + 1])
			{
				std::swap(m_Numbers[j], m_Numbers[j + 1]);
			}			
		}

		Draw();
	}
	
	m_Sorted = true;
}

void SortVis::Engine::Draw()
{
	SDL_RenderClear(m_Renderer);

	DrawColumns();

	SDL_RenderPresent(m_Renderer);
}

void SortVis::Engine::DrawColumns()
{
	SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);

	SDL_Rect Column;
	for (int i = 0, Size = m_Numbers.size(); i < Size; ++i)
	{
		Column.x = i * m_ColumnWidth;
		Column.w = m_ColumnWidth;
		Column.h = (m_Numbers[i] * m_WindowSize.Y) / m_MaxValue;
		Column.y = m_WindowSize.Y - Column.h;
		SDL_RenderFillRect(m_Renderer, &Column);
	}

	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
}

void SortVis::Engine::HandleEvents()
{
	SDL_Event Event;

	while (SDL_PollEvent(&Event))
	{
		switch (Event.type)
		{
		case SDL_QUIT:
			m_Running = false;
			break;
		}
	}
}

void SortVis::Engine::GenerateRandom(int pMaxNumber)
{
	std::mt19937 Seed(std::random_device{}());
	std::uniform_int_distribution<int> Distribution(1, pMaxNumber);

	for (int i = 0; i < pMaxNumber; ++i)
	{
		int Number = Distribution(Seed);
		while (std::count(m_Numbers.begin(), m_Numbers.end(), Number) != 0)
		{
			Number = Distribution(Seed);
		}
		m_Numbers.push_back(Number);
	}
	std::cout << "Generated random number sequence.\n";
}

void SortVis::Engine::CalculateNumbers()
{
	m_ColumnWidth = m_WindowSize.X / m_Numbers.size();
	m_MaxValue = *std::max_element(m_Numbers.begin(), m_Numbers.end());
}

void SortVis::Engine::LoadFile(const char* pPathToNumbersFile)
{
	std::ifstream NumbersFile(pPathToNumbersFile);
	if (NumbersFile.is_open())
	{
		std::string Number;
		while (std::getline(NumbersFile, Number))
		{
			m_Numbers.push_back(std::stoi(Number));
		}
	}
	else
	{
		throw std::runtime_error("Couldn't open numbers file.");
	}

	if (m_Numbers.empty())
	{
		throw std::runtime_error("Numbers file is empty.");
	}

	std::cout << "Loaded numbers file.\n";
}

void SortVis::Engine::InitWindow(Coord pWindowSize, const char* pWindowTitle)
{
	m_Window = SDL_CreateWindow(
		pWindowTitle,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		pWindowSize.X,
		pWindowSize.Y,
		SDL_WINDOW_SHOWN
	);

	if (m_Window == nullptr)
	{
		throw std::runtime_error("Could not initialize SDL window");
	}
}

void SortVis::Engine::InitRenderer()
{
	m_Renderer = SDL_CreateRenderer(
		m_Window,
		-1,
		SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED
	);

	if (m_Renderer == nullptr)
	{
		throw std::runtime_error("Could not initialize SDL renderer");
	}
}