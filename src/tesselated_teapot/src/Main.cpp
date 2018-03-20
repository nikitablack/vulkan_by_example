#include "MainApplication.h"

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

int main()
{
	try
	{
		MainApplication app{800, 600, "Tesselated Teapot"};
		
		app.run();
	}
	catch(runtime_error const & err)
	{
		cout << err.what() << endl;
	}
	
	return 0;
}