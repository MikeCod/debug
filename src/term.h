/*******************************************************************************
 * @file		term.h
 * @brief		
 * @date		Mo Nov 2024
 * @author		Dimitri Simon
 * 
 * PROJECT:		src
 * 
 * MODIFIED:	Sat Dec 07 2024
 * BY:			Dimitri Simon
 * 
 * Copyright (c) 2024 Dimitri Simon
 * 
 *******************************************************************************/

/* Legacy */

#define __STRINGIFY(x) #x
#define STRINGIFY(x) __STRINGIFY(x)

/**
 * \brief Update terminal display
 * \param x How to update (string)
 */
#define T_OUT(x) \
	"\033[" x "m"

#define T_RESET "\033[0m"

/** Style */
#define T_NORMAL "0;"
#define T_BOLD "1;"
#define T_DIM "2;"
#define T_ITALIC "3;"
#define T_UNDERLINE "4;"
#define T_BLINK "5;"
#define T_FAST_BLINK "6;"
#define T_REVERSE "7;"
#define T_HIDE "8;"
#define T_STRIKE "9;"
#define T_DOUBLE_UNDERLINE "21;"

/** Foreground */
#define T_FG_DEFAULT "39"
#define T_FG_BLACK "30"
#define T_FG_RED "31"
#define T_FG_GREEN "32"
#define T_FG_YELLOW "33"
#define T_FG_BLUE "34"
#define T_FG_MAGENTA "35"
#define T_FG_CYAN "36"
#define T_FG_WHITE "37"

#define T_FG_DARK_GRAY "90"
#define T_FG_LIGHT_RED "91"
#define T_FG_LIGHT_GREEN "92"
#define T_FG_LIGHT_YELLOW "93"
#define T_FG_LIGHT_BLUE "94"
#define T_FG_LIGHT_MAGENTA "95"
#define T_FG_LIGHT_CYAN "96"

/** Background */
#define T_BG_DEFAULT "49"
#define T_BG_BLACK "40"
#define T_BG_RED "41"
#define T_BG_GREEN "42"
#define T_BG_YELLOW "43"
#define T_BG_BLUE "44"
#define T_BG_MAGENTA "45"
#define T_BG_CYAN "46"
#define T_BG_WHITE "47"

#define T_BG_DARK_GRAY "100"
#define T_BG_LIGHT_RED "101"
#define T_BG_LIGHT_GREEN "102"
#define T_BG_LIGHT_YELLOW "103"
#define T_BG_LIGHT_BLUE "104"
#define T_BG_LIGHT_MAGENTA "105"
#define T_BG_LIGHT_CYAN "106"


/* Extended 24-bits colors (RGB) (only supported by some terminal) */

#define TE_FG "38"
#define TE_BG "48"

#define TE_RGB(t,r,g,b) \
	"\e[" #t ";2;" #r ";" #g ";" #b "m"

#define TE_RGB_FG(r,g,b) \
	"\e[" TE_FG ";2;" #r ";" #g ";" #b "m"

#define TE_RGB_BG(r,g,b) \
	"\e[" TE_BG ";2;" #r ";" #g ";" #b "m"

