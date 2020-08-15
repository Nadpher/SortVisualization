#ifndef ENGINE_H
#define ENGINE_H

#include "Coord.h"
#include <SDL.h>
#include <vector>
#include <iostream>

namespace SortVis
{
	class Engine
	{
	public:

		Engine() = delete;
		Engine(Coord windowSize, int maxNumber);
		Engine(Coord windowSize, const char* pathToNumbersFile);
		Engine(Coord windowSize, const char* pathToNumbersFile, const char* windowTitle);
		Engine(Coord windowSize, int maxNumber, const char* windowTitle);

		~Engine();

		void run();

	private:

		const Coord windowSize;
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		
		std::vector<int> numbers = { };
		int columnWidth = 0;
		int maxValue = 0;
		bool running = true;

		void initWindow(Coord windowSize, const char* windowTitle);
		void initRenderer();
		void calculateNumbers();		
		void loadFile(const char* pathToNumbersFile);
		
		void handleEvents();
		void draw();
		void drawColumns();

		void stepBubbleSort();
		void stepInsertionSort();
		void stepSelectionSort();

		std::vector<int> generateRandom(int maxNumber);		
	};
}

#endif // ENGINE_H