/*******************************************************************************
 * @file		common.h
 * @brief		
 * @date		Sa Dec 2024
 * @author		Dimitri Simon
 * 
 * PROJECT:		DEBUG
 * 
 * MODIFIED:	Sat Dec 07 2024
 * BY:			Dimitri Simon
 * 
 * Copyright (c) 2024 Dimitri Simon
 * 
 *******************************************************************************/

#define LOG_UNDEFINED (-1)
#define LOG_NONE 0
#define LOG_FATAL 1
#define LOG_ERROR 2
#define LOG_WARNING 3
#define LOG_INFO 4
#define LOG_DEBUG 5
#define LOG_TRACE 6

#define LF "\n            "

#pragma GCC diagnostic warning "-Wall"
#pragma GCC diagnostic warning "-Wextra"
#pragma GCC diagnostic warning "-Wformat"
#pragma GCC diagnostic warning "-Wformat-security"


#ifndef DEBUG_SPACING_FILE
#define DEBUG_SPACING_FILE 10
#endif // DEBUG_SPACING_FILE
#ifndef DEBUG_SPACING_FUNCTION
#define DEBUG_SPACING_FUNCTION 10
#endif // DEBUG_SPACING_FUNCTION
#ifndef DEBUG_SPACING_LINE
#define DEBUG_SPACING_LINE 4
#endif // DEBUG_SPACING_LINE
