#pragma once

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
		Engine(Coord pWindowSize, Coord pAmountAndMax);
		Engine(Coord pWindowSize, const char* pPathToNumbersFile);
		Engine(Coord pWindowSize, const char* pPathToNumbersFile, const char* pWindowTitle);
		Engine(Coord pWindowSize, Coord pAmountAndMax, const char* pWindowTitle);

		~Engine();

		void Run();

	private:

		SDL_Window* m_Window = nullptr;
		SDL_Renderer* m_Renderer = nullptr;
		const Coord m_WindowSize;
		std::vector<int> m_Numbers = { };
		int m_ColumnWidth = 0;
		int m_MaxValue = 0;
		bool m_Running = true;

		void InitWindow(Coord pWindowSize, const char* pWindowTitle);
		void InitRenderer();
		void CalculateNumbers();		
		void LoadFile(const char* pPathToNumbersFile);
		
		void HandleEvents();
		void BubbleSort();
		void Draw();
		void DrawColumns();

		void GenerateRandom(Coord pAmountAndMax);		
	};
}