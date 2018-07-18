#include "MainApplication.h"

#include <iostream>
#include <stdexcept>
#include <string>

int main()
{
	try
	{
		MainApplication app{800, 600, "Teapot"};
		
		app.run();
	}
	catch(std::runtime_error const & err)
	{
		std::cout << err.what() << std::endl;
	}
	catch(...)
	{
		std::cout << "unexpected error" << std::endl;
	}
	
	return 0;
}