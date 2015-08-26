#include "Errors.h"

void fatalError(std::string errorString)
{
	std::cerr << errorString << std::endl;
	std::cout << "Press any key to exit...";
	getchar();
	SDL_Quit();
	exit(69);
}