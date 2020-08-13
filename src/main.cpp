#include "Engine.h"
#undef main

int main()
{
	try
	{
		// if the amount is higher than the screen width it draws nothing other than a black screen :^)
		SortVis::Engine Visualization({ 1024, 768 }, 128);
		Visualization.Run();
	}
	catch (std::runtime_error& Error)
	{
		std::cerr << Error.what() << "\n";
	}
}