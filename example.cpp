#include "debug/debug.hpp"

void wonderful()
{
	debug::log::debug() << "Something happened";
}

void marvelous()
{
	debug::log::warning() << "Something wrong happened";
}

int main(void) {
	debug::cerr() << "Hello world!";
	debug::log::fatal() << "Oh damn";

	wonderful();
	marvelous();

	return 0;
}
