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

		enum class DrawMethod
		{
			line,
			point
		};

		enum class SortAlgorithm
		{
			selectionSort,
			insertionSort,
			bubbleSort
		};

		// Random number generation
		Engine() = delete;
		Engine(Coord windowSize, int maxNumber);
		Engine(Coord windowSize, int maxNumber, SortAlgorithm algorithm);
		Engine(Coord windowSize, int maxNumber, SortAlgorithm algorithm, DrawMethod method);
		Engine(Coord windowSize, int maxNumber, const char* windowTitle);
		Engine(Coord windowSize, int maxNumber, const char* windowTitle, SortAlgorithm algorithm);
		Engine(Coord windowSize, int maxNumber, const char* windowTitle, SortAlgorithm algorithm, DrawMethod method);

		// Load from file
		Engine(Coord windowSize, const char* pathToNumbersFile);
		Engine(Coord windowSize, const char* pathToNumbersFile, SortAlgorithm algorithm);
		Engine(Coord windowSize, const char* pathToNumbersFile, SortAlgorithm algorithm, DrawMethod method);
		Engine(Coord windowSize, const char* pathToNumbersFile, const char* windowTitle);
		Engine(Coord windowSize, const char* pathToNumbersFile, const char* windowTitle, SortAlgorithm algorithm);
		Engine(Coord windowSize, const char* pathToNumbersFile, const char* windowTitle, SortAlgorithm algorithm, DrawMethod method);

		~Engine();

		void run();		

	private:

		const Coord windowSize;
		const SortAlgorithm selectedSortAlgorithm = SortAlgorithm::bubbleSort;
		const DrawMethod selectedDrawMethod = DrawMethod::line;

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
		void drawSelection();

		void drawColumns();
		void drawPoints();

		void step();
		void stepBubbleSort();
		void stepInsertionSort();
		void stepSelectionSort();

		std::vector<int> generateRandom(int maxNumber);		
	};
}

#endif // ENGINE_H