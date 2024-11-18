#define DEBUG_OUT 1
#include "debug/debug.h"

void wonderful(void)
{
	printf_level(LOG_DEBUG, "Something happened"); // Show in any context equal or below DEBUG (5)
}

void marvelous(void)
{
	printf_level(LOG_WARNING, "Something %s happened", "wrong"); // Show in any context equal or below WARNING (3)
}

int main(void)
{
	printf_debug("Hello %s !", "world");		// Show in any debug context
	printf_level(LOG_ERROR, "Oh damn"); // Show in any context equal or below ERROR (2)

	wonderful();
	marvelous();

	return 0;
}
