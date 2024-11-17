#include "debug/debug.hpp"
#include <iostream>


void wonderful() {
	printf_level(LOG_DEBUG, "Something happened"); // Show in any context equal or below DEBUG (5)
}

void marvelous() {
	printf_level(LOG_WARNING, "Something %s happened", "wrong"); // Show in any context equal or below WARNING (3)
}

int main() {
	printf_debug("Hello world !"); // Show in any debug context
	printf_level(LOG_ERROR, "Oh damn"); // Show in any context equal or below ERROR (2)

	wonderful();
	marvelous();

	std::cout << "Done" << std::endl;

	return 0;
}
