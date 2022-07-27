#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "utilies.h"

char savedWords[33][6] = { /*33 saved words exists*/
			"asciz",	
			"entry",
			"dd",
			"dw",
			"db",
			"stop",	
			"call",
			"la",
			"jmp",
			"sh",
			"lh",	
			"sw",
			"lw",
			"sb",
			"lb",
			"bgt",
			"blt",
			"beq",
			"bne",	
			"nori",
			"ori",
			"andi",
			"subi",
			"addi",
			"mvlo",
			"mvhi",
			"move",	
			"nor",
			"or",
			"and",
			"sub",
			"add"
		};
/*
  this function checks if a label is a saved word
  @labelPtr - the label
  @return 1 if no 0 if yes
*/
int isSavedWord(char *labelPtr) {
	int i; /*index*/
        
	for(i = 0; i < 33; i++) {
		if(strcmp(labelPtr, savedWords[i]) == 0) {
			 
			
			fprintf(stdout, "line %d: Label is saved word\n", lineNumber);		
			return 0;
		}	
	}	
	return 1;
}

/*
  this function checks label synatx return if there are errors or not
  @param bufLine - line of text
  @param labelPtr - the label
  @param j - index through text line
  @return 1 if no 0 if yes
*/
int checkLabelSyntax(char *bufLine, char *labelPtr, int *j) {
	int i =0; /*index*/

	skipSpaces(bufLine, j);
	while(bufLine[*j] != ':') {
		if(!(bufLine[*j] >= 'a' && bufLine[*j] <= 'z') && !(bufLine[*j] >= 'A' && bufLine[*j] <= 'Z') && !(bufLine[*j] >= '0' && bufLine[*j] <= '9')) {
					
			fprintf(stdout, "line %d: Illegal character at label\n", lineNumber);
			return 0;
		}
		
		if(i == MAX_LABEL_SIZE) {
			fprintf(stdout, "line %d: Label too long\n", lineNumber);
			return 0;
		}
		
		if((i == 0) && (bufLine[*j] >= '0' && bufLine[*j] <= '9')) {
			fprintf(stdout, "line %d: Label cannot start with digit\n", lineNumber);
			return 0;
		}
		
		labelPtr[i] = bufLine[*j];
		
		*j+=1;
		i++;
	}

	labelPtr[i] = '\0'; /*End of label*/
	
	return 1;
}


/*
  this function if syntax and if label is not saved word
  @param bufLine - line of text
  @param labelPtr - the label
  @param j - index through text line
  @return 1 if label is good 0 if not
*/
int handleLabel(char *bufLine, char *labelPtr, int *j) { 

	/*Check if syntax is correct and copy the label to labelPtr memory allocation*/
	if (!checkLabelSyntax(bufLine, labelPtr, j))
		return 0;

	/*Check if label isn't saved word*/
	if(!isSavedWord(labelPtr))
		return 0;

	return 1;
}

/*
  this function inserts label to symbol table
  @param labelPtr - the label
  @param firstSymbolRow- pointer to symbol table
  @param attribute - attribute of symbol row(data, code, extern..)
  @return 1 if label is good 0 if not
*/
int insertLabel(char *labelPtr, symbolRow *firstRow, char *attribute) {
	/*Check if the label already in the label table*/
	symbolRow *row = firstRow; /*pointer to symbol table*/

	if(strlen(row->symbol) != 0) {	/*is first row of table*/
		if(strcmp(labelPtr, row->symbol) == 0) { /*check first node*/

			if(strcmp(attribute,"extern")!=0) {
				fprintf(stdout, "line %d: label already in the symbol table\n", lineNumber);
				return 0;
			}

			if(strcmp(row->attribute,"extern")!=0) {
				fprintf(stdout, "line %d: label already in the symbol table\n", lineNumber);
				return 0;
			}
	
			else {
				return 1;								
			}			
		}
	}


		while(row->next != NULL) { /*check other nodes*/
			if(strcmp(labelPtr, row->symbol) == 0) {

				if(strcmp(attribute,"extern")!=0){
					fprintf(stdout, "line %d: label already in the symbol table\n", lineNumber);
					return 0;
				}

				if(strcmp(row->attribute,"extern")!=0){
					fprintf(stdout, "line %d: label already in the symbol table\n", lineNumber);
					return 0;
				}
	
				else {
					return 1;				
				}
			}
			row = row->next;
		}

		if(strcmp(labelPtr, row->symbol) == 0) {

				if(strcmp(attribute,"extern")!=0){
					fprintf(stdout, "line %d: label already in the symbol table\n", lineNumber);
					return 0;
				}

				if(strcmp(row->attribute,"extern")!=0){
					fprintf(stdout, "line %d: label already in the symbol table\n", lineNumber);
					return 0;
				}
	
				else {
					return 1;				
				}
			}
	
		if(strlen(row->symbol) != 0) {
			row->next = (symbolRow *) malloc(sizeof(symbolRow));
			if(row->next == NULL) {
				printf("Cannot allocate memory for program\n");
				exit(0);
			}

			row = row->next;
			row->next = NULL;
		}
		
		strcpy(row->symbol, labelPtr);
		if(strcmp(attribute, "extern") == 0) {		
			row->value = 0;
		}

		else {
		 	if(strcmp(attribute, "data")==0)	
				row->value = DC;
			else
				row->value = IC;
		}

		strcpy(row->attribute, attribute);

	return 1;	
}

/*
  this function inserts gets label from sentence
  @bufLine - line of text
  @param labelArg - the label
  @param j - line index
  @return 1 if label is good 0 if not
*/
int getLabel(char *bufLine, char *labelArg, int *j) {
	int i = 0; /*index*/
			
	skipSpaces(bufLine, j);
	while(!(isspace(bufLine[*j]) || bufLine[*j] == '\0')) {
				
		if(!(bufLine[*j] >= 'a' && bufLine[*j] <= 'z') && !(bufLine[*j] >= 'A' && bufLine[*j] <= 'Z') && !(bufLine[*j] >= '0' && bufLine[*j] <= '9')) {			
			fprintf(stdout, "line %d: Illegal character at label\n", lineNumber);
			return 0;
		}

		if(i == MAX_LABEL_SIZE) {
			fprintf(stdout, "line %d: Label to long\n", lineNumber);
			return 0;
		}		
	
		if((i == 0) && (bufLine[*j] >= '0' && bufLine[*j] <= '9')) {
			fprintf(stdout, "line %d: Label cannot start with digit\n", lineNumber);
			return 0;
		}

		labelArg[i] = bufLine[*j];
		
		*j+=1;
		i++;
	}

	labelArg[i] = '\0'; /*End of label*/
	
	if(!isSavedWord(labelArg)) {		
		return 0;
	}

	return 1;
}

/*
  this function frees the symbol linked list
  @param row- pointer to symbol table
*/
void freeSymbolTable(symbolRow *row)
{
  symbolRow* current=row;
  symbolRow* next;
  
 
  while(current!=NULL)
  {
    next=current->next;
    free(current);
    current=next;
  }
  row=NULL;
}


/*
  this function updates the symbol table after first pass
  @param row- pointer to symbol table
*/
void updateSymbolTable(symbolRow *row) { /*WORKING*/
	
	if(row->next == NULL) {
		if(strstr(row->attribute, "data"))
			row->value+=IC;
		return;
	}
	updateSymbolTable(row->next);
	if(strstr(row->attribute, "data"))
		row->value+=IC;
}

/*
  this function inserts skip label in sentence(adevence index after it)
  @bufLine - line of text
  @param j - line index
*/
void skipLabel(char *bufLine, int *j) {

	skipSpaces(bufLine, j);
	while(bufLine[*j] != ':') {
		*j+=1;
	}
	*j+=1;
}

/*
  this function searchs for label symbol table
  @param label - symbol to search
  @param row - pointer to symbol table
  @return pointer to symbol table if label found, null if not
*/
symbolRow* searchSymbol(char * label, symbolRow *row) {
	while(1) {	
		if(strcmp(label, row->symbol) == 0)
			return row;
		else if(row->next == NULL)
			return NULL;
		else
			row = row->next;
	}
}

