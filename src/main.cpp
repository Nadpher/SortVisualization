#include "Engine.h"
#undef main

#include <iostream>

int main()
{
	try
	{
		SortVis::Engine Application({1024, 768});
		Application.Run();
	}
	catch (std::runtime_error& Error)
	{
		std::cerr << Error.what() << "\n";
	}
}