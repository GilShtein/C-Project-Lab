#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>
#include "programvars.h"
#include "utilies.h"

#define DATA_COMMAND 1
#define EXTERN_COMMAND 2
#define ENTRY_COMMAND 3
#define ASCIZ_COMMAND 4
#define ILLEGAL_COMMAND 5

/*
  this function returns the type of guidence command
  @param command[] - guidence command string
  @return type of command
*/
int commandType(char command[]) {
	if ((strcmp(command, "dh") == 0) || (strcmp(command, "dw") == 0) || (strcmp(command, "db") == 0))
		return DATA_COMMAND;
	
	else if(strcmp(command, "extern") == 0)
		return EXTERN_COMMAND;

	else if(strcmp(command, "entry") == 0)
		return ENTRY_COMMAND;

	else if(strcmp(command, "asciz") == 0)
		return ASCIZ_COMMAND;

	else
		return ILLEGAL_COMMAND;
}
 
/*
  this function inserts asciz command to data table (linked list)
  @param string - the asciz senctence attribute
  @param row - pointer to data table
*/
void insertAsciz(char *string, dataRow *row) { 	
	int i = 0; /*string's index*/
	int c; /*value to store string's char*/

	/*Get to the last row in the list*/		
	while(row->next != NULL)
		row = row->next;
		
	while(string[i] != '\0') {
		/*Allocate memory for next row*/
		if(DC == 0) /*first row*/ {
			
			row->address = DC;
			row->bytesNum = 1;
			c = string[i];
			DecimalToBinary(c, 1, row);
		}

		else {
			
			row->next = (dataRow *) malloc(sizeof(dataRow));
			if(row->next == NULL) {
				printf("Cannot allocate memory for program");
				exit(0);
			}
			
			row = row->next;
			row->next = NULL;
			row->address = DC;
			row->bytesNum = 1;
			c = string[i];
			DecimalToBinary(c, 1, row);
      		}
	i++;
	DC+=1;
	}

	if(DC == 0) /*first row*/ {
			
			row->address = DC;
			row->bytesNum = 1;
			c = '\0';
			DecimalToBinary(c, 1, row);
		}

	else {
		
			row->next = (dataRow *) malloc(sizeof(dataRow));
			if(row->next == NULL) {
				printf("Cannot allocate memory for program");
				exit(0);
			}
			
			row = row->next;
			row->next = NULL;
			row->address = DC;
			row->bytesNum = 1;
			c = '\0';
			DecimalToBinary(c, 1, row);
      		}
	DC+=1;
}

/*
  this function inserts data command to data table (linked list)
  @param value - the data senctence attribute
  @param bytesNumber - number of bytes in data command
  @param row - pointer to data table
  @return 1 if errors exists in file, 0  otherwise
*/
void insertData(int value, int bytesNumber, dataRow *row) {
	/*Get to the last row in the list*/
	
	while(row->next != NULL)
		row = row->next;
	
	if(DC == 0) { /*first row*/
		row->address = DC;		
		row->bytesNum = bytesNumber;
		if(bytesNumber == 1) {
			DecimalToBinary(value, bytesNumber, row);
		}
		
		else if(bytesNumber == 2) {
			DecimalToBinary(value, bytesNumber, row);
		}
		
		else if(bytesNumber == 4) {
			DecimalToBinary(value, bytesNumber, row);
		}
		
	}

	else {
		/*Allocate memory for next row*/
		row->next = (dataRow *) malloc(sizeof(dataRow));
		if(row->next == NULL) {
			printf("Cannot allocate memory for program");
			exit(0);
		}
	  
		/*Makes new row*/
		row = row->next;
	    	row->next = NULL;
		row->address = DC;
		row->bytesNum = bytesNumber;
		if(bytesNumber == 1) {
			DecimalToBinary(value, bytesNumber, row);
		}
		
		else if(bytesNumber == 2) {
			DecimalToBinary(value, bytesNumber, row);
		}
		
		else if(bytesNumber == 4) {
			DecimalToBinary(value, bytesNumber, row);
		}
	}
	DC+=bytesNumber;	
}

/*
  this function handle data command check syntax and then gets attributes
  @param bufLine - text line
  @param labelPtr - label of line
  @param command - command of line
  @param j - index through text line
  @param firstSymbolRow- pointer to symbol table
  @param firstDataRow- pointer to data table 
  @return 1 if errors exists in file, 0  otherwise
*/
int handleDataCommand(char *bufLine, char *labelPtr, char *command, int *j, symbolRow *firstSymbolRow, dataRow *firstDataRow) {
	int bytesNumber; /*number of bytes*/
	int maxNumber; /*maximum valid number for command*/
	int minNumber; /*minimum valid number for command*/
	int value = 0; /*value for command's attribute*/
	int sign = 1; /*keep sign of command's attribute*/
        unsigned int uvalue = 0; /*unsigned int for dw command*/
	char number[15];/*array to store max value as array of chars*/
	int i;/*index*/
	int l;/*index*/
	

	if(!(strlen(labelPtr) == 0)) { /*Checks if there's label in the sentence*/
		if(!insertLabel(labelPtr, firstSymbolRow, "data"))
			return 0;
	}
	
	if(strcmp(command, "db") == 0) {
		bytesNumber = 1;
	}
	else if(strcmp(command, "dh") == 0) {
		bytesNumber = 2;
	}
	else /*dw*/ {
		bytesNumber = 4;
	}

	
	maxNumber = (int)(pow(2,((bytesNumber*8) -1)) -1);
	minNumber = (-1) * (int)(pow(2,(bytesNumber*8) -1));
	
	while(bufLine[*j] != '\0') {
		skipSpaces(bufLine, j);
		
		if(bufLine[*j] == '-') {
			sign = -1;
			*j+=1;
		}
		else if(bufLine[*j] == '+') {
			sign = 1;
			*j+=1;
		}		

		if(bufLine[*j] < '0' || bufLine[*j] > '9') {
			fprintf(stdout, "line %d: wrong attribue to command\n", lineNumber);
			return 0;
		}
		
		else {		

			if(bytesNumber != 4) {	
				value = (int)(atoi(&bufLine[*j]));
				value*=sign;
				while((bufLine[*j] >= '0') && (bufLine[*j] <= '9')) 
					*j+=1; 
			    	
				if(value < minNumber || value > maxNumber) {
					fprintf(stdout, "line %d: value doesn't fit to number of bytes allocated by command\n", lineNumber);
					return 0;
				}
				insertData(value, bytesNumber, firstDataRow);
			}

			else { /*if bytesnumber is 4*/
				uvalue = (unsigned int)(atoi(&bufLine[*j]));
				
				i=*j;
				while((bufLine[*j] >= '0') && (bufLine[*j] <= '9')) 
					*j+=1;
				
				if(uvalue == maxNumber || uvalue*sign == minNumber) /*if value is larger than 32 bits*/{
					l=0;
					if(sign==1)
						strcpy(number,"2147483647");
					else
						strcpy(number,"2147483648");
					while((bufLine[i] >= '0') && (bufLine[i] <= '9')) 
					{
						if(l<=9)
						{
							if(number[l]<bufLine[i])
							{
								fprintf(stdout, "line %d: value doesn't fit to number of bytes allocated by command\n", lineNumber);
							return 0;
							}
							
						}
						else/*number is bigger than 10 digits*/
						{
							fprintf(stdout, "line %d: value doesn't fit to number of bytes allocated by command\n", lineNumber);
							return 0;	
						}
						l+=1;
						i++;
					} 
					
				}

				uvalue*=sign;
				insertData(uvalue, bytesNumber, firstDataRow);
			}
		}
		
		/*Now checks for comma*/
		skipSpaces(bufLine, j);
                
		if(bufLine[*j] == '\0')
			break;
		if((bufLine[*j] < '0' || bufLine[*j] > '9') && bufLine[*j] != ','&& bufLine[*j]!='\0'&& !isspace(bufLine[*j])) {
                       
			fprintf(stdout, "line %d: wrong attribue to command\n", lineNumber);
			return 0;
		}
		else if(bufLine[*j] != ','&& bufLine[*j]!='\0'&& !isspace(bufLine[*j])) {
			fprintf(stdout, "line %d: missing comma\n", lineNumber);
			return 0;
		}
		*j+=1;
		/*Initialize for the next value*/
		value = 0;
		sign = 1;
	}	
	
	return 1;
}

/*
  this function handels asciz command. check syntax, then gets attriutes (linked list)
  @param bufLine - text line
  @param labelPtr - label of line
  @param j - index through text line
  @param firstSymbolRow- pointer to symbol table
  @param firstDataRow- pointer to data table 
  @return 1 if errors exists in file, 0 otherwise
*/
int handleAscizCommand(char *bufLine, char *labelPtr, int *j, symbolRow *firstSymbolRow, dataRow *firstDataRow) {
	
	char string[81]; /*store asciz attribute*/
	int i = 0; /*index thorugh string*/	
	int lastApoj = 0; /*last quotation position in text line*/
	int lastApoi = 0; /*last quotation position inside the attribute*/

	if(!(strlen(labelPtr) == 0)) { /*Checks if there's label in the sentence*/
		if(!insertLabel(labelPtr, firstSymbolRow ,"data"))
			return 0;
	}

	skipSpaces(bufLine, j);

	if(bufLine[*j] != '"') { /*checks stirngs start with "*/
		fprintf(stdout, "line %d: string must start with \" \n", lineNumber);
		return 0;
	}
	
	*j+=1;	
	
	while(bufLine[*j] != '\0') {				
		if((bufLine[*j] < 32 || bufLine[*j] > 126) && bufLine[*j]!='\t' && bufLine[*j]!='\n') {
			fprintf(stdout, "line %d: not ascii printable character\n", lineNumber);
			return 0;
		}
		
		string[i] = bufLine[*j];		
		if(bufLine[*j] == '"') {
			lastApoj = *j;
			lastApoi = i;
		}

		i++;
		*j+=1;		
	}
 
	if(i == 0) {
		fprintf(stdout, "line %d: string must end with \" \n", lineNumber);
		return 0;
	}
	
	lastApoj++; 
	if(!endOfInstruct(bufLine, &lastApoj))
		return 0;
	
	
	if (lastApoi == 0) { /*checks stirng end with "*/
		fprintf(stdout, "line %d: string must end with \" \n", lineNumber);
		return 0;
	}

	else {		
		string[lastApoi] = '\0';
	}

	insertAsciz(string,firstDataRow);
	return 1;
}

/*
  this function handels extern command, check syntax then gets attribute
  @param bufLine - text line
  @param labelPtr - label of line
  @param j - index through text line
  @param firstSymbolRow- pointer to symbol table
  @param firstExternRow- pointer to extern table 
  @return 1 if errors exists in file, 0 otherwise
*/
int handleExternCommand(char *bufLine, char *labelPtr,  int *j, symbolRow *firstSymbolRow, externRow *firstExternRow) {
	char attribute[MAX_LABEL_SIZE]; /*stors extern command attribute*/

	if(!(strlen(labelPtr) == 0)) 
		fprintf(stdout, "Warning: line %d: label before extern sentence\n", lineNumber);
        
	skipSpaces(bufLine, j);
         
	if(!getLabel(bufLine,attribute,j))	
           return 0;
	
	if(!insertLabel(attribute, firstSymbolRow, "extern"))
		return 0;
	
 	if(!endOfInstruct(bufLine, j))
		return 0;
	
	return 1;
}

/*
  this function handels entry command in first pass, skips on it
  @param labelPtr - label of line
  @return 1 if errors exists in file, 0 otherwise
*/
int handleEntryCommand(char *labelPtr) {
		if(!(strlen(labelPtr) == 0)) 
			fprintf(stdout, "Warning: line %d: label before entry sentence\n", lineNumber);

	return 1;
}

/*
  this function handels any guidence command, gets command type, check it then handles as needed
  @param bufLine - text line
  @param labelPtr - label of line
  @param j - index through text line
  @param firstSymbolRow- pointer to symbol table
  @param firstDataRow- pointer to data table
  @param firstExternRow- pointer to extern table 
  @return 1 if errors exists in file, 0 otherwise
*/
int guidenceHandle(char *bufLine, char *labelPtr, int *j, symbolRow *firstSymbolRow, dataRow *firstDataRow, externRow* firstExternRow) { 
	/*Assume that we start at "."*/
	char command[7]; /*Space for guidence commad*/	
	int i = 0; /*length of command*/
	int typeOfCommand; /*type of command*/
	
	*j+=1;
        
	while(!isspace(bufLine[*j]) && i < 7 && bufLine[*j] != '\0') {
		if(i == 6) { /*Means command is too long*/
                        

			fprintf(stdout, "line %d: Unrecognized command in sentence\n", lineNumber);
				
		}
		command[i] = bufLine[*j];
		i++;
		*j+=1;
	}
	command[i] = '\0';

	typeOfCommand = commandType(command);
	if(typeOfCommand == ILLEGAL_COMMAND) {
                 
		fprintf(stdout, "line %d: Unrecognized command in sentence\n", lineNumber);
		return 0;
	}
	else if(typeOfCommand == DATA_COMMAND)
		return handleDataCommand(bufLine, labelPtr, command, j, firstSymbolRow, firstDataRow);  

	else if(typeOfCommand == ASCIZ_COMMAND)
		return handleAscizCommand(bufLine, labelPtr, j, firstSymbolRow, firstDataRow);

	else if(typeOfCommand == EXTERN_COMMAND)
		return handleExternCommand(bufLine, labelPtr, j, firstSymbolRow, firstExternRow);

	else /*typeOfCommand == ENTERY_COMMAND, will be handled in the next pass*/
		return handleEntryCommand(labelPtr);		
}

/*
  this function frees the data linked list
  @param row- pointer to data table
*/
void freeDataTable(dataRow *row) {

	dataRow *current = row; /*pointer to data table*/
	dataRow *next; /*pointer to data table*/
  
  while(current!=NULL)
  {
    next=current->next;
    free(current);
    current=next;
  }
  row=NULL;	
}

/*
  this function updates the data table after first pass
  @param row- pointer to data table
*/
void updateDataTable(dataRow *row) {
	
	if(row->next == NULL) {
		row->address+=ICF;
		return;
	}
	updateDataTable(row->next);
	row->address+=ICF;
}

/*
  this function handels entry command in second pass, gets the attribute, check it and then inserts to symbol table
  @param bufLine - text line
  @param labelPtr - label of line
  @param j - index through text line
  @param firstSymbolRow- pointer to symbol table
  @return 1 if errors exists in file, 0 otherwise
*/
int handleEntrySecPass(char *bufLine, int *j, symbolRow *firstSymbolRow) { /*WORKING*/
	char label[(MAX_LABEL_SIZE + 1)]; /*string to store the attribute*/
	int i = 0; /*checks attribute size*/
	symbolRow *labelPos; /*pointer to symbol table*/
	labelPos = NULL;

	skipSpaces(bufLine, j);
	/*extract the label*/
	while(!isspace(bufLine[*j]) && bufLine[*j] != '\0') {
		label[i] = bufLine[*j];
		i++;
		*j+=1;

		if(i == (MAX_LABEL_SIZE +1)) {
			fprintf(stdout, "line %d: Label attribute is too long !", lineNumber);
			return 0;
		}
	}
 
	label[i] = '\0';

	/*search the label*/
	labelPos = searchSymbol(label, firstSymbolRow);
	if(labelPos == NULL) {
		fprintf(stdout, "line %d: Entry label doesn't exist in file", lineNumber);
		return 0;
	}
	
	if(strcmp(labelPos->attribute, "extern") == 0) {
		fprintf(stdout, "line %d: Entry label depends on extern label in the same file", lineNumber);
		return 0;
	}
	/*add to struct*/
	if(strstr(labelPos->attribute,"entry")==NULL)
	{
		strcat(labelPos->attribute,", entry");
		labelPos->entryFlag = 1;	
	}
	if(!endOfInstruct(bufLine, j))
		return 0;

	return 1;
}

/*
  this function handels any guidence command in second pass, gets command type, check it then handles as needed
  @param bufLine - text line
  @param j - index through text line
  @param firstSymbolRow- pointer to symbol table
  @param firstDataRow- pointer to data table
  @return 1 if errors exists in file, 0 otherwise
*/
int handleGuidenceSecPass(char *bufLine, int *j, symbolRow *firstSymbolRow, dataRow *firstDataRow) {

	char command[7]; /*string for guidence commad*/	
	int i = 0; /*index thorugh the command*/
	int typeOfCommand; /*type of command*/
	
        *j+=1;
	while(!isspace(bufLine[*j]) && i < 7 && bufLine[*j] != '\0') {
		if(i == 6) { /*Means command is too long*/
			fprintf(stdout, "line %d: Unrecognized command in sentence", lineNumber);
				
		}
		command[i] = bufLine[*j];
		i++;
		*j+=1;
	}
	command[i] = '\0';

	typeOfCommand = commandType(command);
	
	if((typeOfCommand == ENTRY_COMMAND)) {
		return handleEntrySecPass(bufLine, j, firstSymbolRow);
	}

	if(typeOfCommand != ENTRY_COMMAND) {
		return 1;
	}
   return 1;
}


