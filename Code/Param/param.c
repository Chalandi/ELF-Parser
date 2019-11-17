///////////////////////////////////////////////////////////////////////////////
//  Copyright Amine Chalandi 2019 - 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include<param.h>


#define START_PARAMETERS                     const ParamList ParamListAction[] = {
#define DEFINE_PARAM(param,action,info)      {param, action, info},
#define END_PARAMETERS                       };

static void Param_SearchOpSetFlag(int* argc,char** argv);
static void Param_HeaderOpSetFlag(int* argc,char** argv);
static void Param_S19OpSetFlag(int* argc,char** argv);
static void Param_COpSetFlag(int* argc,char** argv);
static void Param_SecTabOpSetFlag(int* argc,char** argv);
static void Param_SymTabOpSetFlag(int* argc,char** argv);
static void Param_DisplayHelpOpSetFlag(int* argc,char** argv);
static void Param_SrcListOpSetFlag(int* argc,char** argv);


/*******************************************************************************************************************
** COMMAND LINE OPTIONS DEFINITION : 
*******************************************************************************************************************/
START_PARAMETERS
  DEFINE_PARAM("-header" , Param_HeaderOpSetFlag     ,  "             : Display the ELF file header")
  DEFINE_PARAM("-sec"    , Param_SecTabOpSetFlag     ,  "             : Display the sections table")
  DEFINE_PARAM("-sym"    , Param_SymTabOpSetFlag     ,  "             : Display the symbols table")
  DEFINE_PARAM("-srclist", Param_SrcListOpSetFlag    ,  "             : List all used files in the program")
  DEFINE_PARAM("-search" , Param_SearchOpSetFlag     ,  "<Symbol>     : Search for the <symbol> information in the ELF file")
  DEFINE_PARAM("-s19"    , Param_S19OpSetFlag        ,  "<OutputFile> : Extract the binary in s19 format")
  DEFINE_PARAM("-c"      , Param_COpSetFlag          ,  "<OutputFile> : Extract the binary in C-Array format")
  DEFINE_PARAM("-h"      , Param_DisplayHelpOpSetFlag,  "             : Display the information")
END_PARAMETERS

/*******************************************************************************************************************
** Globals 
*******************************************************************************************************************/

boolean Flag_HeaderOpSetFlag       = FALSE;
boolean Flag_SearchOpSetFlag       = FALSE;
boolean Flag_S19OpSetFlag          = FALSE;
boolean Flag_COpSetFlag            = FALSE;
boolean Flag_SecTabOpSetFlag       = FALSE;
boolean Flag_SymTabOpSetFlag       = FALSE;
boolean Flag_DisplayHelpOpSetFlag  = FALSE;
boolean Flag_SrcListOpSetFlag      = FALSE;

boolean boGlobalParamError         = FALSE;

int TotalOptionsNbr = 0; 

extern char* ElfFilePath;
extern char* S19FilePath;
extern char* CFilePath  ;
extern char* SearchTxt;

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Param_OptionParser(int argc,char** argv)
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
  WORD saved_attributes;
  boolean boOptionNotFound = TRUE;

  TotalOptionsNbr = argc;

  /* Save current attributes */
  GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
  saved_attributes = consoleInfo.wAttributes;

    ElfFilePath = (char*)argv[1];

  for(unsigned int option=1; option < (unsigned int)(argc); option++)
  {

    boOptionNotFound = TRUE;

    for(unsigned int cpt=0; cpt < (sizeof(ParamListAction)/sizeof(ParamList)); cpt++)
    {
      if(0 == strcmp((char*)(ParamListAction[cpt].param), argv[option]) && ParamListAction[cpt].action != NULL)
      {
        boOptionNotFound = FALSE;
        ParamListAction[cpt].action(&option, argv);
      }
    }

    if((boOptionNotFound && option != 1) || boGlobalParamError)
    {
      SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED);
      printf("\n SYNTAX ERROR !!! \n");

      DbgPrint("TotalOptionsNbr            = %d \n",TotalOptionsNbr          );
      DbgPrint("boGlobalParamError         = %d \n",boGlobalParamError       );
      DbgPrint("boOptionNotFound           = %d \n",boOptionNotFound         );
      DbgPrint("Flag_S19OpSetFlag          = %d \n",Flag_S19OpSetFlag        );
      DbgPrint("Flag_COpSetFlag            = %d \n",Flag_COpSetFlag          );
      DbgPrint("Flag_SecTabOpSetFlag       = %d \n",Flag_SecTabOpSetFlag     );
      DbgPrint("Flag_SymTabOpSetFlag       = %d \n",Flag_SymTabOpSetFlag     );
      DbgPrint("Flag_DisplayHelpOpSetFlag  = %d \n",Flag_DisplayHelpOpSetFlag);

      for(uint32 i=0; i < (uint32)TotalOptionsNbr; i++)
      {
        DbgPrint("argv[%d] = %s\n",i, argv[i]);
      }

      /* Restore original attributes */
      SetConsoleTextAttribute(hConsole, saved_attributes);

      Param_DisplayHelp();
      return(FALSE);
    }

  }

  if(TRUE == Flag_DisplayHelpOpSetFlag || boOptionNotFound)
  {
    DbgPrint("boGlobalParamError         = %d \n",boGlobalParamError       );
    DbgPrint("boOptionNotFound           = %d \n",boOptionNotFound         );
    DbgPrint("Flag_S19OpSetFlag          = %d \n",Flag_S19OpSetFlag        );
    DbgPrint("Flag_COpSetFlag            = %d \n",Flag_COpSetFlag          );
    DbgPrint("Flag_SecTabOpSetFlag       = %d \n",Flag_SecTabOpSetFlag     );
    DbgPrint("Flag_SymTabOpSetFlag       = %d \n",Flag_SymTabOpSetFlag     );
    DbgPrint("Flag_DisplayHelpOpSetFlag  = %d \n",Flag_DisplayHelpOpSetFlag);

    Param_DisplayHelp();
    return(FALSE);
  }
  else
  {
    return(TRUE);
  }
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
void Param_DisplayHelp(void)
{
  //system("cls");
  printf("\n ***********************************************************");
  printf("\n   ELF PARSER TOOL V1.0.19 ( DEVELOPED BY CHALANDI AMINE )  ");
  printf("\n ***********************************************************");
  printf("\n\n Usage: ElfParser.exe <inElfFile> [option(s)]\n");
  printf("\n Options are:\n");
  for(unsigned int i=0; i < (sizeof(ParamListAction)/sizeof(ParamList)); i++)
  {
    printf("\n %-8s %s",ParamListAction[i].param, ParamListAction[i].info);
  }
  printf("\n");
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
static void Param_SrcListOpSetFlag(int* argc,char** argv)
{ 
  (void)argc;
  (void)argv;
  Flag_SrcListOpSetFlag = TRUE;
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
static void Param_HeaderOpSetFlag(int* argc,char** argv)
{ 
  (void)argc;
  (void)argv;
  Flag_HeaderOpSetFlag = TRUE;
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
static void Param_SearchOpSetFlag(int* argc,char** argv)
{ 
  if((uint32)*argc + 1 < (uint32)TotalOptionsNbr)
  {
    Flag_SearchOpSetFlag = TRUE;
    SearchTxt = (char*)argv[++*argc];
  }
  else
  {
    boGlobalParamError = TRUE;
  }
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
static void Param_S19OpSetFlag(int* argc,char** argv)
{ 
  if((uint32)*argc + 1 < (uint32)TotalOptionsNbr)
  {
    Flag_S19OpSetFlag = TRUE;
    S19FilePath = (char*)argv[++*argc];
  }
  else
  {
    boGlobalParamError = TRUE;
  }
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
static void Param_COpSetFlag(int* argc,char** argv)
{
  if((uint32)*argc + 1 < (uint32)TotalOptionsNbr)
  {
    Flag_COpSetFlag = TRUE;
    CFilePath = (char*)argv[++*argc];
  }
  else
  {
    boGlobalParamError = TRUE;
  }
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
static void Param_SecTabOpSetFlag(int* argc,char** argv)
{ 
  (void)argc;
  (void)argv;
  Flag_SecTabOpSetFlag = TRUE;
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
static void Param_SymTabOpSetFlag(int* argc,char** argv)
{
  (void)argc;
  (void)argv;
  Flag_SymTabOpSetFlag = TRUE; 
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
static void Param_DisplayHelpOpSetFlag(int* argc,char** argv)
{
  (void)argc;
  (void)argv;
  Flag_DisplayHelpOpSetFlag = TRUE; 
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Param_GetSrcListOpFlag(void)
{ 
  return(Flag_SrcListOpSetFlag); 
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Param_GetHeaderOpFlag(void)
{ 
  return(Flag_HeaderOpSetFlag); 
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Param_GetSearchOpFlag(void)
{ 
  return(Flag_SearchOpSetFlag); 
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Param_GetS19OpFlag(void)
{ 
  return(Flag_S19OpSetFlag); 
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Param_GetCOpFlag(void)
{
  return(Flag_COpSetFlag); 
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Param_GetSecTabOpFlag(void)
{ 
  return(Flag_SecTabOpSetFlag);
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Param_GetSymTabOpFlag(void)
{ 
  return(Flag_SymTabOpSetFlag); 
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Param_GetDisplayHelpOpFlag(void)
{ 
  return(Flag_DisplayHelpOpSetFlag); 
}
