///////////////////////////////////////////////////////////////////////////////
//  Copyright Amine Chalandi 2019 - 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include<Elf.h>

Elf32_Ehdr* pElfHeader     = NULL;
Elf32_Shdr* pSectionHeader = NULL;
Elf32_Sym*  pSymTable      = NULL;

char* pSectionName = NULL;


const sSymTabBind SymTabBind[] = {
                                    {STB_LOCAL  , "LOCAL" },
                                    {STB_GLOBAL , "GLOBAL"},
                                    {STB_WEAK   , "WEAK"  },
                                    {STB_NUM    , ""      },
                                    {STB_LOOS   , ""      },
                                    {STB_LOPROC , ""      },
                                    {STB_HIPROC , ""      }
                                 };

#define SYM_TABLE_BIND_SIZE   (uint32)(sizeof(SymTabBind)/sizeof(sSymTabBind))


const sSymTabType SymTabType[] = {
                                    {STT_NOTYPE    , "UNSPECIFIED"},
                                    {STT_OBJECT    , "OBJECT"     },
                                    {STT_FUNC      , "FUNCTION"   },
                                    {STT_SECTIONS  , "SECTION"    },
                                    {STT_FILE      , "FILE"       },
                                    {STT_COMMON    , "COMMON"     },
                                    {STT_TLS       , ""           },
                                    {STT_NUM       , ""           },
                                    {STT_LOOS      , ""           },
                                    {STT_HIOS      , ""           },
                                    {STT_LOPROC    , ""           },
                                    {STT_HIPROC    , ""           }
                                 };

#define SYM_TABLE_TYPE_SIZE   (uint32)(sizeof(SymTabType)/sizeof(sSymTabBind))

const sElfType ElfType[] = {
                              {TYP_UNKNOWN_ELF     , "Unknown ELF file type"},
                              {TYP_RELOCATABLE_ELF , "Relocatable ELF"      },
                              {TYP_EXECUTABLE_ELF  , "Executable ELF"       }
                           };

#define ELF_TYPE_TABLE_SIZE   (uint32)(sizeof(ElfType)/sizeof(sElfType))

const sMachine targetMachine[] = {
                                   {EM_NONE      ,"Invalid machine"               },
                                   {EM_SPARC     ,"Sun SPARC"                     },
                                   {EM_386       ,"Intel 80386"                   },
                                   {EM_68K       ,"Freescale 68000"               },
                                   {EM_486       ,"Intel 80486"                   },
                                   {EM_MIPS      ,"MIPS"                          },
                                   {EM_960       ,"Intel i960"                    },
                                   {EM_PPC       ,"Power Architecture"            },
                                   {EM_PPC64     ,"Power Architecture 64-bit mode"},
                                   {EM_V800      ,"Renesas V800 series"           },
                                   {EM_FR20      ,"Fujitsu FR"                    },
                                   {EM_MCORE     ,"Freescale MCore"               },
                                   {EM_ARM       ,"ARM"                           },
                                   {EM_SH        ,"Hitachi SH"                    },
                                   {EM_TRICORE   ,"Infineon TriCore"              },
                                   {EM_COLDFIRE  ,"Freescale ColdFire"            },
                                   {EM_STARCORE  ,"Freescale StarCore"            },
                                   {EM_FIREPATH  ,"Broadcom FirePath"             },
                                   {EM_M32R      ,"Renesas M32R"                  },
                                   {EM_BLACKFIN  ,"ADI Blackfin"                  }
                                 };

#define TARGET_MACHINE_TABLE_SIZE  ((sizeof(targetMachine))/(sizeof(sMachine)))

const sSectionType SectionTypeTable[] = {
                                           {SHT_NULL     , "NULL"    },
                                           {SHT_PROGBITS , "PROGBITS"},
                                           {SHT_SYMTAB   , "SYMTAB"  },
                                           {SHT_STRTAB   , "STRTAB"  },
                                           {SHT_RELA     , "RELA"    },
                                           {SHT_NOTE     , "NOTE"    },
                                           {SHT_NOBITS   , "NOBITS"  },
                                           {SHT_REL      , "REL"     }
                                        };

#define SECTION_TYPE_TABLE_SIZE  ((sizeof(SectionTypeTable))/(sizeof(sSectionType)))

const sSectionAttr SectionAttrTable[] = {
                                          {SHF_WRITE , 'W'},
                                          {SHF_ALLOC , 'A'},
                                          {SHF_EXECU  ,'X'}
                                        };

#define SECTION_ATTR_TABLE_SIZE  ((sizeof(SectionAttrTable))/(sizeof(sSectionAttr)))

static char* Elf_GetSymTabBindStr(Elf32_Byte bind);
static char* Elf_GetSymTabTypeStr(Elf32_Byte type);
static char* Elf_GetMachineNameStr(Elf32_Half machine);
static char* Elf_GetElfTypeStr(Elf32_Half type);
static char* Elf_GetSectionNameStr(Elf32_Word type);
static char* Elf_GetSectionAttrStr(Elf32_Word type);

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Elf_ProcessElfHeader(char* Buffer, boolean PrintInfo)
{

  if(Buffer != NULL)
  {
    pElfHeader = (Elf32_Ehdr*)Buffer;

    if( pElfHeader->e_ident[0] == 0x7F &&
        pElfHeader->e_ident[1] == 'E'  &&
        pElfHeader->e_ident[2] == 'L'  &&
        pElfHeader->e_ident[3] == 'F'  
      )
    {
      if(PrintInfo)
      {
        printf("\nELF File Header :\n\n");

        if(pElfHeader->e_ident[4] == ELFCLASS32)
        {
          printf("Class      = 32 bit\n");
        }
        else if(pElfHeader->e_ident[4] == ELFCLASS64)
        {
          printf("Class      = 64 bit\n");
        }
        else
        {
            printf("Class      = unknown class \n");
        }

        if(pElfHeader->e_ident[5] == ELFDATA2LSB)
        {
          printf("Endianness = little endian \n");
        }
        else if(pElfHeader->e_ident[5] == ELFDATA2MSB)
        {
          printf("Endianness = big endian \n");
        }
        else
        {
            printf("Endianness = unknown endian \n");
        }

        //display the ELF header
        printf("Type       = 0x%x (%s)\n", pElfHeader->e_type, Elf_GetElfTypeStr(pElfHeader->e_type));
        printf("Machine    = 0x%x (%s)\n", pElfHeader->e_machine, Elf_GetMachineNameStr(pElfHeader->e_machine));
        printf("Version    = 0x%x     \n", pElfHeader->e_version   );
        printf("Entry      = 0x%x     \n", pElfHeader->e_entry     );
        printf("Phoff      = 0x%x     \n", pElfHeader->e_phoff     );
        printf("Shoff      = 0x%x     \n", pElfHeader->e_shoff     );
        printf("Flags      = 0x%x     \n", pElfHeader->e_flags     );
        printf("Ehsize     = %d       \n", pElfHeader->e_ehsize    );
        printf("Phentsize  = %d       \n", pElfHeader->e_phentsize );
        printf("Phnum      = %d       \n", pElfHeader->e_phnum     );
        printf("Shentsize  = %d       \n", pElfHeader->e_shentsize );
        printf("Shnum      = %d       \n", pElfHeader->e_shnum     );
        printf("Shstrndx   = %d       \n", pElfHeader->e_shstrndx  );
      }

      return(TRUE);

    }
    else
    {
      printf("This is not a valid ELF file [KO]\n");
      return(FALSE);
    }
  }
  else
  {
    return(FALSE);
  }
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Elf_SectionHeaderTable(char* Buffer)
{
  // section header table pointer
  pSectionHeader = (Elf32_Shdr*)((uint32)Buffer + (uint32)(pElfHeader->e_shoff));
    
  // section string table pointer
  pSectionName = (char*)((uint32)Buffer + (uint32)((&pSectionHeader[pElfHeader->e_shstrndx])->sh_offset));

  printf("\nSECTIONS TABLE : \n");
  printf("\n%-10s%-20s%-20s%-20s%-22s%-22s%-22s\n","ID", "Section", "Type", "Flags", "Addr", "Offset", "Size");


  for(uint32 i = 0; i < pElfHeader->e_shnum ; i++)
  {
    printf("%-1s%-2d%-7s%-20s%-20s%-20s0x%-20x0x%-20x0x%-20x\n",
            "[",
            i, 
            "]",
            &pSectionName[(&pSectionHeader[i])->sh_name], 
            Elf_GetSectionNameStr((&pSectionHeader[i])->sh_type),
            Elf_GetSectionAttrStr((&pSectionHeader[i])->sh_flags),
            (&pSectionHeader[i])->sh_addr,
            (&pSectionHeader[i])->sh_offset,
            (&pSectionHeader[i])->sh_size
          );
  }
  return(TRUE);
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Elf_SymbolTable(char* Buffer)
{
  uint32 SymTabSize = 0;

  // section header table pointer
  pSectionHeader = (Elf32_Shdr*)((uint32)Buffer + (uint32)(pElfHeader->e_shoff));
    
  /* search for the symbol table index */
  for(uint32 i=0; i < pElfHeader->e_shnum; i++)
  {
    if((&pSectionHeader[i])->sh_type == SHT_SYMTAB)
    {
      pSymTable  = (Elf32_Sym*)((uint32)Buffer + (uint32)((&pSectionHeader[i])->sh_offset));
      SymTabSize = (uint32)(((&pSectionHeader[i])->sh_size) / sizeof(Elf32_Sym));
      break;
    }
  }

  /* search for the symbol table (string name table) */
  for(uint32 i=0; i < pElfHeader->e_shnum; i++)
  {
    if((&pSectionHeader[i])->sh_type == SHT_STRTAB && (i != pElfHeader->e_shstrndx))
    {
      // pointer to the table which contains the symbol names strings
      pSectionName = (char*)((uint32)Buffer + (uint32)((&pSectionHeader[i])->sh_offset));
      break;
    }
  }

  printf("\nSYMBOL TABLE : \n");
  printf("\n%-17s%-17s%-15s%-15s%-15s\n\n","Value", "Size", "Bind", "Type", "Name");

  /* Display the symbol table */
  for(uint32 i=0; i< SymTabSize ;i++)
  {
    if(STT_OBJECT == ELF32_ST_TYPE((&pSymTable[i])->st_info) || STT_FUNC == ELF32_ST_TYPE((&pSymTable[i])->st_info))
    {
      printf("0x%-15x0x%-15x%-15s%-15s%-15s\n",
              (&pSymTable[i])->st_value,
              (&pSymTable[i])->st_size,
              Elf_GetSymTabBindStr(ELF32_ST_BIND((&pSymTable[i])->st_info)),
              Elf_GetSymTabTypeStr(ELF32_ST_TYPE((&pSymTable[i])->st_info)),
              &pSectionName[(&pSymTable[i])->st_name]
            );
    }
  }

  return(TRUE);
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
static char* Elf_GetSymTabBindStr(Elf32_Byte bind)
{
  for(uint32 i=0; i < SYM_TABLE_BIND_SIZE; i++)
  {
    if(SymTabBind[i].code == bind)
    {
      return((char*)(SymTabBind[i].name));
    }
  }
  return(NULL);
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
static char* Elf_GetSymTabTypeStr(Elf32_Byte type)
{
  for(uint32 i=0; i < SYM_TABLE_TYPE_SIZE; i++)
  {
    if(SymTabType[i].code == type)
    {
      return((char*)(SymTabType[i].name));
    }
  }
  return(NULL);
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
static char* Elf_GetMachineNameStr(Elf32_Half machine)
{
  for(uint32 i = 0; i < TARGET_MACHINE_TABLE_SIZE; i++)
  {
    if(machine == targetMachine[i].machine)
    {
      return((char*)(targetMachine[i].name));
    }
  }
  return(NULL);
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
static char* Elf_GetElfTypeStr(Elf32_Half type)
{
  for(uint32 i = 0; i < ELF_TYPE_TABLE_SIZE; i++)
  {
    if(type == ElfType[i].code)
    {
      return((char*)(ElfType[i].name));
    }
  }
  return(NULL);
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
static char* Elf_GetSectionNameStr(Elf32_Word type)
{
  for(uint32 i = 0; i < SECTION_TYPE_TABLE_SIZE; i++)
  {
    if(type == SectionTypeTable[i].type)
    {
      return((char*)(SectionTypeTable[i].name));
    }
  }
  return(NULL);
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
static char* Elf_GetSectionAttrStr(Elf32_Word type)
{
  static char attr[4] = {' ',' ',' ','\0'};

  for(uint32 i=0; i < SECTION_ATTR_TABLE_SIZE; i++)
  {

    if((type & SectionAttrTable[i].code) == SectionAttrTable[i].code)
    {
      attr[i] = SectionAttrTable[i].name;
    }
    else
    {
      attr[i] = ' ';
    }
  }

  return(attr);
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Elf_ExtractBinaryToC(char* Buffer, char* path)
{
  uint8* OffAdd = NULL;
  uint32 size   = 0;

  if(Buffer == NULL || path == NULL)
  {
    return(FALSE);
  }

  // section header table pointer
  pSectionHeader = (Elf32_Shdr*)((uint32)Buffer + (uint32)(pElfHeader->e_shoff));

  // section string table pointer
  pSectionName = (char*)((uint32)Buffer + (uint32)((&pSectionHeader[pElfHeader->e_shstrndx])->sh_offset));

  // open file
  FILE* file = fopen(path, "wb");

  if(file != NULL)
  {
    for(uint32 i=0; i < pElfHeader->e_shnum  ;i++)
    {
      if(((((&pSectionHeader[i])->sh_flags) & (uint32)SHF_ALLOC) == (uint32)SHF_ALLOC) && 
           ((&pSectionHeader[i])->sh_type == SHT_PROGBITS) && 
           ((&pSectionHeader[i])->sh_size > 0)
        )
      {
        OffAdd = (uint8*)((uint32)Buffer + (uint32)((&pSectionHeader[i])->sh_offset));
        size   = (&pSectionHeader[i])->sh_size;

        /* print the section content */
        fprintf(file,"\n const unsigned char _%s[] = {\n\n",&pSectionName[(&pSectionHeader[i])->sh_name + 1 /* to avoid '.' in the section name*/]);
        for(uint32 cpt = 0; cpt < size; cpt++)
        {
          fprintf(file,"0x%02x", OffAdd[cpt]);

          if((cpt > 0) && ((cpt + 1) % 16 == 0))
          {
            fprintf(file,"\n");
          }
          else if (cpt + 1 != size)
          {
            fprintf(file,", ");
          }
          else
          {
            fprintf(file,"\n");
          }
        }
        fprintf(file,"};\n");
      }
    }
    fclose(file);
    return(TRUE);
  }
  else
  {
    return(FALSE);
  }
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Elf_ExtractBinaryToS19(char* Buffer, char* path)
{
  
  uint32 PhyAdd   = 0;
  uint8* OffAdd   = NULL;
  uint32 size     = 0;
  uint32 padding  = 0;
  uint8  checksum = 0;
  uint32 count    = 0;

  #define S19_HEADER_RECORD       "S0"
  #define S19_DATA_RECORD_32BIT   "S3"
  #define S19_COUNT_RECORD        "S503"
  #define S19_TERM_RECORD_32BIT   "S705"
  #define S19_PACKAGE_SIZE        28

  const char version[] = {"ELF_PARSER_BY_CHALANDI_AMINE_2019"};

  if(Buffer == NULL || path == NULL)
  {
    return(FALSE);
  }

  // section header table pointer
  pSectionHeader = (Elf32_Shdr*)((uint32)Buffer + (uint32)(pElfHeader->e_shoff));

  // section string table pointer
  pSectionName = (char*)((uint32)Buffer + (uint32)((&pSectionHeader[pElfHeader->e_shstrndx])->sh_offset));

  // open the s19 file in write mode
  FILE* file = fopen(path, "wb");

  if(file != NULL)
  {
    /* prepare the count for the S19 record header */
    count = 3 + (sizeof(version)/sizeof(char));

    /* calculate the checksum for the S19 record header */
    for(uint32 i=0; i < (sizeof(version)/sizeof(char)); i++)
    {
      checksum += (uint8)version[i];
    }

    checksum += (uint8)count;

    /* print the S19 header record */
    fprintf(file, "%s%02X%04X", S19_HEADER_RECORD, (uint8)count, (uint16)0);

    /* print the version in the record */
      for(uint32 i=0; i < (sizeof(version)/sizeof(char)); i++)
    {
      fprintf(file, "%02X", version[i]);
    }

    /* print the checksum */
    fprintf(file, "%02X\n", (uint8)~checksum);

    /* check all section in the ELF file */
    for(uint32 i=0; i < pElfHeader->e_shnum  ;i++)
    {
      if(((((&pSectionHeader[i])->sh_flags) & (uint32)SHF_ALLOC) == (uint32)SHF_ALLOC) && 
           ((&pSectionHeader[i])->sh_type == SHT_PROGBITS) && 
           ((&pSectionHeader[i])->sh_size > 0)
        )
      {
        PhyAdd = (&pSectionHeader[i])->sh_addr;
        OffAdd = (uint8*)((uint32)Buffer + (uint32)((&pSectionHeader[i])->sh_offset));
        size   = (&pSectionHeader[i])->sh_size;
        padding= (size % S19_PACKAGE_SIZE);

        /* print the S19 data records */
        for(uint32 cpt = 0; cpt < ((size / S19_PACKAGE_SIZE) * S19_PACKAGE_SIZE); cpt++)
        {
        
          if(cpt % S19_PACKAGE_SIZE == 0)
          {
            /* New Data Record */
            fprintf(file, "%s%02X%08X", S19_DATA_RECORD_32BIT, (uint8)(S19_PACKAGE_SIZE + 5), (uint32)(PhyAdd + cpt));

            /* Init the checksum */
            checksum = (uint8)(S19_PACKAGE_SIZE + 5)                  +
                       (uint8)(((PhyAdd + cpt) & 0xFF000000UL) >> 24) +
                       (uint8)(((PhyAdd + cpt) & 0x00FF0000UL) >> 16) +
                       (uint8)(((PhyAdd + cpt) & 0x0000FF00UL) >> 8 ) +
                       (uint8)(((PhyAdd + cpt) & 0x000000FFUL) >> 0 ) ;
            count ++;
          }

          /* Print one byte in the file */
          fprintf(file, "%02X", OffAdd[cpt]);
        
          /* Calculate the checksum */
          checksum += OffAdd[cpt];

          if(((cpt + 1) % S19_PACKAGE_SIZE == 0))
          {
            /* The end of one s19 record, insert the checksum */
            fprintf(file, "%02X\n", (uint8)~checksum); /* 1's complement */
          }

        }
        if(padding > 0) /* data size less than one S19 package */
        {
          /* prepare the S19 data record */
          fprintf(file, "%s%02X%08X", S19_DATA_RECORD_32BIT, (uint16)(padding + 5), (uint32)(PhyAdd + size - padding));
          checksum =  (uint8)(padding + 5)                                      +
                      (uint8)(((PhyAdd + size - padding) & 0xFF000000UL) >> 24) +
                      (uint8)(((PhyAdd + size - padding) & 0x00FF0000UL) >> 16) +
                      (uint8)(((PhyAdd + size - padding) & 0x0000FF00UL) >> 8 ) +
                      (uint8)(((PhyAdd + size - padding) & 0x000000FFUL) >> 0);
           count ++;

          for(uint32 cpt = 0; cpt < padding; cpt++)
          {
            fprintf(file, "%02X", OffAdd[size - padding + cpt]);
            checksum += OffAdd[size - padding + cpt];
          }
          fprintf(file, "%02X\n", (uint8)~checksum);
        }
      }
    }
    /* Prepare the S19 count record */
    checksum =  (uint8)(0x03)                           +
                (uint8)(((count) & 0x0000FF00UL) >> 8 ) +
                (uint8)(((count) & 0x000000FFUL) >> 0);
    fprintf(file, "%s%04X%02X\n", S19_COUNT_RECORD, (uint16)count, (uint8)~checksum);

    /* Prepare the S19 termination record */
    checksum =  (uint8)(0x05)                                         +
                (uint8)(((pElfHeader->e_entry) & 0xFF000000UL) >> 24) +
                (uint8)(((pElfHeader->e_entry) & 0x00FF0000UL) >> 16) +
                (uint8)(((pElfHeader->e_entry) & 0x0000FF00UL) >> 8 ) +
                (uint8)(((pElfHeader->e_entry) & 0x000000FFUL) >> 0);
    fprintf(file, "%s%08X%02X\n", S19_TERM_RECORD_32BIT, (uint32)pElfHeader->e_entry, (uint8)~checksum);
    fclose(file);
    return(TRUE);
  }
  else
  {
    return(FALSE);
  }
}

/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Elf_SearchInfo(char* Buffer, char* Symbol)
{
  uint32 SymTabSize = 0;

  if(Symbol == NULL)
  {
    return(FALSE);
  }

  // section header table pointer
  pSectionHeader = (Elf32_Shdr*)((uint32)Buffer + (uint32)(pElfHeader->e_shoff));
    
  /* search for the symbol table index */
  for(uint32 i=0; i < pElfHeader->e_shnum; i++)
  {
    if((&pSectionHeader[i])->sh_type == SHT_SYMTAB)
    {
      pSymTable  = (Elf32_Sym*)((uint32)Buffer + (uint32)((&pSectionHeader[i])->sh_offset));
      SymTabSize = (uint32)(((&pSectionHeader[i])->sh_size) / sizeof(Elf32_Sym));
      break;
    }
  }

  /* search for the symbol table (string name table) */
  for(uint32 i=0; i < pElfHeader->e_shnum; i++)
  {
    if((&pSectionHeader[i])->sh_type == SHT_STRTAB && (i != pElfHeader->e_shstrndx))
    {
      // pointer to the table which contains the symbol names strings
      pSectionName = (char*)((uint32)Buffer + (uint32)((&pSectionHeader[i])->sh_offset));
      break;
    }
  }

  //printf("\nSYMBOL TABLE : \n");
  //printf("\n%-17s%-17s%-15s%-15s%-15s\n\n","Value", "Size", "Bind", "Type", "Name");

  /* Display the symbol table */
  for(uint32 i=0; i< SymTabSize ;i++)
  {
    if(0 == strcmp(Symbol,&pSectionName[(&pSymTable[i])->st_name]))
    {
      printf("\nSYMBOL INFO (%s) : \n", Symbol);
      printf("\n%-17s%-17s%-15s%-15s%-15s\n","Value", "Size", "Bind", "Type", "Name");
      printf("0x%-15x0x%-15x%-15s%-15s%-15s\n",
              (&pSymTable[i])->st_value,
              (&pSymTable[i])->st_size,
              Elf_GetSymTabBindStr(ELF32_ST_BIND((&pSymTable[i])->st_info)),
              Elf_GetSymTabTypeStr(ELF32_ST_TYPE((&pSymTable[i])->st_info)),
              &pSectionName[(&pSymTable[i])->st_name]
            );
      break;
    }
  }

  return(TRUE);
}


/*******************************************************************************************************************
** Function:    
** Description: 
** Parameter:   
** Return:      
*******************************************************************************************************************/
boolean Elf_ListSrcFiles(char* Buffer)
{
  #define BUF_SIZE  1000

   char* startAdd      = NULL;
   char str[BUF_SIZE]  = {0};
   char dstr[BUF_SIZE] = {0};
   uint32 size         = 0;
   uint32 lines        = 0;
   uint32 srclst       = 0;
   uint32 i            = 0;
   boolean boSrcFound  = FALSE;
   boolean boFirstCall = TRUE;


  // section header table pointer
  pSectionHeader = (Elf32_Shdr*)((uint32)Buffer + (uint32)(pElfHeader->e_shoff));
    
  // section string table pointer
  pSectionName = (char*)((uint32)Buffer + (uint32)((&pSectionHeader[pElfHeader->e_shstrndx])->sh_offset));

  //search for section ".debug_line"
  for(i = 0; i < pElfHeader->e_shnum ; i++)
  {
    if(0 == strcmp(&pSectionName[(&pSectionHeader[i])->sh_name], ".debug_line"))
    {
      break;
    }
  }

  if(i == pElfHeader->e_shnum)
  {
    printf("\n .debug_line section is not found !\n");
    return(FALSE);
  }

  startAdd = (char*)((uint32)Buffer + (uint32)((&pSectionHeader[i])->sh_offset));
  size     = (&pSectionHeader[i])->sh_size;

  // open the tmp files
  FILE* tmp1 = fopen("tmp1", "wb+");
  FILE* tmp2 = fopen("tmp2", "wb+");
  FILE* tmp3 = fopen("tmp3", "wb+");

  if(tmp1 == NULL ||
     tmp2 == NULL ||
     tmp3 == NULL
    )
  {
  return(FALSE);
  }

  /* STEP 1: Create the full list of strings contains in .debug_line */
  for(uint32 cpt = 0; cpt < size ; cpt++)
  {
    if(startAdd[cpt] == '\0'                        ||
       startAdd[cpt] == ' '                         ||
       startAdd[cpt] == '.'                         ||
       startAdd[cpt] == '_'                         ||
       startAdd[cpt] == ':'                         ||
       startAdd[cpt] == '\\'                        ||
       startAdd[cpt] == '/'                         ||
       (startAdd[cpt] >= '0'&& startAdd[cpt] <='9') ||
       (startAdd[cpt] >= 'a'&& startAdd[cpt] <='z') ||
       (startAdd[cpt] >= 'A'&& startAdd[cpt] <='Z')
      )
      {
        if(startAdd[cpt] == '/')
        {
          startAdd[cpt] = '\\';
        }

        if(startAdd[cpt] != '\0')
        {
          fputc(startAdd[cpt], tmp1);
        }
        else
        {
          lines++;
          fputc('\n', tmp1);
        }
        fflush(tmp1);
    }
  }

  /* STEP 2: Create file with all src and inc files (contains duplicates) */
  fseek(tmp1, 0, SEEK_SET);
  for(uint32 line = 0; line < (lines + 1); line++)
  {
    if(fgets(str, BUF_SIZE, tmp1) != NULL && (strlen(str) >= 6))
    {
      for(uint32 pos = 0; pos < strlen(str); pos++)
      {
        if( 0 == strncmp((str + pos),".c\n"   , strlen(".c\n"))     ||
            0 == strncmp((str + pos),".cpp\n" , strlen(".cpp\n"))   ||
            0 == strncmp((str + pos),".h\n"   , strlen(".h\n"))     ||
            0 == strncmp((str + pos),".s\n"   , strlen(".s\n"))     ||
            0 == strncmp((str + pos),".asm\n" , strlen(".asm\n"))   ||
            0 == strncmp((str + pos),".v850\n", strlen(".v850\n"))
          )
        {
          fputs(str, tmp2);
          memset(str, 0, BUF_SIZE);
          srclst++;
          break;
        }
      }
      fflush(tmp2);
    }
  }

  /* STEP 3: Remove the duplicate strings  */
  fseek(tmp2, 0, SEEK_SET);
  for(uint32 line = 0; line < srclst; line++)
  {
    if(fgets(str, BUF_SIZE, tmp2) != NULL)
    {
       /* save the first */
       if(boFirstCall)
       {
          fputs(str, tmp3);
          fseek(tmp3, 0, SEEK_SET);
          boFirstCall = FALSE;
       }
       else
       {
         /* check if the str is already exsist in file 3*/
         while(fgets(dstr, BUF_SIZE, tmp3) != NULL)
         {
          if(0 == strcmp(str,dstr))
          {
            boSrcFound = TRUE;
            break;
          }
         }
       }

       if(!boSrcFound)
       {
          /* save it */
          fputs(str, tmp3);
       }

      memset(str, 0, BUF_SIZE);
      memset(dstr, 0, BUF_SIZE);
      boSrcFound = FALSE;
      fseek(tmp3, 0, SEEK_SET);
      fflush(tmp3);
    }
  }

  /* STEP 4: List all files */
  fseek(tmp3, 0, SEEK_SET);
  while(fgets(str, BUF_SIZE, tmp3) != NULL)
  {
    for(uint32 pos = 0; pos < strlen(str); pos++)
    {
      if(0 == strncmp((str + pos), ":\\", strlen(":\\")))
      {
        printf("%s", str);
        break;
      }
    }
    memset(str, 0, BUF_SIZE);
  }

  fclose(tmp1);
  fclose(tmp2);
  fclose(tmp3);

  /* delete tmp files */
  system("del /F tmp1");
  system("del /F tmp2");
  system("del /F tmp3");

  return(TRUE);
}