/**
 * \brief Debug functions
 * \details Set of functions for debugging
 * \author Dimitri Simon
 * \date 2024-01-17
 */

extern "C"
{
#include "debug.h"
}

#include <iostream>

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
		debug_level(std::streambuf *sbuf) : sbuf(sbuf)
		{
		}
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
			char *strlevel = std::getenv("DEBUG");
			if(strlevel == NULL)
				return debug_cout(NULL);
			int level = std::atoi(strlevel);
			if(level < l)
				return debug_cout(NULL);
			
			return debug_cout(std::cout.rdbuf());
		}
		debug_cout fatal() { return level(LOG_FATAL); };
		debug_cout error() { return level(LOG_ERROR); };
		debug_cout warning() { return level(LOG_WARNING); };
		debug_cout info() { return level(LOG_INFO); };
		debug_cout debug() { return level(LOG_DEBUG); };
		debug_cout trace() { return level(LOG_TRACE); };
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
		debug_cout info() { return debug_cout(); };
		debug_cout debug() { return debug_cout(); };
		debug_cout trace() { return debug_cout(); };
	}
#endif
}
