# Debug API

## Overview

API made to simplify debugging for C and C++.

### Requirements

C++20 or above.

## Installation

```sh
make install
# or
make i
```
You may uninstall using
```sh
make uninstall
# or
make u
```

## Loglevel

| Macro | Value | Purpose |
|-|:-:|-|
| **LOG_FATAL** | 1 | Cannot continue running |
| **LOG_ERROR** | 2 | May continue running, but it'd impact execution |
| **LOG_WARNING** | 3 | Unexpected event. The program can continue running, but the issue shall be solved |
| **LOG_INFO** | 4 | Notable event |
| **LOG_DEBUG** | 5 | Information about execution |
| **LOG_TRACE** | 6 | Show buffer content |


## Usage

__[C Example :](example.c)__
```c
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
```

__[C++ Example :](example.cpp)__
```cpp
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
```

You can build using :
```sh
# C
$ gcc example.c -DDEBUG # Enable debug
$ gcc example.c # No debug (optimized)

# C++
$ g++ example.cpp -DDEBUG # Enable debug
$ g++ example.cpp -O1 # No debug (optimized)
```

After what, if you enabled debugging, you shall be able to debug using `DEBUG` environment variable.

```sh
$ DEBUG=6 ./a.out
        example.c  main        15 Hello world !
 FATAL  example.c  main        16 Oh damn
 DEBUG  example.c  wonderful    5 Something happened
 WARN   example.c  marvelous   10 Something wrong happened
```

### Note C++

The flag `-O1` is **required to remove the calls** of these debugging functions.
Little residues of debugging functions remains in the executable, but everything else (strings) doesn't.

The residues of these debugging functions is a `ret` for each one:
```assembly
; Assembly

debug_cout_source_location:
	ret
debug_cerr_source_location:
	ret
...
```

At the moment, only debug level is supported in C++. From 0 to 6.

## How to use `DEBUG`

You may specify debug level using associated number (1-6, or `*`, which actually represents 6) and have debug output with a level higher.
```sh
$ DEBUG=4 ./a.out # Show everything higher or equal to LOG_INFO/4
$ DEBUG=* ./a.out # Show everything
```

You may filter using `:`, to specify a file or function.
```sh
$ DEBUG="*:main*" ./a.out # Show everything, related to files or functions starting by "main" in their name 
$ DEBUG="3:*main" ./a.out # Show only warning and higher, related to files or functions ending by "main" in their name 
$ DEBUG="*:*m*i*" ./a.out # Show everything, related to files or functions having "m" in their name preceded by "i"
$ DEBUG="2:*m*i*" ./a.out # Show only error and higher, related to files or functions having "m" in their name preceded by "i"
```
*If `*` is not followed by `;` nor the NULL byte (end of string), `*` is understood as a wildcard for the name*
```sh
# These 2 are equals
$ DEBUG="*:*main" ./a.out
$ DEBUG="*main" ./a.out
```

Filters may be chained using `;` (acting as OR inclusive) :
```sh
# Show all errors and higher, and everything when the name CONTAINS "test"
$ DEBUG="2;*wonder*" ./a.out

# Show all fatal errors
#  And everything when the name CONTAINS "wonder"
#  And all INFO events and higher when the name IS "marvelous"
$ DEBUG="1;*wonder*;4:marvelous" ./a.out
```

### No context

To print a string no matter the debug loglevel:
```cpp
printf_debug("Hello world !");
```

## Spacing

By default, debug output have spacing.
You may disable those with the macro `DEBUG_SPACING_FILE`, `DEBUG_SPACING_FUNCTION` and `DEBUG_SPACING_LINE`.
Set these to 0 if you want to spacing.
You may like using environment variables, using [`.env`](.env)
```sh
gcc example.c -DDEBUG -Wall $(sed -E 's/^(.+)/\-D\1/g' .env)

# or
make
```

## Contributing

Pull requests are welcome. For major changes, please open an issue first
to discuss what you would like to change.

Please make sure to update tests as appropriate.

## License

[MIT](LICENSE)