///////////////////////////////////////////////////////////////////////////////
//  Copyright Amine Chalandi 2019 - 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __PARAM_H__
#define __PARAM_H__

#include<common.h>

typedef void (*ParamFunc)(int* argc,char** argv);

typedef struct
{
  const char* param;
  ParamFunc action;
  const char* info;
}ParamList;

extern const ParamList ParamListAction[];


boolean Param_OptionParser(int argc,char** argv);
void Param_DisplayHelp(void);

boolean Param_GetSearchOpFlag(void);
boolean Param_GetS19OpFlag(void);
boolean Param_GetCOpFlag(void);
boolean Param_GetSecTabOpFlag(void);
boolean Param_GetSymTabOpFlag(void);
boolean Param_GetDisplayHelpOpFlag(void);
boolean Param_GetHeaderOpFlag(void);
boolean Param_GetSrcListOpFlag(void);

#endif