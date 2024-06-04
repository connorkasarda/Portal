#include <iostream>
#include "Root.h"

int main(int argc, char** argv)
{
	Portal::Root engine;

	engine.Initialize();
	while (true)
	{
		engine.Update();
		engine.Render();
		// For demonstration purposes, break loop after one iteration
		break;
	}
	engine.Shutdown();

	return 0;
}