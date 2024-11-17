/**
 * \brief Debug functions
 * \details Set of functions for debugging
 * \author Dimitri Simon
 * \date 2024-01-17
 */

#ifndef __DEBUG_HPP__
#define __DEBUG_HPP__

extern "C"
{
#include "debug.h"
}

// #undef printf_debug

#include <iostream>

#define LOG_UNDEFINED (-1)
#define LOG_NONE 0
#define LOG_FATAL 1
#define LOG_ERROR 2
#define LOG_WARNING 3
#define LOG_INFO 4
#define LOG_DEBUG 5
#define LOG_TRACE 6

namespace debug
{
#ifdef DEBUG
	class debug_cout
		: public std::ostream
	{
	public:
		debug_cout(std::streambuf *sbuf)
			: std::ios(sbuf), std::ostream(sbuf)
		{}
		~debug_cout() {}
	};

	debug_cout cout(int l) {
		static int level = l;
		return debug_cout(std::cout.rdbuf());
	}
#else
	class debug_cout
		: public std::ostream
	{
	public:
		debug_cout() {}
		~debug_cout() {}
	};

	debug_cout cout(int l) {
		return debug_cout();
	}
#endif
}

#endif // __DEBUG_HPP__