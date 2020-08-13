#include "Engine.h"
#undef main

int main()
{
	try
	{
		// if the amount is higher than the screen width it all goes to shit :^)
		SortVis::Engine Visualization({ 1024, 768 }, {1024, 1024});
		Visualization.Run();
	}
	catch (std::runtime_error& Error)
	{
		std::cerr << Error.what() << "\n";
	}
}