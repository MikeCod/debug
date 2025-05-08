// g++ example.cpp -std=c++20 -O1
#include "debug/debug.hpp"

void wonderful()
{
	debug::log::debug() << "Something happened";
}

void marvelous()
{
	debug::log::warning() << "Something wrong happened (errno " << 1 << ")";
}

int main(void) {
	debug::cerr() << "Hello world!";
	debug::log::fatal() << "Oh damn";

	wonderful();
	marvelous();

	return 0;
}
