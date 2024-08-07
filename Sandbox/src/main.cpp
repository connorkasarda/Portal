#include <iostream>
#include <string>
#include <new>
#include "Portal.h"

int main(int argc, char** argv)
{
	std::cout << "Hello, Sandbox!" << std::endl;

	// Testing
	Portal::StackAllocator salloc(100);

	int* intptr = salloc.Allocate<int>();
	*intptr = 42;
	float* floatptr = salloc.Allocate<float>();
	*floatptr = 3.14f;
	salloc.Deallocate(floatptr);
	salloc.Deallocate(intptr);

	struct Player
	{
		std::string name{};
		int health{};
	};
	Player* playerptr = salloc.Allocate<Player>();
	new (playerptr) Player{"Seig", 100};
	// Need to manually destruct when working with objects instantiated using 
	// placement new syntax
	playerptr->~Player();
	salloc.Reset();

	Portal::BistackAllocator balloc(128);

	int* intptr1 = balloc.AllocateBack<int>();
	*intptr1 = 1;
	int* intptr2 = balloc.AllocateBack<int>();
	*intptr2 = 2;
	int* intptr3 = balloc.AllocateBack<int>();
	*intptr3 = 3;

	double* doubleptr1 = balloc.AllocateFront<double>();
	*doubleptr1 = 4.5;
	double* doubleptr2  =balloc.AllocateFront<double>();
	*doubleptr2 = 6.7;
	double* doubleptr3 = balloc.AllocateFront<double>();
	*doubleptr3 = 8.9;

	balloc.DeallocateFront(doubleptr3);
	balloc.DeallocateBack(intptr3);
	balloc.DeallocateBack(intptr2);
	balloc.DeallocateFront(doubleptr2);

	int* intptr4 = balloc.AllocateBack<int>();
	*intptr4 = 10;

	double* doubleptr4 = balloc.AllocateFront<double>();
	*doubleptr4 = 11.12;

	balloc.Reset();

	Portal::PoolAllocator palloc(sizeof(Player), 8);

	Player* playerptr1 = static_cast<Player*>(palloc.Allocate());
	Player* playerptr2 = static_cast<Player*>(palloc.Allocate());
	Player* playerptr3 = static_cast<Player*>(palloc.Allocate());

	new (playerptr1) Player{"Connor", 97};
	new (playerptr2) Player{"Alex", 95};
	new (playerptr3) Player{"Sophia", 96};

	palloc.Deallocate(playerptr2);
	playerptr2->~Player();

	Player* playerptr4 = static_cast<Player*>(palloc.Allocate());
	Player* playerptr5 = static_cast<Player*>(palloc.Allocate());

	new (playerptr4) Player{ "Will", 100 };
	new (playerptr5) Player{ "Maria", 98 };

	// Need to manually destruct when working with objects instantiated using 
	// placement new syntax
	playerptr1->~Player();
	playerptr3->~Player();
	playerptr4->~Player();
	playerptr5->~Player();

	palloc.Reset();

	// Startup (priority queue to maintain order?)

	// Run
	while (true)
	{
		// Execute only once for testing purposes
		break;
	}
	// Shutdown

	return 0;
}