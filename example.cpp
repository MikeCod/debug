#include "debug/debug.hpp"

int main() {
	debug::cout << "Hello " << 0 << std::endl;
	debug::log::fatal() << "Hello " << 1 << std::endl;
	debug::log::info() << "Hello " << 4 << std::endl;

	return 0;
}
