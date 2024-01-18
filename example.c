#include <debug/debug.h>


void wonderful()
{
	printf_level(LOG_DEBUG, "Something happened");
}

void marvelous()
{
	printf_level(LOG_WARNING, "Something %s happened", "wrong");
}

int main() {
	printf_debug("Test ");
	printf_level(LOG_ERROR, "Test 2");

	wonderful();
	marvelous();

	return 0;
}