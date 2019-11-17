///////////////////////////////////////////////////////////////////////////////
//  Copyright Amine Chalandi 2019 - 2020.
//  Distributed under the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt
//  or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#include<io.h>



/*******************************************************************************************************************
** Function:    LoadInputFile2
** Description: load input file into the RAM
** Parameter:   char* path
** Return:      unsigned char*
*******************************************************************************************************************/
unsigned char* LoadInputFile(char* path)
{
    FILE* file = NULL;
    unsigned char* buf = NULL;
    unsigned int filesize = 0;

    if(path != NULL)
    {
      file = fopen(path, "rb");
    }

    if (file != NULL)
    {
        /* determinate the size of the file */
        fseek(file, 0, SEEK_END);
        filesize = ftell(file) + sizeof(char);
        fseek(file, 0, SEEK_SET);

        /* dynamic allocation */
        buf = (char*)malloc(filesize * sizeof(char));

        /* read the data from the file */
        fread(buf, filesize, sizeof(unsigned char), file);

        fclose(file);
        return(buf);
    }
    else
    {
        printf("\n\r error: Cannot open the file !\n\r");
        return(buf);
    }
}


/*******************************************************************************************************************
** Function:    SaveOutputFile
** Description: Save RAM buffer to a file
** Parameter:   char* path
** Return:      boolean
*******************************************************************************************************************/
boolean SaveOutputFile(char* path, string buf)
{
    FILE* file = NULL;

    if(path != NULL)
    {
      file = fopen(path, "w");
    }

    if (file != NULL)
    {
      fwrite(buf, strlen(buf), sizeof(char), file);
      
      fclose(file);
      return(TRUE);
    }
    else
    {
        DbgPrint("\n\r error: Cannot save the file !\n\r");
        return(FALSE);
    }
}
