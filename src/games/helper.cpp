#include "helper.h"

#include <iostream>

void clean(unsigned int nLine)
{
	std::cout << "\033[" << nLine << "A"; // Move cursor up n lines

    for (int i = 0; i < nLine; ++i) {
        std::cout << "\033[K"; // Clear the current line
    }
}