/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dailylib.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mgallais <mgallais@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 09:59:32 by mgallais          #+#    #+#             */
/*   Updated: 2024/07/29 17:35:45 by mgallais         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DAILYLIB_HPP
# define DAILYLIB_HPP

/*###########################################################################*/
/*                                  Defines                                  */
/*###########################################################################*/
# ifndef DAILYLIB_DEF
#  define DAILYLIB_DEF 1
# endif

# if DAILYLIB_DEF == 1

// Custom defines
#  define WHITESPACES "\t\n\v\f\r "
#  define YES 1
#  define NO 0
#  define SUCCESS 0
#  define NOTHING 0
#  define ERROR -1

# endif
/*###########################################################################*/
/*                                 Includes                                  */
/*###########################################################################*/
# ifndef DAILYLIB_C_LIB
#  define DAILYLIB_C_LIB 1
# endif

# if DAILYLIB_C_LIB == 1

// Standard C libraries
#  include <unistd.h>     // For system calls like write() and sleep()
#  include <stdlib.h>     // For general utility functions like malloc() and atoi()
#  include <stdarg.h>     // For handling variable argument lists
#  include <stdint.h>     // For fixed-width integer types
#  include <stdbool.h>    // For boolean types
#  include <string.h>     // For string manipulation functions like strlen() and strcpy()
#  include <stdio.h>      // For standard input/output functions like printf() and scanf()
#  include <fcntl.h>      // For file control options like open() and close()
#  include <errno.h>      // For error handling and reporting
#  include <math.h>       // For mathematical functions like sqrt() and pow()
#  include <time.h>       // For date and time functions like time() and localtime()
#  include <limits.h>     // For limits of integer types like INT_MAX and INT_MIN
#  include <float.h>      // For limits of floating-point types like FLT_MAX and FLT_MIN
#  include <signal.h>     // For signal handling
#  include <sys/types.h>  // For data types

# endif


# ifndef DAILYLIB_CPP_LIB
#  define DAILYLIB_CPP_LIB 1
# endif

# if DAILYLIB_CPP_LIB == 1

// Standard C++ libraries
#  include <iostream>      // For standard input/output functions (std::cout, std::cin, std::cerr)
#  include <algorithm>     // For functions std::min, std::max, std::sort, std::find, std::count, std::replace, std::remove
#  include <cstring>       // For string manipulation functions (strlen, strcpy, strcmp, ...)
#  include <csignal>       // For functions signal, sigaction
#  include <cstdio>        // For standard input/output functions if needed (e.g. printf)
#  include <cstdlib>       // For type conversion functions if needed (e.g. atoi, atof)
#  include <ctime>         // For time functions (time, clock, difftime, mktime, ...)
#  include <sstream>       // For string stream functions (std::stringstream)

# endif
/*###########################################################################*/
/*                                Structures                                 */
/*###########################################################################*/
# ifndef DAILYLIB_TYPE_STRUCT
#  define DAILYLIB_TYPE_STRUCT 1
# endif


# if DAILYLIB_TYPE_STRUCT == 1
// Structure for 2 integers
typedef struct s_2int
{
	int		x;
	int		y;
}	t_2int;

// Structure for 3 integers
typedef struct s_3int
{
	int		x;
	int		y;
	int		z;
}	t_3int;

// Structure for 4 integers
typedef struct s_4int
{
	int		r;
	int		g;
	int		b;
	int		l;
}	t_4int;

// Structure for 2 floats
typedef struct s_2float
{
	float	x;
	float	y;
}	t_2float;

// Structure for 3 floats
typedef struct s_3float
{
	float	x;
	float	y;
	float	z;
}	t_3float;

// Structure for 4 floats
typedef struct s_4float
{
	float	x;
	float	y;
	float	z;
	float	w;
}	t_4float;

// Structure for 2 doubles
typedef struct s_2double
{
	double	x;
	double	y;
}	t_2double;

// Structure for 3 doubles
typedef struct s_3double
{
	double	x;
	double	y;
	double	z;
}	t_3double;

// Structure for 4 doubles
typedef struct s_4double
{
	double	x;
	double	y;
	double	z;
	double	w;
}	t_4double;

// Structure for 3 unsigned chars
// Can contain a color in RGB format
typedef struct s_RBG
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
}	t_RGB;

// Structure for 4 unsigned chars
// Can contain a color in RGBA format
typedef struct s_RBGA
{
	unsigned char	r;
	unsigned char	g;
	unsigned char	b;
	unsigned char	a;
}	t_RGBA;
# endif
#endif