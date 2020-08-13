#include "Engine.h"
#undef main

int main()
{
	try
	{
		// If the max number is higher than the window width it draws nothing other than a black screen :^)
		SortVis::Engine Visualization({ 1024, 768 }, 1024);
		Visualization.Run();
	}
	catch (std::runtime_error& Error)
	{
		std::cerr << Error.what() << "\n";
	}
}