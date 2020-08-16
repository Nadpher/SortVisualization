#include "Engine.h"
#undef main

int main()
{
	try
	{
		// If the max number is higher than the window width it draws nothing other than a black screen :^)
		// Default sort algorithm is bubble
		// Default draw method is line
		SortVis::Engine visualization({ 1024, 768 });
		visualization.setDrawMethod(SortVis::Engine::DrawMethod::line);
		visualization.setSortAlgorithm(SortVis::Engine::SortAlgorithm::cocktailSort);
		visualization.generateRandom(1024);
		visualization.run();
	}
	catch (std::runtime_error& error)
	{
		std::cerr << error.what() << "\n";
	}
}