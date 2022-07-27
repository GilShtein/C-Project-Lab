#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "programvars.h"
#include "utilies.h"

#define EMPTY_SENTENCE 10
#define COMMENT_SENTENCE 20
#define GUIDENCE_SENTENCE 30
#define INSTRUCTION_SENTENCE 40
#define LABEL_EXISTS 50
#define ERROR_SENTENCE 60
#define MAX_LINE_SIZE 82
#define DATA_TYPE 1
#define ENTRY_TYPE 2
#define EXTERN_TYPE 3

/*
  this function return the type of line assumed by the first char which is not white space 
  @param bufLine- line of text
  @param j - index inside line
  @return type of sentence
*/
int sentenceType(char *bufLine, int j) {
	
	skipSpaces(bufLine, &j);  /*Skip spaces in line begging*/

	if (bufLine[j] == '\n') { /*Empty line*/
		return EMPTY_SENTENCE;
	}

	if(bufLine[j] == ';') { /*Comment line starts with ';'*/		
		return COMMENT_SENTENCE;
	}

	if(bufLine[j] == '.') { /*Guidence sentence starts with '.'*/
		return GUIDENCE_SENTENCE;
	}
 

	/*Now we don't know if there might be a label or not and if it is guidence or instruction sentence*/
	/*We assume there is a label if the first word of the sentence contains ':'*/
	while(bufLine[j] != ':') {
		if (isspace(bufLine[j])) { /*First word without ':', assumes instruction*/
			return INSTRUCTION_SENTENCE;
		}
		j+=1;
	}

	return LABEL_EXISTS; /*There was ':' in the first word of the line so we assume that's a label*/	
}

/*
  this function return the type of line assumed by the first char which is not white space, while the index is after the label starting the line, if it was.
  @param bufLine- line of text
  @param j - index inside line
  @return type of sentence
*/
int sentenceTypeAfterLabel(char *bufLine, int *j) {

	skipSpaces(bufLine, j);  /*Skip spaces in line begging*/

	if(bufLine[*j] == '.') /*Guidence sentence starts with '.' after label*/
		return GUIDENCE_SENTENCE;

	else if(bufLine[*j] == '\0') {
		fprintf(stdout,"line %d: missing text after label, illegal assembly sentence\n",lineNumber);
		return ERROR_SENTENCE;
	}

	else
		return INSTRUCTION_SENTENCE;
} 

/*
  this function advence line number by 1 and free line of text and label allocated dynamiclly 
  @param bufLine- memory allocated to store line of text
  @param labelPtr - memory allocate to store label of line
*/
void nextLine(char *bufLine,char* labelPtr) {
	lineNumber++;
	free(bufLine);
        free(labelPtr);
}

/*
  this function manage all the first pass, iterate through the file line by line, check syntax and stores values as needed 
  @param filePointer- pointer to file
  @param firstSymbolRow- pointer to symbol table
  @param firstDataRow- pointer to data table
  @param firstInstRow- pointer to instruction table
  @param firstExternRow- pointer to extern table
  @return 1 if errors exists in file, 0  otherwise
*/
int firstpass(FILE *filePointer, symbolRow *firstSymbolRow, dataRow *firstDataRow, instructionRow *firstInstRow, externRow *firstExternRow)
{	
	char *bufLine; /*line of text*/
        char * labelPtr; /*label of line*/
	int i; /*for checking line size*/
 	int j; /*index through line*/
	int labelExists; /*flag if label exist*/
	int typeLineOfText; /*flag to check what type of sentence we get*/
	int errorInFile = 0; /*flag to check if there was any error in the file*/	
        
	while(!feof(filePointer))
	{
                labelPtr = (char*)malloc(MAX_LINE_SIZE * sizeof(char));
		if (labelPtr == NULL) {
			printf("Cannot allocate memory for program");
			exit(0);
		}
                strcpy(labelPtr,"");
		/*Allocate memory for new line*/
		bufLine = (char*)malloc(10 * MAX_LINE_SIZE * sizeof(char));
		if (bufLine == NULL) {
			printf("Cannot allocate memory for program");
			exit(0);
		}

		j = 0; /*indexing through bufLine*/

		/*Read line from file into buffer*/
		/*Check if line do not fit the size*/
		fgets(bufLine, 10 * MAX_LINE_SIZE, filePointer);
		i = 0;        	
		while(bufLine[i] != '\0') {
			i++;
     		}

		if(i > 80) {
			errorInFile = 1;
			fprintf(stdout, "Line %d: line is too long\n", lineNumber);
			nextLine(bufLine,labelPtr);
			continue;
		}
 
		typeLineOfText = sentenceType(bufLine, j); /*return what kind of sentence or if there's a label*/
                
		if(typeLineOfText==EMPTY_SENTENCE) {
			nextLine(bufLine,labelPtr);			
			continue;
		}
		
		else if(typeLineOfText==COMMENT_SENTENCE) {
			nextLine(bufLine,labelPtr);			
			continue;
		}
                
		else if(typeLineOfText==LABEL_EXISTS) {
           		
			labelExists = 1;			
		        j=0;
                        
			if(!handleLabel(bufLine, labelPtr, &j)) { /*Error in sentnce, label unvalid*/ 
				errorInFile = 1;
			nextLine(bufLine,labelPtr);				
				continue; /*if error continue to next line*/
			}
			j++; /*bufLine[j] points to ":" we want him to point to the next char*/
		}
			
		/*At this point we store the label at labelPtr and bufLine points to the first char after the label, now we need to understand if it is a guidence or instruction sentence*/
		if(labelExists)
			typeLineOfText = sentenceTypeAfterLabel(bufLine, &j); /*Now checks the line after the label*/
			
		if(typeLineOfText==ERROR_SENTENCE) {
			errorInFile = 1;
			nextLine(bufLine,labelPtr);
			continue;
		}		

		else if(typeLineOfText==GUIDENCE_SENTENCE) {
			if (!guidenceHandle(bufLine, labelPtr, &j, firstSymbolRow, firstDataRow, firstExternRow)) {
				errorInFile = 1;
				nextLine(bufLine,labelPtr);
				continue;
			}
		}

		else /*(typeLineOfText==INSTRUCTION_SENTENCE)*/ {
                          
			if(!instructionHandle(bufLine, labelPtr,&j , firstSymbolRow, firstInstRow)) {
				errorInFile = 1;
				nextLine(bufLine,labelPtr);			
				continue;
			}
			IC+=4;
		} 
		nextLine(bufLine,labelPtr);
                
	}

	if(errorInFile) {
		freeSymbolTable(firstSymbolRow);
		freeDataTable(firstDataRow);
		freeInstTable(firstInstRow);
		return 1;
	} 

	ICF = IC;
	DCF = DC; 
	
	updateSymbolTable(firstSymbolRow);
	updateDataTable(firstDataRow);

	return 0;
}

/*
  this function manage all the second pass, iterate through the file line by line, check syntax and stores values as needed 
  @param filePointer- pointer to file
  @param firstSymbolRow- pointer to symbol table
  @param firstDataRow- pointer to data table
  @param firstInstRow- pointer to instruction table
  @param firstExternRow- pointer to extern table
  @return 1 if errors exists in file, 0  otherwise
*/
int secondpass(FILE *filePointer, symbolRow *firstSymbolRow, dataRow *firstDataRow, instructionRow *firstInstRow, externRow *firstExternRow) {
	
	char *bufLine; /*line of text*/
        char *labelPtr; /*label of line*/
	int i; /*for checking line size*/
 	int j; /*index through line*/	
	int typeLineOfText; /* flag to check what type of sentence we get */
	int errorInFile = 0; /*flag to check if there was any error in the file*/
	
	while(!feof(filePointer))
	{

                labelPtr = (char*)malloc(MAX_LINE_SIZE * sizeof(char));
		if (labelPtr == NULL) {
			printf("Cannot allocate memory for program");
			exit(0);
		}
                strcpy(labelPtr,"");
		/*Allocate memory for new line*/
		bufLine = (char*)malloc(MAX_LINE_SIZE * sizeof(char));
		if (bufLine == NULL) {
			printf("Cannot allocate memory for program");
			exit(0);
		}		

		j = 0; /*run with index through the line*/

		/*Read line from file into buffer*/
		/*Check if line do not fit the size*/
		fgets(bufLine, 82, filePointer);
		i = 0;        	
		while(bufLine[i] != '\0') {
			i++;
     		}
		
		if(i > 80) {
			errorInFile = 1;
			nextLine(bufLine,labelPtr);
			continue;
		}

		/*getLine succeeded, the line fits the size*/	 
		typeLineOfText = sentenceType(bufLine, j); /*return what kind of sentence or if there's a label*/

		if(typeLineOfText==EMPTY_SENTENCE) {
			nextLine(bufLine,labelPtr);			
			continue;
		}
		
		else if(typeLineOfText==COMMENT_SENTENCE) {
			nextLine(bufLine,labelPtr);			
			continue;
		}
		
		else if(typeLineOfText==LABEL_EXISTS) { 
			skipLabel(bufLine, &j);
			typeLineOfText = sentenceTypeAfterLabel(bufLine, &j);
		}
		

		if(typeLineOfText==GUIDENCE_SENTENCE) {
			if (!handleGuidenceSecPass(bufLine, &j, firstSymbolRow, firstDataRow)) {
				errorInFile = 1;
				nextLine(bufLine,labelPtr);
				continue;
			}
		}

		else /*(typeLineOfText==INSTRUCTION_SENTENCE)*/ {                        
			if(!instHandleSecPass(bufLine, labelPtr, &j, firstSymbolRow, firstInstRow, firstExternRow)) {
				errorInFile = 1;
				nextLine(bufLine,labelPtr);		
				continue;
			}
			IC+=4;
		} 
		nextLine(bufLine,labelPtr);

	}

       while(firstSymbolRow != NULL) { /*iterate to last node of linked list*/
		firstSymbolRow = firstSymbolRow->next;
	}	

	while(firstDataRow != NULL) { /*iterate to last node of linked list*/
		firstDataRow= firstDataRow->next;
	}

	while(firstInstRow != NULL) { /*iterate to last node of linked list*/
		firstInstRow = firstInstRow->next;
	}
		
	rewind(filePointer);
	fclose(filePointer);
	freeInstTable(firstInstRow);
	freeSymbolTable(firstSymbolRow);
	freeDataTable(firstDataRow);

	if(errorInFile)
		return 1;
	return 0;	
}
