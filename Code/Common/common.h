///////////////////////////////////////////////////////////////////////////////
//  Copyright Amine Chalandi 2019 - 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __COMMON_H__
#define __COMMON_H__

#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<windows.h>

#ifdef _DEBUG
#define DbgPrint printf
#else
#define DbgPrint
#endif


typedef void (*pfunc)(void);

typedef unsigned char*  string;
typedef unsigned char const *  const_string;

typedef unsigned char       uint8;
typedef signed char         sint8;
typedef unsigned short      uint16;
typedef signed short        sint16;
typedef unsigned long       uint32;
typedef signed long         sint32;
typedef unsigned long long  uint64;
typedef signed long long    sint64;

/*
typedef enum
{
  FALSE = 0,
  TRUE
}boolean;
*/

#define MAX_LINES_IN_FILE  1024 * 1024
#define MAX_LINE_LEN       1024

typedef char (*IoBuffer)[MAX_LINE_LEN];


#endif
