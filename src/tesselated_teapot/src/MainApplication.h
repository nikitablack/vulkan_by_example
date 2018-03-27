#pragma once

#include "app/AppData.h"

#include <string>

class MainApplication
{
public:
	MainApplication(uint32_t windowWidth, uint32_t windowHeight, std::string const & appName);
	~MainApplication();
	
	void run();
	
private:
	void render();

private:
	app::AppData _appData{};
	uint32_t _numProjMatrixBuffersToUpdate{0};
	uint32_t _numViewMatrixBuffersToUpdate{0};
};