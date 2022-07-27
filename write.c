#include <stdio.h>
#include <stdlib.h>
#include "programvars.h"
#include "structs.h"

/*
  this function gets pointer to instruction table (linked list) and writes the instructions to file as requierd
  @param Ptr- pointer to instruction table
  @param ofd- pointer to file
*/
void writeInstruction(instructionRow *ptr, FILE *ofd)
{
  fprintf(ofd, "     %d %d \n", ICF-100,DCF);
  while(ptr!=NULL) /*run through the list of the instruction*/
  {
    while(!feof(ofd))/*pointing to the right line where we want to write the hexadecimal code in the new file*/
      fgetc(ofd);
    fprintf(ofd, "0%d %02x %02x %02x %02x\n", ptr->address, ptr->binary[0], ptr->binary[1], ptr->binary[2], ptr->binary[3]);
    ptr=ptr->next;
  }  
}

/*
  this function gets pointer to data table (linked list) and writes the rows to file as requierd
  @param Ptr- pointer to data table
  @param ofd- pointer to file
*/
void writeData(dataRow* ptr, FILE *fd) 
{
	int number=0;      
	int address = ICF;
	int i;

	while(ptr!=NULL) { /*go through the data table */

		if(ptr->bytesNum == 1) { /*ascii or db rows*/
			if(number % 4 == 0 && number != 0) {
				fprintf(fd, "\n");	
				fprintf(fd, "0%d %02x ",address,ptr->binary_db_asciz[0]);
			}

			else if(number == 0)
				fprintf(fd, "0%d %02x ",address,ptr->binary_db_asciz[0]);
			else
				fprintf(fd, "%02x ",ptr->binary_db_asciz[0]);
			number++;
			address++;		
		}

		else if(ptr->bytesNum == 2) { /*dh rows*/
			for(i=0;i<2;i++) {
				if(number % 4 == 0 && number != 0) {
					fprintf(fd, "\n");	
					fprintf(fd, "0%d %02x ",address,ptr->binary_dh[i]);
				}

				else if(number == 0)
					fprintf(fd, "0%d %02x ",address,ptr->binary_dh[i]);
				else
					fprintf(fd, "%02x ", ptr->binary_dh[i]);	
			number++;
			address++;
			}
		}

		else { /*dw rows*/
			for(i=0;i<4;i++) {
				if(number % 4 == 0) {
					fprintf(fd, "\n");						
					fprintf(fd, "0%d %02x ",address,ptr->binary_dw[i]);
				}
			
				else if(number == 0)
					fprintf(fd, "0%d %02x ",address,ptr->binary_dw[i]);
				else
					fprintf(fd, "%02x ",ptr->binary_dw[i]);	
				number++;
				address++;
			}
		}
	ptr = ptr->next;
	} /*while*/
}

/*
  this function gets pointer to symbol table (linked list) and writes the entry rows to file as requierd
  @param Ptr- pointer to symbol table
  @param fileName[]- name of file
*/
void writeEnt(symbolRow * ptr,char fileName[])
{
  FILE* entfd; /*file pointer */
  if(!(entfd=fopen(fileName,"w+")))/*open new entry file */
  {
    fprintf(stdout,"cannot open file\n");
    exit(0);
  }
  while(ptr!=NULL)/* run through the list of the symbols*/
  {
    if(ptr->entryFlag==1)
    	fprintf(entfd, "%s 0%d\n",ptr->symbol, ptr->value);/*write the hexadecimal code into the string array */

    ptr=ptr->next;
  }
}

/*
  this function gets pointer to extern table (linked list) and writes the rows to file as requierd
  @param Ptr- pointer to extern table
  @param fileName[]- name to file
*/
void writeExt(externRow * ptr,char fileName[])
{
  FILE *extfd;/*file pointer */
  if(!(extfd=fopen(fileName,"w+")))/* open new entry file*/
  {
    fprintf(stdout,"cannot open file\n");
    exit(0);
  }
  while(ptr!=NULL)/*run through the  extern table*/
  {
      fprintf(extfd, "%s 0%d\n", ptr->labelName, ptr->address);/*write the hexadecimal code into the string array*/
      ptr=ptr->next;  
  }
}     

/*
  this function gets pointers to all linked lists (tables), and writes them into files as requierd, using aboves functions
  @param instructionPtr- pointer to instruction table
  @param dataPtr- pointer to data table
  @param symbolPtr- pointer to symbol table
  @param externPtr- pointer to extern table
  @param fileObjectExtension - stores the name of the .ob file
  @param fileEntryExtension- stores the name of the .ent file
  @param fileExternExtension- stores the name of the .ext file
*/
void write(instructionRow* instructionPtr,dataRow* dataPtr, symbolRow *symbolPtr, externRow *externPtr,char* fileObjectExtension,char *fileEntryExtension,char *fileExternExtension)
{
  
  FILE* ofd;/*file pointer */

  if(!(ofd=fopen(fileObjectExtension,"w+")))/*open new object file*/
  {
    fprintf(stdout,"cannot open file\n");
    exit(0);
  }
  writeInstruction(instructionPtr, ofd);/* write instruction table in hexa to the object file*/
  
	
  writeData(dataPtr,ofd);/* write data table in hexa to the object file*/
  fclose(ofd);
  writeEnt(symbolPtr,fileEntryExtension);
  writeExt(externPtr,fileExternExtension);
}
