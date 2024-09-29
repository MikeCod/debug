/**
 * \brief Debug functions
 * \details Set of functions for debugging
 * \author Dimitri Simon
 * \date 2024-01-17
 */

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>
#include <stdlib.h>

#include "term.h"

/**
 * @brief Print something in debug mode
 * @details No loglevel/context
 * @param format
 * @param ... Parameters for format
 */
#define printf_debug(str)

/**
 * @brief Print within a context
 * @param level loglevel/context (1-5)
 * @param format
 * @param ... Parameters for format
 */
#define printf_level(l, fmt, ...)

#define LOG_FATAL 1
#define LOG_ERROR 2
#define LOG_WARNING 3
#define LOG_INFO 4
#define LOG_DEBUG 5
#define LOG_TRACE 6

#define C(x) (x + '0')

#if (defined(DEBUG) || defined(DEBUG_LEVEL))
#ifndef DEBUG_SPACING_FILE
#define DEBUG_SPACING_FILE 12
#endif // DEBUG_SPACING_FILE
#ifndef DEBUG_SPACING_FUNCTION
#define DEBUG_SPACING_FUNCTION 15
#endif // DEBUG_SPACING_FUNCTION
#ifndef DEBUG_SPACING_LINE
#define DEBUG_SPACING_LINE 4
#endif // DEBUG_SPACING_LINE

#undef printf_debug
#undef printf_level

#define STRINGIFY(x) __STRINGIFY(x)
#define __STRINGIFY(x) #x
#define __line__ STRINGIFY(__LINE__)

#define FORMAT         \
	T_OUT(T_FG_YELLOW) \
	"%-" STRINGIFY(DEBUG_SPACING_FILE) "s" T_RESET " " T_OUT(T_BOLD T_FG_WHITE) "%-" STRINGIFY(DEBUG_SPACING_FUNCTION) "s" T_RESET ":" T_OUT(T_FG_CYAN) "%-" STRINGIFY(DEBUG_SPACING_LINE) "u" T_RESET " "

#define __printf_debug(format, ...) \
	printf(FORMAT format "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define printf_debug(format, ...) \
	printf("        " FORMAT format "\n", __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define __level(level)                                                  \
	{                                                                   \
		switch (level)                                                  \
		{                                                               \
		case LOG_DEBUG:                                                 \
			printf(T_OUT(T_BOLD T_FG_WHITE) " %-5s ", "DEBUG");         \
			break;                                                      \
		case LOG_INFO:                                                  \
			printf(T_OUT(T_BOLD T_FG_CYAN) " %-5s ", "INFO");           \
			break;                                                      \
		case LOG_WARNING:                                               \
			printf(T_OUT(T_BOLD T_FG_YELLOW) " %-5s ", "WARN");         \
			break;                                                      \
		case LOG_ERROR:                                                 \
			printf(T_OUT(T_BOLD T_FG_RED) " %-5s ", "ERROR");           \
			break;                                                      \
		case LOG_FATAL:                                                 \
			printf(T_OUT(T_REVERSE T_BOLD T_FG_RED) " %-5s ", "FATAL"); \
			break;                                                      \
		default:                                                        \
			printf(T_OUT(T_REVERSE T_FG_WHITE) " %-5s ", "TRACE");      \
			break;                                                      \
		}                                                               \
		printf(T_RESET " ");                                            \
	}

#ifdef DEBUG_LEVEL
#define printf_level(level, str, ...)           \
	{                                           \
		if (level <= DEBUG_LEVEL)               \
		{                                       \
			__level(level);                     \
			__printf_debug(str, ##__VA_ARGS__); \
		}                                       \
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

	tmp = result = (char*) malloc(strlen(orig) + (len_with - len_rep) * count + 1);

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

int make_regex(char *regex, const char *buf)
{
	if (buf[1] == ':')
		return sprintf(regex, "[" STRINGIFY(LOG_FATAL) "-%c]\\:%s", buf[0], &buf[2]);
	if (buf[0] >= C(LOG_FATAL) && buf[0] <= C(LOG_TRACE))
		return sprintf(regex, "[" STRINGIFY(LOG_FATAL) "-%c]\\:(.*)", buf[0]);
	return sprintf(regex, "[" STRINGIFY(LOG_FATAL) "-" STRINGIFY(LOG_TRACE) "]\\:%s", buf);
}

static char *debug_var = NULL;
static regex_t debug_regex;

#define printf_level(level, format, ...)                                            \
	{                                                                               \
		if (debug_var == NULL)                                                      \
		{                                                                           \
			debug_var = getenv("DEBUG");                                            \
			debug_var = str_replace(debug_var, "*:", STRINGIFY(LOG_TRACE) ":");     \
			debug_var = str_replace(debug_var, "*", "(.*)");                        \
			char strregex[8000] = "^(";                                             \
			char *p = strregex + 2;                                                 \
			char *c = strtok(debug_var, ";");                                       \
                                                                                    \
			p += make_regex(p, debug_var);                                          \
			for (c = strtok(NULL, ";"); c != NULL; c = strtok(NULL, ";"))           \
			{                                                                       \
				*p++ = '|';                                                         \
				p += make_regex(p, c);                                              \
			}                                                                       \
			*p++ = ')';                                                             \
			*p++ = '$';                                                             \
			assert(!regcomp(&debug_regex, strregex, REG_EXTENDED));                 \
		}                                                                           \
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
			__printf_debug(format, ##__VA_ARGS__);                                  \
			break;                                                                  \
		case REG_NOMATCH:                                                           \
			break;                                                                  \
		default:                                                                    \
			char msg[800];                                                          \
			regerror(ret, &debug_regex, msg, sizeof(msg));                          \
			fprintf(stderr, "Regex match failed: %s\n", msg);                       \
			exit(1);                                                                \
		}                                                                           \
	}

#endif // DEBUG_LEVEL

#endif // (defined(DEBUG) || defined(DEBUG_LEVEL))

#endif // __DEBUG_H__