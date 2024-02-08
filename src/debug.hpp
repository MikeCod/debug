/**
 * \brief Debug functions
 * \details Set of functions for debugging
 * \author Dimitri Simon
 * \date 2024-01-17
 */

#ifndef __DEBUG_HPP__
#define __DEBUG_HPP__

#include <cstdio>
#include <cstdlib>
#include <iostream>

#include "term.h"

/**
 * @brief Print something in debug mode
 * @details No loglevel/context
 * @param format
 * @param ... Parameters for format
 */
#define printf_debug

/**
 * @brief Print within a context
 * @param level loglevel/context (1-5)
 * @param format
 * @param ... Parameters for format
 */
#define printf_level

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
// #undef printf_level

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

#endif // (defined(DEBUG) || defined(DEBUG_LEVEL))

#endif // __DEBUG_H__