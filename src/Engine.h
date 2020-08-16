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
			bubbleSort,
			cocktailSort,
			gnomeSort
		};

		Engine() = delete;

		// Random number generation
		Engine(Coord windowSize,
			const char* windowTitle = "Sort visualization"
		);

		~Engine();

		void run();
		inline void setDrawMethod(DrawMethod drawMethod)	      { selectedDrawMethod = drawMethod;       }
		inline void setSortAlgorithm(SortAlgorithm sortAlgorithm) { selectedSortAlgorithm = sortAlgorithm; }

		void generateRandom(int maxNumber);		
		void loadFile(const char* pathToNumbersFile);

	private:

		const Coord windowSize;
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;

		SortAlgorithm selectedSortAlgorithm = SortAlgorithm::bubbleSort;
		DrawMethod selectedDrawMethod = DrawMethod::line;
		
		std::vector<int> numbers = { };
		int columnWidth = 0;
		int maxValue = 0;
		bool running = true;

		void initWindow(Coord windowSize, const char* windowTitle);
		void initRenderer();
		void calculateNumbers();
		
		void handleEvents();

		void draw();
		void drawSelection();
		void drawColumns();
		void drawPoints();

		void step();
		void stepBubbleSort();
		void stepInsertionSort();
		void stepSelectionSort();
		void stepCocktailSort();
		void stepGnomeSort();
	};
}

#endif // ENGINE_H