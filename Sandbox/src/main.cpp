#include <iostream>
#include "Root.h"

int main(int argc, char** argv)
{
	Portal::Root engine;

	engine.Startup();
	while (true)
	{
		engine.Run();
		// For demonstration purposes, break loop after one iteration
		break;
	}
	engine.Shutdown();

	return 0;
}