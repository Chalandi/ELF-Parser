///////////////////////////////////////////////////////////////////////////////
//  Copyright Amine Chalandi 2019 - 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef __ELF_H__
#define __ELF_H__

#include<Common.h>


#define EI_NIDENT 16

#define ELFCLASSNONE 0U
#define ELFCLASS32   1U
#define ELFCLASS64   2U

#define ELFDATANONE 0U
#define ELFDATA2LSB 1U //Little endian
#define ELFDATA2MSB 2U //big endian

#define ET_NONE  0U //No file type
#define ET_REL   1U //Relocatable file
#define ET_EXEC  2U //Executable file

#define EM_NONE      0U    //Invalid machine
#define EM_SPARC     2U    //Sun SPARC
#define EM_386       3U    //Intel 80386
#define EM_68K       4U    //Freescale 68000
#define EM_486       6U    //Intel 80486
#define EM_MIPS      8U    //MIPS
#define EM_960       19U   //Intel i960
#define EM_PPC       20U   //Power Architecture
#define EM_PPC64     21U   //Power Architecture 64-bit mode
#define EM_V800      36U   //Renesas V800 series
#define EM_FR20      37U   //Fujitsu FR
#define EM_MCORE     39U   //Freescale MCore
#define EM_ARM       40U   //ARM
#define EM_SH        42U   //Hitachi SH
#define EM_TRICORE   44U   //Infineon TriCore
#define EM_COLDFIRE  52U   //Freescale ColdFire
#define EM_STARCORE  58U   //Freescale StarCore
#define EM_FIREPATH  78U   //Broadcom FirePath
#define EM_M32R      88U   //Renesas M32R
#define EM_BLACKFIN  106U  //ADI Blackfin

#define SHT_NULL       0u
#define SHT_PROGBITS   1u
#define SHT_SYMTAB     2u
#define SHT_STRTAB     3u
#define SHT_RELA       4u
#define SHT_NOTE       7u
#define SHT_NOBITS     8u
#define SHT_REL        9u

#define STB_LOCAL     0
#define STB_GLOBAL    1
#define STB_WEAK      2
#define STB_NUM       3
#define STB_LOOS     10
#define STB_LOPROC   13
#define STB_HIPROC   15

#define STT_NOTYPE      0
#define STT_OBJECT      1
#define STT_FUNC        2
#define STT_SECTIONS    3
#define STT_FILE        4
#define STT_COMMON      5
#define STT_TLS         6
#define STT_NUM         7
#define STT_LOOS       10
#define STT_HIOS       12
#define STT_LOPROC     13
#define STT_HIPROC     15

#define TYP_UNKNOWN_ELF     0
#define TYP_RELOCATABLE_ELF 1
#define TYP_EXECUTABLE_ELF  2

#define SHF_WRITE     1
#define SHF_ALLOC     2
#define SHF_EXECU     4

#define ELF32_ST_BIND(x)   (Elf32_Byte)((x)>>4)
#define ELF32_ST_TYPE(x)   (Elf32_Byte)((x) & 0x0f)

typedef unsigned char  Elf32_Byte;
typedef unsigned long  Elf32_Addr;
typedef unsigned short Elf32_Half;
typedef unsigned long  Elf32_Off;
typedef signed long    Elf32_Sword;
typedef unsigned long  Elf32_Word;


//ELF header struct
typedef struct {
  Elf32_Byte e_ident[EI_NIDENT];    //Identifies the file as an ELF file and provides machine-independent data to decode the contents.
  Elf32_Half e_type;                //Identifies the ELF file type.
  Elf32_Half e_machine;             //Specifies the target architecture for an individual file.
  Elf32_Word e_version;             //Identifies the ELF file version.
  Elf32_Addr e_entry;               //Specifies the virtual address to which the system first transfers control upon starting the process.
  Elf32_Off  e_phoff;               //Specifies the program header table's file offset in bytes. Otherwise, this field contains zero.
  Elf32_Off  e_shoff;               //Specifies the section header table's file offset in bytes. Otherwise, this field contains zero.
  Elf32_Word e_flags;               //Specifies processor-specific flags associated with the file.
  Elf32_Half e_ehsize;              //Specifies the ELF header size in bytes.
  Elf32_Half e_phentsize;           //Specifies the size in bytes of one entry in the file's program header table.
  Elf32_Half e_phnum;               //Specifies the number of entries in the program header table.
  Elf32_Half e_shentsize;           //Specifies the section header's size in bytes.
  Elf32_Half e_shnum;               //Specifies the number of entries in the section header table.
  Elf32_Half e_shstrndx;            //Specifies the index of the section containing the string table used for section names.
} Elf32_Ehdr;


//section header structure
typedef struct {
  Elf32_Word sh_name;              //Specifies the name of the section. Its value is an index into the section header string table section
  Elf32_Word sh_type;              //Specifies the section's contents and semantics.
  Elf32_Word sh_flags;             //Specifies 1-bit flags that describe miscellaneous attributes of the section.
  Elf32_Addr sh_addr;              //Specifies the address at which the section's first byte should reside. Otherwise, this field contains zero. (Physical address)
  Elf32_Off  sh_offset;            //Specifies the byte offset from the beginning of the file to the first byte in the section.
  Elf32_Word sh_size;              //Specifies the section's size in bytes.
  Elf32_Word sh_link;              //Contains a section header table index link, whose interpretation depends on the section type.
  Elf32_Word sh_info;              //Contains extra information, whose interpretation depends on the section type.
  Elf32_Word sh_addralign;         //Some sections have address alignment constraints.
  Elf32_Word sh_entsize;           //Some sections contain a table of fixed-size entries, such as a symbol table.
} Elf32_Shdr;

//program header struct
typedef struct {
  Elf32_Word p_type;              //The kind of segment this array element describes and how to interpret the array element's information.
  Elf32_Off  p_offset;            //Offset from the beginning of the file at which the first byte of the segment resides.
  Elf32_Addr p_vaddr;             //Address at which the first byte of the segment resides in memory.
  Elf32_Addr p_paddr;             //For targets where physical addressing is relevant, this field gives the segment's physical address.
  Elf32_Word p_filesz;            //Number of bytes in the file image of the segment; it may be zero.
  Elf32_Word p_memsz;             //Number of bytes in the memory image of the segment; it may be zero.
  Elf32_Word p_flags;             //Flags relevant to the segment.
  Elf32_Word p_align;             //This field specifies the value to which the segments are aligned in memory and in the file.
} Elf32_Phdr;

//symbol table struct
typedef struct {
  Elf32_Word st_name;             //Contains an index into the file's symbol string table
  Elf32_Addr st_value;            //Specifies the associated symbol's value.
  Elf32_Word st_size;             //Many symbols have associated sizes.
  Elf32_Byte st_info;             //Specifies the symbol's binding attributes and type.
  Elf32_Byte st_other;            //Contains the value zero and has no defined meaning. (Pad)
  Elf32_Half st_shndx;            //Every symbol table entry is “defined” in relation to some section. this field contains the relevant section header table index.
} Elf32_Sym;

typedef struct
{
  uint32 machine;
  char const * const  name;
}sMachine;

typedef struct
{
  uint32 type;
  char const * const name;
}sSectionType;


typedef struct
{
  Elf32_Byte code;
  char const * const name;
}sSymTabBind;

typedef struct
{
  Elf32_Byte code;
  char const * const name;
}sSymTabType;


typedef struct
{
  uint32 code;
  char   name;
}sSectionAttr;


typedef struct
{
  uint32 code;
  char const * const name;
}sElfType;

boolean Elf_ProcessElfHeader(char* Buffer, boolean PrintInfo);
boolean Elf_SectionHeaderTable(char* Buffer);
boolean Elf_SymbolTable(char* Buffer);
boolean Elf_ExtractBinaryToC(char* Buffer, char* path);
boolean Elf_ExtractBinaryToS19(char* Buffer, char* path);
boolean Elf_SearchInfo(char* Buffer, char* Symbol);
boolean Elf_ListSrcFiles(char* Buffer);
#endif
