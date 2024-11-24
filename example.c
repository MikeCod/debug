// gcc example.c
#include "debug/debug.h"

void wonderful(void)
{
	printf_debug("Something happened"); // Show in any context equal or below DEBUG (5)
}

void marvelous(void)
{
	printf_warning("Something %s happened", "wrong"); // Show in any context equal or below WARNING (3)
}

int main(void)
{
	dbg_printf("Hello %s !", "world");		// Show in any debug context
	printf_fatal("Oh damn"); // Show in any context equal or below ERROR (2)

	wonderful();
	marvelous();

	return 0;
}
