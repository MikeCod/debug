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
		bool need_newline = true;

	public:
		debug_cout(std::streambuf *sbuf)
			: std::ios(sbuf), std::ostream(sbuf)
		{
		}
		debug_cout(debug_cout &&other)
			: debug_cout(other.rdbuf())
		{
			other.need_newline = false;
		}
		~debug_cout()
		{
			this->need_newline && (*this) << std::endl;
		}
	};
	class debug_log
	{
		std::streambuf *sbuf;

	public:
		debug_log(std::streambuf *sbuf) : sbuf(sbuf)
		{
		}
		~debug_log()
		{
		}
		template <typename T>
		debug_cout operator<<(T &&value)
		{
			char *strlevel = std::getenv("DEBUG");
			if (strlevel == NULL)
				return debug_cout(NULL);
			if (std::atoi(strlevel) == 0)
				return debug_cout(NULL);

			debug_cout rc(sbuf);
			rc << std::forward<T>(value);
			return rc;
		}
		debug_cout operator<<(std::ostream &(*manip)(std::ostream &))
		{
			char *strlevel = std::getenv("DEBUG");
			if (strlevel == NULL)
				return debug_cout(NULL);
			if (std::atoi(strlevel) == 0)
				return debug_cout(NULL);

			debug_cout rc(sbuf);
			rc << manip;
			return rc;
		}
	} cout(std::cout.rdbuf()), cerr(std::cerr.rdbuf());

	namespace log
	{
		class debug_level
		{
			std::streambuf *sbuf;
			int level = LOG_UNDEFINED;

		public:
			debug_level(std::streambuf *sbuf, int l) : sbuf(sbuf)
			{
				this->level = l;
			}
			template <typename T>
			debug_cout operator<<(T &&value)
			{
				char *strlevel = std::getenv("DEBUG");
				if (strlevel == NULL)
					return debug_cout(NULL);
				if (this->level > std::atoi(strlevel))
					return debug_cout(NULL);

				debug_cout rc(sbuf);
				rc << std::forward<T>(value);
				return rc;
			}
			debug_cout operator<<(std::ostream &(*manip)(std::ostream &))
			{
				char *strlevel = std::getenv("DEBUG");
				if (strlevel == NULL)
					return debug_cout(NULL);
				if (this->level > std::atoi(strlevel))
					return debug_cout(NULL);

				debug_cout rc(sbuf);
				rc << manip;
				return rc;
			}
		} fatal(std::cerr.rdbuf(), LOG_FATAL),
			error(std::cerr.rdbuf(), LOG_ERROR),
			warning(std::cerr.rdbuf(), LOG_WARNING),
			info(std::cerr.rdbuf(), LOG_INFO),
			debug(std::cerr.rdbuf(), LOG_DEBUG),
			trace(std::cerr.rdbuf(), LOG_TRACE);
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

	class debug_log
	{
	public:
		debug_log(std::streambuf *sbuf) {}
		template <typename T>
		debug_cout operator<<(T &&value)
		{
			return debug_cout();
		}
	} cout(std::cout.rdbuf()), cerr(std::cerr.rdbuf());

	namespace log
	{
		class debug_level
		{
		public:
			debug_level(std::streambuf *sbuf, int l) {}
			template <typename T>
			debug_cout operator<<(T &&value)
			{
				return debug_cout();
			}
		} fatal(std::cerr.rdbuf(), LOG_FATAL),
			error(std::cerr.rdbuf(), LOG_ERROR),
			warning(std::cerr.rdbuf(), LOG_WARNING),
			info(std::cerr.rdbuf(), LOG_INFO),
			debug(std::cerr.rdbuf(), LOG_DEBUG),
			trace(std::cerr.rdbuf(), LOG_TRACE);
	}
#endif
}
