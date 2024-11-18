#include "debug/debug.hpp"

void wonderful()
{
	debug::log::debug << "Something happened";
}

void marvelous()
{
	debug::log::warning << "Something wrong happened";
}

int main() {
	debug::cout << "Hello world!";
	debug::log::error << "Oh damn";

	wonderful();
	marvelous();

	return 0;
}
