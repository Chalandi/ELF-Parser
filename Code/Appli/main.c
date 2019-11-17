///////////////////////////////////////////////////////////////////////////////
//  Copyright Amine Chalandi 2019 - 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include<common.h>
#include<param.h>
#include<io.h>
#include<Elf.h>


char* ElfFilePath = NULL;
char* S19FilePath = NULL;
char* CFilePath   = NULL;
char* SearchTxt   = NULL;

static char* Buffer = NULL;

extern Elf32_Ehdr* pElfHeader;
extern Elf32_Shdr* pSectionHeader;
extern char* pSectionName;

/*********************************************************
**
*********************************************************/
int main(sint32 argc,string argv[])
{
  if(Param_OptionParser(argc,argv))
  {
    Buffer = LoadInputFile(ElfFilePath);

    if(Buffer != NULL)
    {
      if(TRUE == Elf_ProcessElfHeader(Buffer, Param_GetHeaderOpFlag()))
      {
        if(Param_GetSecTabOpFlag()) 
        {
          Elf_SectionHeaderTable(Buffer);
        }

        if(Param_GetSymTabOpFlag())
        {
          Elf_SymbolTable(Buffer);
        }

        if(Param_GetCOpFlag())
        {
          Elf_ExtractBinaryToC(Buffer, CFilePath);
        }

        if(Param_GetS19OpFlag())
        {
          Elf_ExtractBinaryToS19(Buffer, S19FilePath);
        }

        if(Param_GetSearchOpFlag())
        {
          Elf_SearchInfo(Buffer, SearchTxt);
        }

        if(Param_GetSrcListOpFlag())
        {
          Elf_ListSrcFiles(Buffer);
        }
      }

    }
  }
  return 0;
}

