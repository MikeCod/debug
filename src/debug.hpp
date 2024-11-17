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

// enum
// 		{
// 			NONE = 0,
// 			FATAL,
// 			ERROR,
// 			WARNING,
// 			INFO,
// 			DEBUG,
// 			TRACE
// 		};

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
	} cout(std::cout.rdbuf());
	
	namespace log
	{
		debug_cout level(int l)
		{
			static int level = l;
			return debug_cout(std::cout.rdbuf());
		}
		debug_cout fatal() { return level(LOG_FATAL); };
		debug_cout error() { return level(LOG_ERROR); };
		debug_cout warning() { return level(LOG_WARNING); };
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
	// debug_cout cout(int l) { return debug_cout(); }

	class debug_level
	{
	public:
		debug_level(std::streambuf *sbuf) {}
		template <typename T>
		debug_cout operator<<(T &&value)
		{
			return debug_cout();
		}
	} cout(std::cout.rdbuf());

	namespace log
	{
		debug_cout fatal() { return debug_cout(); };
		debug_cout error() { return debug_cout(); };
		debug_cout warning() { return debug_cout(); };
	}
#endif
}

#endif // __DEBUG_HPP__