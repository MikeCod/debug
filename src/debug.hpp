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

#include <iostream>

#define LOG_UNDEFINED (-1)
#define LOG_NONE 0
#define LOG_FATAL 1
#define LOG_ERROR 2
#define LOG_WARNING 3
#define LOG_INFO 4
#define LOG_DEBUG 5
#define LOG_TRACE 6

#define DEBUG__
namespace debug
{
#ifdef DEBUG
	class debug_cout : public std::ostream
	{
	public:
		debug_cout(std::streambuf *sbuf)
			: std::ios(sbuf), std::ostream(sbuf)
		{
		}
		debug_cout(debug_cout &&other)
			: debug_cout(other.rdbuf()) {}
		~debug_cout() {}
	};
	debug_cout cout(int l)
	{
		static int level = l;
		return debug_cout(std::cout.rdbuf());
	}
	class debug_level
	{
		std::streambuf *sbuf;

	public:
		debug_level(std::streambuf *sbuf) : sbuf(sbuf) {}
		template <typename T>
		debug_cout operator<<(T &&value)
		{
			debug_cout rc(sbuf);
			rc << std::forward<T>(value);
			return rc;
		}
		debug_cout operator<<(std::ostream &(*manip)(std::ostream &))
		{
			debug_cout rc(sbuf);
			rc << manip;
			return rc;
		}
	};
	namespace level
	{
		debug_cout warning() { return cout(1); };
	}
#else
	class debug_cout
	{
	public:
		template <typename T>
		debug_cout &operator<<(T &&)
		{
			return *this;
		}

		debug_cout &operator<<(std::ios_base &(*)(std::ios_base &))
		{
			return *this;
		}

		debug_cout &operator<<(std::ios &(*)(std::ios &)) { return *this; }

		debug_cout &operator<<(std::ostream &(*)(std::ostream &))
		{
			return *this;
		}
	};
	debug_cout cout(int l) { return debug_cout(); }

	class debug_level
	{
	public:
		debug_level(std::streambuf *sbuf) {}
		template <typename T>
		debug_cout operator<<(T &&value)
		{
			return debug_cout();
		}
	};

	namespace level
	{
		debug_cout __warning() { return debug_cout(); };
	}

#endif
}

#endif // __DEBUG_HPP__