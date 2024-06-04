#ifndef ROOT_H
#define ROOT_H

namespace Portal
{
	class Root
	{
	public:
		// Initializes Portal Engine systems in specific order
		void Startup();
		// Executes Portal Engine cycle until exit
		void Run();
		// Closes Portal Engine systems in reverese order
		void Shutdown();
	};
}

#endif