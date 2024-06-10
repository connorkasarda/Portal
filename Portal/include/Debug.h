#ifndef DEBUG_H
#define DEBUG_H

// Prints error message when invariant broken
void error(int line, const char* file, const char* format, ...);

#ifdef DEBUG
#define ASSERT(test, msg, ...) \
	do { \
		if (!(test)) { \
			error(__LINE__, __FILE__, "Invariant failed: " msg, \
				#test, ##__VA_ARGS__); \
		} \
	} while (0)
#else
#define ASSERT(test, msg, ...) ((void)0)
#endif

#endif