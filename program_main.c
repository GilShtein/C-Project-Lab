#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "programvars.h"
#include "passfuncts.h"
#include "writefuncts.h"

/*
wrriten by Gil Stein && Tomer Gertsenstein
final project - MMN14 August 2021

this program gets assembly files and transform its content into machine language and return object file if it succeeded as well as entry and extern files if needed
*/

int main(int argc, char *argv[])
{
  FILE* fd;
  FILE* ofd;
  int i;
  int countError=0;/* error flag, 0 no error, >0 have error*/
  int lengthNameFile;/*length of name of given file  */
  char *temp; /*saves file name*/
  char *temp2; /*saves file name with ob extension*/
  char *temp3; /*saves file name with ent extension*/
  char *temp4; /*saves file name with ext extension*/
  char *temp5;/*buffer to save file name*/
  char * str;/*pointer to use for strtok*/
  char ext[3];
  instructionRow *instructionPtr; /*Ptr to instructions linked list*/
  instructionRow *firstInstPtr;
  symbolRow *firstSymbolPtr; /*Ptr to symbols linked list*/
  symbolRow *symbolPtr;
  dataRow *firstDataPtr; /*Ptr to data linked list*/
  dataRow *dataPtr;
  externRow *externPtr; /*Ptr to extern symbols linked list*/
  externRow *firstExternPtr;
 
  if(argc == 1) {
	fprintf(stdout, "no file arguments for program\n");
	return 0;
  }

  for(i=1;i<argc;i++) /*iterate on files*/
  {
   
    IC = 100;
    DC = 0;
    lineNumber = 1; 
    lengthNameFile = strlen(argv[i]);
    temp = (char *) malloc(lengthNameFile * sizeof(char)); 
    temp5 = (char *) malloc(lengthNameFile * sizeof(char));
    temp2 = (char *) malloc(lengthNameFile * sizeof(char));
    temp3 = (char *) malloc(lengthNameFile * sizeof(char));
    temp4 = (char *) malloc(lengthNameFile * sizeof(char));

    strcpy(temp, argv[i]);               
    memcpy(ext, &temp[lengthNameFile-3], 3);
    /*check if file name is valid*/
    if(strcmp(ext, ".as") != 0) {
	fprintf(stdout, "arguments has to be assembly files - '.as' extension\n");
	return 0;
    } 
    strcpy(temp5, argv[i]); 
    str=strtok(temp5,".");
    strcpy(temp2,str);
    strcpy(temp3,str);
    strcpy(temp4,str);
    
    strcat(temp2,".ob");
    strcat(temp3,".ent");
    strcat(temp4,".ext");

    
	
	
         

   if(strcmp(temp,argv[i]) == 0) 
    { 
      
      fd=fopen(argv[i],"r+");
	
      if(fd)
      {

        instructionPtr=(instructionRow*)calloc(1,sizeof(instructionRow)); /*allocating memory to the first node of the instruction list */
      
        if(!instructionPtr)
        {
          fprintf(stdout,"cannot build instruction list\n");
          exit(0);
        }
	
        firstInstPtr = instructionPtr;
	instructionPtr->address = IC;
	instructionPtr->next = NULL;

        dataPtr=(dataRow*)calloc(1,sizeof(dataRow));/* allocating memory to the first node of the data list*/
       if(!dataPtr)
        {
          fprintf(stdout,"cannot build data list\n");
          exit(0);
        }

	firstDataPtr = dataPtr;
	dataPtr->address = DC;	
 	dataPtr->next=NULL;

        symbolPtr=(symbolRow*)calloc(1,sizeof(symbolRow));/* allocating memory to the first node of the symbol list */
        if(!symbolPtr)
        {
          fprintf(stdout,"cannot build symbol list\n");
          exit(0);
        }

	firstSymbolPtr = symbolPtr;	
	strcpy(symbolPtr->symbol,"");
        symbolPtr->next = NULL;	

        externPtr=(externRow *)calloc(1,sizeof(externRow));/*allocating memory to the first node of extern list */
        if(!externPtr)
        {
          fprintf(stdout,"cannot build data list\n");
          exit(0);
        }
        
	firstExternPtr = externPtr;
        externPtr->next=NULL;
        countError=firstpass(fd, symbolPtr, dataPtr, instructionPtr, externPtr); /*validate the first pass*/
        IC =100;
        lineNumber=1;
        if(countError==0) /*if no error reset the Pointers to the first node of the linked lists and file for second pass*/
        {
       	   	symbolPtr = firstSymbolPtr;
		dataPtr = firstDataPtr;
		instructionPtr = firstInstPtr;
		externPtr = firstExternPtr;
		rewind(fd);
        } 

	else { /*error in first pass, continue to next file*/
		fclose(fd);
		continue;
	}

       countError+=secondpass(fd, symbolPtr, dataPtr, instructionPtr, externPtr); /*validate second pass*/
       if(countError==0)
       {
         ofd = fopen(temp2, "w+");
         if(!ofd) {
		fprintf(stdout,"cannot open file: %s\n", temp2);
	} 
	
	else {
        
         /*reset pointers before writing to files*/
	 fclose(ofd); 
	 symbolPtr = firstSymbolPtr;
	 dataPtr = firstDataPtr;
	 instructionPtr = firstInstPtr;
	 externPtr = firstExternPtr;
         write(instructionPtr,dataPtr, symbolPtr, externPtr,temp2,temp3,temp4); /*write the linked lists to files as requierd*/
       }
	}
     }
     else
       fprintf(stdout,"cannot open file: %s\n",argv[i]);
   } /*end if temp=argv[i]*/
   
   else 
      fprintf(stdout,"Ilegal file name, file name can only end in as\n");
  
	
  	free(temp);
 	free(temp2);
  	free(temp3);
  	free(temp4);
	free(temp5);
	
  } /*  end for  */
	
	

	return 0;
}
