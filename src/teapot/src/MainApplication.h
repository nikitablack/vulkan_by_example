#pragma once

#include "app/AppData.h"

#include <string>

class MainApplication
{
public:
	MainApplication(uint32_t windowWidth, uint32_t windowHeight, std::string const & appName);
	MainApplication(MainApplication const &) = delete;
	MainApplication & operator=(MainApplication const &) = delete;
	~MainApplication();
	
	void run();
	
private:
	MainApplication();
	
	void render();

private:
	app::AppData m_appData{};
	char * m_matricesMemoryPtr{nullptr};
	uint32_t m_numProjMatrixBuffersToUpdate{0};
	uint32_t m_numViewMatrixBuffersToUpdate{0};
	uint32_t m_numModelMatrixBuffersToUpdate{0};
};