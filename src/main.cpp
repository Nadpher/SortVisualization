#include "Engine.h"
#undef main

int main()
{
	try
	{
		// If the max number is higher than the window width it draws nothing other than a black screen :^)
		SortVis::Engine visualization({ 1024, 768 });
		visualization.setDrawMethod(SortVis::Engine::DrawMethod::line);
		visualization.setSortAlgorithm(SortVis::Engine::SortAlgorithm::bubbleSort);

		visualization.run();
	}
	catch (std::runtime_error& error)
	{
		std::cerr << error.what() << "\n";
	}
}