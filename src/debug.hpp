/**
 * \brief Debug functions
 * \details Set of functions for debugging
 * \author Dimitri Simon
 * \date 2024-01-17
 */

#include <iostream>
#include <iomanip>
#include <source_location>

#include "term.h"

#define LOG_UNDEFINED (-1)
#define LOG_NONE 0
#define LOG_FATAL 1
#define LOG_ERROR 2
#define LOG_WARNING 3
#define LOG_INFO 4
#define LOG_DEBUG 5
#define LOG_TRACE 6

#ifndef DEBUG_SPACING_FILE
#define DEBUG_SPACING_FILE 10
#endif // DEBUG_SPACING_FILE
#ifndef DEBUG_SPACING_FUNCTION
#define DEBUG_SPACING_FUNCTION 10
#endif // DEBUG_SPACING_FUNCTION
#ifndef DEBUG_SPACING_LINE
#define DEBUG_SPACING_LINE 4
#endif // DEBUG_SPACING_LINE

#define DEBUG_SPACING_FUNCTION_CPP_ADD 6

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

	std::string pad_right(std::string src, std::size_t total, char pchar = ' ')
	{
		std::string copy(src);
		if (total > copy.size())
		{
			copy.insert(copy.size(), total - copy.size(), pchar);
		}
		return copy;
	}
	class debug_log
	{
		std::streambuf *sbuf;
		std::source_location location;
		bool need_pad = true;

	public:
		debug_log(std::streambuf *sbuf, const std::source_location &location) : sbuf(sbuf)
		{
			this->location = location;
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
			if (this->need_pad)
			{
				rc << "        "
					T_OUT(T_FG_YELLOW) << pad_right(this->location.file_name(), DEBUG_SPACING_FILE) << T_RESET " "
					T_OUT(T_BOLD T_FG_WHITE) << pad_right(this->location.function_name(), DEBUG_SPACING_FUNCTION + DEBUG_SPACING_FUNCTION_CPP_ADD) << T_RESET " "
					T_OUT(T_FG_CYAN) << std::setw(DEBUG_SPACING_LINE) << std::to_string(this->location.line()) << T_RESET " ";
				
				this->need_pad = false;
			}
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
	};

	debug_log cout(const std::source_location &location = std::source_location::current())
	{
		return debug_log(std::cout.rdbuf(), location);
	}
	debug_log cerr(const std::source_location &location = std::source_location::current())
	{
		return debug_log(std::cout.rdbuf(), location);
	}
	namespace log
	{
		class debug_level
		{
			std::streambuf *sbuf;
			int level = LOG_UNDEFINED;
			bool need_level = true;
			std::source_location location;

		public:
			debug_level(std::streambuf *sbuf, const std::source_location &location, const int l) : sbuf(sbuf)
			{
				this->location = location;
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
				if (this->need_level)
				{
					switch (level)
					{
					case LOG_DEBUG:
						rc << T_OUT(T_BOLD T_FG_WHITE) " DEBUG ";
						break;
					case LOG_INFO:
						rc << T_OUT(T_BOLD T_FG_CYAN) " INFO  ";
						break;
					case LOG_WARNING:
						rc << T_OUT(T_BOLD T_FG_YELLOW) " WARN  ";
						break;
					case LOG_ERROR:
						rc << T_OUT(T_BOLD T_FG_RED) " ERROR ";
						break;
					case LOG_FATAL:
						rc << T_OUT(T_REVERSE T_BOLD T_FG_RED) " FATAL ";
						break;
					default:
						rc << T_OUT(T_REVERSE T_FG_WHITE) " TRACE ";
						break;
					}
					rc << T_RESET " "
					T_OUT(T_FG_YELLOW) << pad_right(this->location.file_name(), DEBUG_SPACING_FILE) << T_RESET " "
					T_OUT(T_BOLD T_FG_WHITE) << pad_right(this->location.function_name(), DEBUG_SPACING_FUNCTION + DEBUG_SPACING_FUNCTION_CPP_ADD) << T_RESET " "
					T_OUT(T_FG_CYAN) << std::setw(DEBUG_SPACING_LINE) << std::to_string(this->location.line()) << T_RESET " ";

					this->need_level = false;
				}
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
		};
		debug_level fatal(const std::source_location &location = std::source_location::current())
		{
			return debug_level(std::cout.rdbuf(), location, LOG_FATAL);
		}
		debug_level error(const std::source_location &location = std::source_location::current())
		{
			return debug_level(std::cout.rdbuf(), location, LOG_ERROR);
		}
		debug_level warning(const std::source_location &location = std::source_location::current())
		{
			return debug_level(std::cout.rdbuf(), location, LOG_WARNING);
		}
		debug_level info(const std::source_location &location = std::source_location::current())
		{
			return debug_level(std::cout.rdbuf(), location, LOG_INFO);
		}
		debug_level debug(const std::source_location &location = std::source_location::current())
		{
			return debug_level(std::cout.rdbuf(), location, LOG_DEBUG);
		}
		debug_level trace(const std::source_location &location = std::source_location::current())
		{
			return debug_level(std::cout.rdbuf(), location, LOG_TRACE);
		}
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
	
	debug_cout cout(const std::source_location &location = std::source_location::current())
	{
		return debug_cout();
	}

	debug_cout cerr(const std::source_location &location = std::source_location::current())
	{
		return debug_cout();
	}
	namespace log
	{
		debug_cout fatal(const std::source_location &location = std::source_location::current())
		{
			return debug_cout();
		}
		debug_cout error(const std::source_location &location = std::source_location::current())
		{
			return debug_cout();
		}
		debug_cout warning(const std::source_location &location = std::source_location::current())
		{
			return debug_cout();
		}
		debug_cout info(const std::source_location &location = std::source_location::current())
		{
			return debug_cout();
		}
		debug_cout debug(const std::source_location &location = std::source_location::current())
		{
			return debug_cout();
		}
		debug_cout trace(const std::source_location &location = std::source_location::current())
		{
			return debug_cout();
		}
		debug_cout cout(const std::source_location &location = std::source_location::current())
		{
			return debug_cout();
		}
	}

#endif
}
