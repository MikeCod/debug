/**
 * \brief Debug functions
 * \details Set of functions for debugging
 * \author Dimitri Simon
 * \date 2024-01-17
 */

#include <stdio.h>
#include <stdlib.h>

#include "term.h"

/**
 * @brief Print something in debug mode
 * @details No loglevel/context
 * @param format
 * @param ... Parameters for format
 */
#define dbg_printf(fmt, ...)

/**
 * @brief Print within a context
 * @param level loglevel/context (1-5)
 * @param format
 * @param ... Parameters for format
 */
#define printf_level(l, fmt, ...)

#define printf_custom(file, func, line, level, format, ...)

#define printf_fatal(fmt, ...)
#define printf_error(fmt, ...)
#define printf_warning(fmt, ...)
#define printf_info(fmt, ...)
#define printf_debug(fmt, ...)
#define printf_trace(fmt, ...)

#define LOG_UNDEFINED (-1)
#define LOG_NONE 0
#define LOG_FATAL 1
#define LOG_ERROR 2
#define LOG_WARNING 3
#define LOG_INFO 4
#define LOG_DEBUG 5
#define LOG_TRACE 6

#define C(x) (x + '0')

#if (defined(DEBUG) || defined(DEBUG_LEVEL))
#ifndef DEBUG_SPACING_FILE
#define DEBUG_SPACING_FILE 10
#endif // DEBUG_SPACING_FILE
#ifndef DEBUG_SPACING_FUNCTION
#define DEBUG_SPACING_FUNCTION 10
#endif // DEBUG_SPACING_FUNCTION
#ifndef DEBUG_SPACING_LINE
#define DEBUG_SPACING_LINE 4
#endif // DEBUG_SPACING_LINE

#ifndef DEBUG_OUT
#define DEBUG_OUT 2
#endif

#undef dbg_printf
#undef printf_custom
#undef printf_level

#define __line__ STRINGIFY(__LINE__)

#define FORMAT         \
	T_OUT(T_FG_YELLOW) \
	"%-" STRINGIFY(DEBUG_SPACING_FILE) "s" T_RESET " " T_OUT(T_BOLD T_FG_WHITE) "%-" STRINGIFY(DEBUG_SPACING_FUNCTION) "s" T_OUT("0;" T_FG_CYAN) "%+" STRINGIFY(DEBUG_SPACING_LINE) "u" T_RESET " "

#define __dbg_printf(format, ...) \
	fprintf(DEBUG_OUT == 2 ? stderr : stdout, FORMAT format "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define dbg_printf(format, ...) \
	fprintf(DEBUG_OUT == 2 ? stderr : stdout, "        " FORMAT format "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define printf_custom(file, func, line, level, format, ...)                                             \
	{                                                                                                   \
		__level(level);                                                                                 \
		fprintf(DEBUG_OUT == 2 ? stderr : stdout, FORMAT format "\n", file, func, line, ##__VA_ARGS__); \
	}

#define __level(level)                                                                                     \
	{                                                                                                      \
		switch (level)                                                                                     \
		{                                                                                                  \
		case LOG_DEBUG:                                                                                    \
			fprintf(DEBUG_OUT == 2 ? stderr : stdout, T_OUT(T_BOLD T_FG_WHITE) " %-5s ", "DEBUG");         \
			break;                                                                                         \
		case LOG_INFO:                                                                                     \
			fprintf(DEBUG_OUT == 2 ? stderr : stdout, T_OUT(T_BOLD T_FG_CYAN) " %-5s ", "INFO");           \
			break;                                                                                         \
		case LOG_WARNING:                                                                                  \
			fprintf(DEBUG_OUT == 2 ? stderr : stdout, T_OUT(T_BOLD T_FG_YELLOW) " %-5s ", "WARN");         \
			break;                                                                                         \
		case LOG_ERROR:                                                                                    \
			fprintf(DEBUG_OUT == 2 ? stderr : stdout, T_OUT(T_BOLD T_FG_RED) " %-5s ", "ERROR");           \
			break;                                                                                         \
		case LOG_FATAL:                                                                                    \
			fprintf(DEBUG_OUT == 2 ? stderr : stdout, T_OUT(T_REVERSE T_BOLD T_FG_RED) " %-5s ", "FATAL"); \
			break;                                                                                         \
		default:                                                                                           \
			fprintf(DEBUG_OUT == 2 ? stderr : stdout, T_OUT(T_REVERSE T_FG_WHITE) " %-5s ", "TRACE");      \
			break;                                                                                         \
		}                                                                                                  \
		fprintf(DEBUG_OUT == 2 ? stderr : stdout, T_RESET " ");                                            \
	}

#ifdef DEBUG_LEVEL
#define printf_level(level, str, ...)         \
	{                                         \
		if (level <= DEBUG_LEVEL)             \
		{                                     \
			__level(level);                   \
			__dbg_printf(str, ##__VA_ARGS__); \
		}                                     \
	}
#else

#include <string.h>
#include <regex.h>
#include <assert.h>

char *str_replace(char *orig, const char *rep, const char *with)
{
	char *result;  // the return string
	char *ins;	   // the next insert point
	char *tmp;	   // varies
	int len_rep;   // length of rep (the string to remove)
	int len_with;  // length of with (the string to replace rep with)
	int len_front; // distance between rep and end of last rep
	int count;	   // number of replacements

	// sanity checks and initialization
	if (!orig || !rep)
		return NULL;
	len_rep = strlen(rep);
	if (len_rep == 0)
		return NULL; // empty rep causes infinite loop during count
	if (!with)
		with = "";
	len_with = strlen(with);

	// count the number of replacements needed
	ins = orig;
	for (count = 0; tmp = strstr(ins, rep); ++count)
	{
		ins = tmp + len_rep;
	}

	tmp = result = (char *)malloc(strlen(orig) + (len_with - len_rep) * count + 1);

	if (!result)
		return NULL;

	// first time through the loop, all the variable are set correctly
	// from here on,
	//    tmp points to the end of the result string
	//    ins points to the next occurrence of rep in orig
	//    orig points to the remainder of orig after "end of rep"
	while (count--)
	{
		ins = strstr(orig, rep);
		len_front = ins - orig;
		tmp = strncpy(tmp, orig, len_front) + len_front;
		tmp = strcpy(tmp, with) + len_with;
		orig += len_front + len_rep; // move to next "end of rep"
	}
	strcpy(tmp, orig);
	return result;
}

#define BUF_SZ 8000

int make_regex(char *regex, const char *buf)
{
	char search[BUF_SZ] = "(.*)";
	char level = C(LOG_TRACE);

#define GET_LEVEL(c)                                          \
	if (c == '*' || (c >= C(LOG_FATAL) && c <= C(LOG_TRACE))) \
		level = c == '*' ? C(LOG_TRACE) : c;

	if (buf[1] == ':')
	{
		snprintf(search, BUF_SZ, "%s", &buf[2]);
		GET_LEVEL(buf[0])
		else fprintf(stderr, "DEBUG: '%c' isn't recognised as a debugging level (max is " __STRINGIFY(LOG_TRACE) " = " STRINGIFY(LOG_TRACE) ")\n\n", buf[0]);
	}
	else
		GET_LEVEL(buf[0])
	else if (buf[0] >= (C(LOG_TRACE) + 1) && buf[0] <= '9')
		fprintf(stderr, "DEBUG: '%c' isn't recognised as a debugging level (max is " __STRINGIFY(LOG_TRACE) " = " STRINGIFY(LOG_TRACE) ")\n\n", buf[0]);
	else strncpy(search, &buf[2], BUF_SZ);

	return sprintf(regex, "[" STRINGIFY(LOG_FATAL) "-%c]\\:%s", level, search);
}

char *debug_var = NULL;
regex_t debug_regex;

#define printf_level(level, format, ...)                                                \
	{                                                                                   \
		debug_var = getenv("DEBUG");                                                    \
		if (debug_var != NULL && strlen(debug_var) != 0)                                \
		{                                                                               \
			if (strcmp(debug_var, "*") == 0)                                            \
				debug_var[0] = '6';                                                     \
			else                                                                        \
			{                                                                           \
				debug_var = str_replace(debug_var, "*:", STRINGIFY(LOG_TRACE) ":");     \
				debug_var = str_replace(debug_var, "*", "(.*)");                        \
			}                                                                           \
			char strregex[8000] = "^(";                                                 \
			char *p = strregex + 2;                                                     \
			char *c = strtok(debug_var, ";");                                           \
                                                                                        \
			p += make_regex(p, debug_var);                                              \
			for (c = strtok(NULL, ";"); c != NULL; c = strtok(NULL, ";"))               \
			{                                                                           \
				*p++ = '|';                                                             \
				p += make_regex(p, c);                                                  \
			}                                                                           \
			*p++ = ')';                                                                 \
			*p++ = '$';                                                                 \
			assert(!regcomp(&debug_regex, strregex, REG_EXTENDED));                     \
			int ret = regexec(&debug_regex, STRINGIFY(level) ":" __FILE__, 0, NULL, 0); \
			if (ret != 0)                                                               \
			{                                                                           \
				char testft[400];                                                       \
				sprintf(testft, STRINGIFY(level) ":%s", __func__);                      \
				ret = regexec(&debug_regex, testft, 0, NULL, 0);                        \
			}                                                                           \
			switch (ret)                                                                \
			{                                                                           \
			case 0:                                                                     \
				__level(level);                                                         \
				__dbg_printf(format, ##__VA_ARGS__);                                    \
				break;                                                                  \
			case REG_NOMATCH:                                                           \
				break;                                                                  \
			default:                                                                    \
			{                                                                           \
				char msg[800];                                                          \
				regerror(ret, &debug_regex, msg, sizeof(msg));                          \
				fprintf(stderr, "Regex match failed: %s\n", msg);                       \
				exit(1);                                                                \
			}                                                                           \
			}                                                                           \
		}                                                                               \
	}

#undef printf_fatal
#undef printf_error
#undef printf_warning
#undef printf_info
#undef printf_debug
#undef printf_trace

#define printf_fatal(fmt, ...) printf_level(LOG_FATAL, fmt, ##__VA_ARGS__)
#define printf_error(fmt, ...) printf_level(LOG_ERROR, fmt, ##__VA_ARGS__)
#define printf_warning(fmt, ...) printf_level(LOG_WARNING, fmt, ##__VA_ARGS__)
#define printf_info(fmt, ...) printf_level(LOG_INFO, fmt, ##__VA_ARGS__)
#define printf_debug(fmt, ...) printf_level(LOG_DEBUG, fmt, ##__VA_ARGS__)
#define printf_trace(fmt, ...) printf_level(LOG_TRACE, fmt, ##__VA_ARGS__)
#endif // DEBUG_LEVEL

#endif // (defined(DEBUG) || defined(DEBUG_LEVEL))
