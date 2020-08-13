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
		Engine(Coord pWindowSize, int pMaxNumber);
		Engine(Coord pWindowSize, const char* pPathToNumbersFile);
		Engine(Coord pWindowSize, const char* pPathToNumbersFile, const char* pWindowTitle);
		Engine(Coord pWindowSize, int pMaxNumber, const char* pWindowTitle);

		~Engine();

		void Run();

	private:

		const Coord m_WindowSize;
		SDL_Window* m_Window = nullptr;
		SDL_Renderer* m_Renderer = nullptr;
		
		std::vector<int> m_Numbers = { };
		int m_ColumnWidth = 0;
		int m_MaxValue = 0;
		bool m_Running = true;
		bool m_Sorted = false;

		void InitWindow(Coord pWindowSize, const char* pWindowTitle);
		void InitRenderer();
		void CalculateNumbers();		
		void LoadFile(const char* pPathToNumbersFile);
		
		void HandleEvents();
		void BubbleSort();
		void Draw();
		void DrawColumns();

		void GenerateRandom(int pMaxNumber);		
	};
}