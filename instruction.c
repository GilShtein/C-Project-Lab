#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "programvars.h"
#include "utilies.h"

#define R0_INSTRUCT 1
#define R1_INSTRUCT 2
#define I1_INSTRUCT 3
#define I2_INSTRUCT 4
#define I3_INSTRUCT 5
#define J_INSTRUCT 6
#define STOP_INSTRUCT 7
#define ILLEGAL_INSTRUCT 8

/*
  this function return the value of a register
  @param bufLine line of text
  @param j index
  @return value of register
*/
int getRegister(char *bufLine, int *j) {
	int value = 0; /*index*/	

	skipSpaces(bufLine, j);
	if(bufLine[*j] == '\0') {
		fprintf(stdout, "line %d: missing argument\n", lineNumber);
		return -1;
	}

	if(bufLine[*j] != '$') {
		fprintf(stdout, "line %d: argument has to start with '$'\n", lineNumber);
		return -1;
	}

	*j+=1;
	if(bufLine[*j] < '0' || bufLine[*j] > '9') {
		fprintf(stdout, "line %d: attributes in this instruction gets only numbers\n", lineNumber);
		return -1;
	}	
 
	value = (int)(atoi(&bufLine[*j]));
	
	while(bufLine[*j] >= '0' && bufLine[*j] <= '9') {
		*j+=1;
	}
			
	if(!(value >= 0 && value <= 31)) {
		fprintf(stdout, "line %d: registers are 0-31 only\n", lineNumber);
		return -1;			
	}

	return value;
}


/*
  this function return 0 if in the end of the line has chars which should not be there or 1 otherwise
  @param bufLine line of text
  @param j index
  @return 0 if there some errors or 1 otherwise
*/
int endOfInstruct(char *bufLine, int *j) { 
	while(bufLine[*j] != '\0') {
		if(!isspace(bufLine[*j])) {
			fprintf(stdout, "line %d: unnecessary letters in end of line\n", lineNumber);
			return 0;
		}
		*j+=1;
	}
	return 1;
}

/*
  this function gets number argument from instruction senctence and check the syntax
  @param bufLine - line of text
  @param value - store the value of the  argument extracted
  @param minNumber - minimum valid value
  @param maxNumber - maximum valid value
  @param j - line index
  @return 0 if there some errors or 1 otherwise
*/
int getNumberArg(char *bufLine, int *value, int minNumber, int maxNumber, int *j) {

	int sign = 1; /*sign of value, 1 = positive, -1 = negative*/
	*value = 0;

	skipSpaces(bufLine, j);
	if(bufLine[*j] == '-') {
		sign = -1;
		*j+=1;
	}
	
	else if(bufLine[*j] == '+') {
		sign = 1;
		*j+=1;
	}

	else if(bufLine[*j] < '0' || bufLine[*j] > '9') {
		fprintf(stdout, "line %d: wrong attribue to instruct\n", lineNumber);
		return 0;
	}
	
	
	*value = (int)(atoi(&bufLine[*j]));
	*value*=sign;		
	while((bufLine[*j] >= '0') && (bufLine[*j] <= '9')) 
		*j+=1; 
	
	if(*value < minNumber || *value > maxNumber) {
		fprintf(stdout, "line %d: value doesn't fit to number of bytes allocated by instruction\n", lineNumber);
			return 0;
	}
	
	return 1;
}

/*
 this function return the type of instruct
  @param instruct pointer to the instruct text
  @return type of instruct
*/
int typeOfInst(char *instruct) {
	if((strcmp(instruct, "add") == 0) || (strcmp(instruct, "sub") == 0) || (strcmp(instruct, "and") == 0) || (strcmp(instruct, "or") == 0) || (strcmp(instruct, "nor") == 0))
 		return R0_INSTRUCT;

	else if((strcmp(instruct, "move") == 0) || (strcmp(instruct, "mvhi") == 0) || (strcmp(instruct, "mvlo") == 0))
		return R1_INSTRUCT;

	else if((strcmp(instruct, "addi") == 0) || (strcmp(instruct, "subi") == 0) || (strcmp(instruct, "andi") == 0) || (strcmp(instruct, "ori") == 0) || (strcmp(instruct, "nori") == 0))
		return I1_INSTRUCT;

	else if((strcmp(instruct, "beq") == 0) || (strcmp(instruct, "bne") == 0) || (strcmp(instruct, "blt") == 0) || (strcmp(instruct, "bgt") == 0))
		return I2_INSTRUCT;

	else if((strcmp(instruct, "lb") == 0) || (strcmp(instruct, "sb") == 0) || (strcmp(instruct, "lw") == 0) || (strcmp(instruct, "sw") == 0) || (strcmp(instruct, "lh") == 0) || (strcmp(instruct, "sh") == 0))
		return I3_INSTRUCT;

	else if((strcmp(instruct, "jmp") == 0) || (strcmp(instruct, "la") == 0) || (strcmp(instruct, "call") == 0))
		return J_INSTRUCT;

	else if (strcmp(instruct, "stop") == 0)
		return STOP_INSTRUCT;

	else
		return ILLEGAL_INSTRUCT;
}

/*
this function check the syntax of add,or... type instruct and extract arguments 
 @param bufLine - line of text
 @param labelPtr - pointer to label text
 @param instruct - pointer to instruct text
 @param j - index
 @param firstSymbolRow- pointer to symbol table
 @param firstInstRow- pointer to instruction table 
 @return 1 if errors exists in file, 0  otherwise
*/
int handleR0Instruct(char *bufLine, char *labelPtr, char *instruct, int *j, symbolRow *firstSymbolRow, instructionRow *firstInstRow) {
	int reg1 = -1; /*register 1*/
	int reg2 = -1; /*register 2*/
	int reg3 = -1; /*register 3*/
        
	if(!(strlen(labelPtr) == 0)) { /*Checks if there's label in the sentence*/
		if(!insertLabel(labelPtr, firstSymbolRow, "code"))
			return 0;
	}
			
	reg1 = getRegister(bufLine, j);
	if(reg1 == -1)
		return 0;
	skipSpaces(bufLine, j);
                
	if((bufLine[*j] < '0' || bufLine[*j] > '9') && bufLine[*j] != ',') {
		        
		fprintf(stdout, "line %d: wrong attribue to command\n", lineNumber);
		return 0;
	}
	if(bufLine[*j] != ',') {
		fprintf(stdout, "line %d: missing comma between arguments\n", lineNumber);
		return 0;
	}
	*j+=1;
			
	reg2 = getRegister(bufLine, j);
	if(reg2 == -1)
		return 0;
        skipSpaces(bufLine, j);
	if((bufLine[*j] < '0' || bufLine[*j] > '9') && bufLine[*j] != ',') {
			
		fprintf(stdout, "line %d: wrong attribue to command\n", lineNumber);
		return 0;
	}

	if(bufLine[*j] != ',') {
		fprintf(stdout, "line %d: missing comma between arguments\n", lineNumber);
		return 0;
	}
	*j+=1;
	
	reg3 = getRegister(bufLine, j);
	if(reg3 == -1)
		return 0;			

	/*now need to check end of the line*/
	if(!endOfInstruct(bufLine, j))
		return 0;
	/* give explicit add, sub. and or nor */
	insertR0Instruction(firstInstRow, instruct, reg1, reg2, reg3);
	return 1;
}

/*
this function check the syntax of move, mvhi... type instruct and extract arguments 
 @param bufLine - line of text
 @param labelPtr - pointer to label text
 @param instruct - pointer to instruct text
 @param j - index
 @param firstSymbolRow- pointer to symbol table
 @param firstInstRow- pointer to instruction table 
 @return 1 if errors exists in file, 0  otherwise
*/
int handleR1Instruct(char *bufLine, char *labelPtr, char *instruct, int *j, symbolRow *firstSymbolRow, instructionRow *firstInstRow) { 
	int reg1 = -1; /*register 1*/
	int reg2 = -1; /*register 2*/
	
	if(!(strlen(labelPtr) == 0)) { /*Checks if there's label in the sentence*/
		if(!insertLabel(labelPtr, firstSymbolRow, "code"))
			return 0;
	}	
	
	reg1 = getRegister(bufLine, j);
	if(reg1 == -1)
		return 0;
	skipSpaces(bufLine, j);

	if((bufLine[*j] < '0' || bufLine[*j] > '9') && bufLine[*j] != ',') {
		fprintf(stdout, "line %d: wrong attribue to command\n", lineNumber);
		return 0;
	}
	
	if(bufLine[*j] != ',') {
		fprintf(stdout, "line %d: missing comma between arguments\n", lineNumber);
		return 0;
	}
	*j+=1;
			
	reg2 = getRegister(bufLine, j);
	if(reg2 == -1)
		return 0;

	/*now need to check end of the line*/
	if(!endOfInstruct(bufLine, j))
		return 0;

	insertR1Instruction(firstInstRow, instruct, reg1, 0, reg2);
	return 1;
}

/*
this function check the syntax of addi, subi... type instruct and lb sb... type instruct and extract arguments 
 @param bufLine - line of text
 @param labelPtr - pointer to label text
 @param instruct - pointer to instruct text
 @param j - index
 @param firstSymbolRow- pointer to symbol table
 @param firstInstRow- pointer to instruction table 
 @return 1 if errors exists in file, 0  otherwise
*/
int handleI1I3Instruct(char *bufLine, char *labelPtr, char *instruct, int *j, symbolRow *firstSymbolRow, instructionRow *firstInstRow) {
	int reg1 = -1; /*register 1*/
	int reg2 = -1; /*register 2*/
	int maxNumber = pow(2,(2*8) -1) -1; /*maxNumber of valid value*/
	int minNumber = (-1) * pow(2,(2*8) -1); /*minNumber of valid value*/
	int immed; /*immed value*/
	
	if(!(strlen(labelPtr) == 0)) { /*Checks if there's label in the sentence*/
		if(!insertLabel(labelPtr, firstSymbolRow, "code")) {			
			return 0;
		}
	}
	
	reg1 = getRegister(bufLine, j);
	if(reg1 == -1) {
		return 0;
	}

	skipSpaces(bufLine, j);
	
	if((bufLine[*j] < '0' || bufLine[*j] > '9') && bufLine[*j] != ',') {
		fprintf(stdout, "line %d: wrong attribue to command\n", lineNumber);
		return 0;
	}

	if(bufLine[*j] != ',') {
		fprintf(stdout, "line %d: missing comma between arguments\n", lineNumber);
		return 0;
	}

	*j+=1;
	
	if(!getNumberArg(bufLine, &immed, minNumber, maxNumber, j)) {
		return 0;
	}

	skipSpaces(bufLine, j);

	if((bufLine[*j] < '0' || bufLine[*j] > '9') && bufLine[*j] != ',') {
		fprintf(stdout, "line %d: wrong attribue to command\n", lineNumber);
		return 0;
	}

	if(bufLine[*j] != ',') {
		fprintf(stdout, "line %d: missing comma between arguments\n", lineNumber);
		return 0;	
	}

	*j+=1;
	reg2 = getRegister(bufLine, j);

	if(reg2 == -1) {
		return 0;
	}

	if(!endOfInstruct(bufLine, j)) {		
		return 0;
	}

	insertI1I3Instruction(firstInstRow, instruct, reg1, reg2, immed);
	return 1;
}

/*
this function check the syntax of bne, bgt... type instruct and extract arguments 
 @param bufLine - line of text
 @param labelPtr - pointer to label text
 @param instruct - pointer to instruct text
 @param j - index
 @param firstSymbolRow- pointer to symbol table
 @param firstInstRow- pointer to instruction table 
 @return 1 if errors exists in file, 0  otherwise
*/
int handleI2Instruct(char *bufLine, char *labelPtr, char *instruct, int *j, symbolRow *firstSymbolRow, instructionRow *instructionPtr) {

	int reg1 = -1; /*register 1*/
	int reg2 = -1; /*register 2*/
	char labelArg[MAX_LABEL_SIZE]; /*label*/
	int pos; /*label pos in sentence*/

	if(!(strlen(labelPtr) == 0)) { /*Checks if there's label in the sentence*/
		if(!insertLabel(labelPtr, firstSymbolRow, "code"))
			return 0;
	}
	
	reg1 = getRegister(bufLine, j);
	if(reg1 == -1) {
		return 0;
	}
	skipSpaces(bufLine, j);

	if((bufLine[*j] < '0' || bufLine[*j] > '9') && bufLine[*j] != ',') {
		fprintf(stdout, "line %d: wrong attribue to command\n", lineNumber);
		return 0;
	}

	if(bufLine[*j] != ',') {		
		fprintf(stdout, "line %d: missing comma between arguments\n", lineNumber);
		return 0;
	}
	*j+=1;
				
	reg2 = getRegister(bufLine, j);
	if(reg2 == -1) {
		return 0;
	}
	skipSpaces(bufLine, j);

	if(bufLine[*j] != ',') {
		fprintf(stdout, "line %d: missing comma between arguments\n", lineNumber);
		return 0;
	}			
	*j+=1;
        skipSpaces(bufLine,j);
	pos = *j;
	if(!getLabel(bufLine, labelArg, j)) {
		return 0;
	}
	

	if(!endOfInstruct(bufLine, j)) {
		return 0;
	}

	/*get to the last row */

	while(instructionPtr->next != NULL)
    		instructionPtr = instructionPtr->next;

	  if(instructionPtr->address != IC) { /* not first row*/
    		instructionPtr->next = (instructionRow *) malloc(sizeof(instructionRow));
		if(instructionPtr->next == NULL) {
			printf("Cannot allocate memory for program");
			exit(0);
		}
	  
		/*Makes new row*/
		instructionPtr = instructionPtr->next;
	    	instructionPtr->next = NULL;
		instructionPtr->address = IC;		
  	}

	strcpy(instructionPtr->instruct, instruct);
	instructionPtr->rs = reg1;
	instructionPtr->rt = reg2;
  	instructionPtr->labelArgPos = pos;

	return 1;
 
}

/*
this function check the syntax of jmp, la, call... type instruct and extract arguments 
 @param bufLine - line of text
 @param labelPtr - pointer to label text
 @param instruct - pointer to instruct text
 @param j - index
 @param firstSymbolRow- pointer to symbol table
 @param firstInstRow- pointer to instruction table 
 @return 1 if errors exists in file, 0  otherwise
*/
int handleJInstruct(char *bufLine, char *labelPtr, char *instruct, int *j, symbolRow *firstSymbolRow, instructionRow *instructionPtr) {
	int reg1 = -1; /*register 1*/
	int flag =  0; /*flag for type of jmp instruct*/
	char labelArg[MAX_LABEL_SIZE]; /*label*/
	int pos; /*position of label*/

	if(!(strlen(labelPtr) == 0)) { /*Checks if there's label in the sentence*/
		if(!insertLabel(labelPtr, firstSymbolRow, "code")) {
			return 0;
		}
	}

	skipSpaces(bufLine, j);
	if(strcmp(instruct, "jmp") == 0 && bufLine[*j] == '$') {
			flag = 1;			
			reg1 = getRegister(bufLine, j);
			if(reg1 == -1)	
				return 0;
		skipSpaces(bufLine,j);	
	}						
		
        else {
		pos = *j;
		if(!getLabel(bufLine, labelArg, j)) {
			return 0;
		}
	}

	if(!endOfInstruct(bufLine, j)) {
		return 0;
	}

/*get to the last row */

	while(instructionPtr->next != NULL)
    		instructionPtr = instructionPtr->next;

	  if(instructionPtr->address != IC) { /* not first row*/
    		instructionPtr->next = (instructionRow *) malloc(sizeof(instructionRow));
		if(instructionPtr->next == NULL) {
			printf("Cannot allocate memory for program");
			exit(0);
		}
	  
		/*Makes new row*/ 
		instructionPtr = instructionPtr->next;
	    	instructionPtr->next = NULL;
		instructionPtr->address = IC;		
  	}
	
	strcpy(instructionPtr->instruct, instruct);
	instructionPtr->rs = reg1;
	instructionPtr->flag = flag;
	instructionPtr->labelArgPos = pos;
	return 1;
}

/*
this function check the syntax of stop instruct
 @param bufLine - line of text
 @param labelPtr - pointer to label text
 @param instruct - pointer to instruct text
 @param j - index
 @param firstSymbolRow- pointer to symbol table
 @param firstInstRow- pointer to instruction table 
 @return 1 if errors exists in file, 0  otherwise
*/
int handleStopInstruct(char *bufLine, char *labelPtr, char *instruct, int *j, symbolRow *firstSymbolRow, instructionRow *firstInstRow) { 
	
	if(!(strlen(labelPtr) == 0)) { /*Checks if there's label in the sentence*/
		if(!insertLabel(labelPtr, firstSymbolRow, "code")) {
			return 0;
		}
	}

	if(!endOfInstruct(bufLine, j)) {		
		return 0;
	}

	stop(firstInstRow);
	return 1;
}

/*
 this function handels any instruction sentence, gets sentence type, check it then handles as needed
 @param bufLine - line of text
 @param labelPtr - pointer to label text
 @param j - index
 @param firstSymbolRow- pointer to symbol table
 @param firstInstRow- pointer to instruction table 
 @return 1 if errors exists in file, 0  otherwise
*/
int instructionHandle(char *bufLine, char *labelPtr, int *j, symbolRow *firstSymbolRow, instructionRow *firstInstRow) {
	char instruct[5]; /*The longest instruction is 4 chars*/
	int i = 0; /*instruct index*/
	int type; /*type of instruct*/
        
	while(!isspace(bufLine[*j]) && i < 5 && bufLine[*j] != '\0') {
		if(i == 4) { /*Means instruct is too long*/
			fprintf(stdout, "line %d: Unrecognized instruction in sentence\n", lineNumber);
			return 0;		
		}		
		instruct[i] = bufLine[*j];
		i++;
		*j+=1;
	}
	
	instruct[i] = '\0';
       
	type = typeOfInst(instruct);
	if(type == ILLEGAL_INSTRUCT) {
		fprintf(stdout, "line %d: Unrecognized instruction in sentence\n", lineNumber);
		return 0;
	}
 
	if(type == R0_INSTRUCT)
		return handleR0Instruct(bufLine, labelPtr, instruct, j, firstSymbolRow, firstInstRow);

	else if(type == R1_INSTRUCT)
		return handleR1Instruct(bufLine, labelPtr, instruct, j, firstSymbolRow, firstInstRow);

	else if(type == I1_INSTRUCT)
		return handleI1I3Instruct(bufLine, labelPtr, instruct, j, firstSymbolRow, firstInstRow);

	else if(type == I2_INSTRUCT)
		return handleI2Instruct(bufLine, labelPtr, instruct, j, firstSymbolRow, firstInstRow);

	else if(type == I3_INSTRUCT)
		return handleI1I3Instruct(bufLine, labelPtr, instruct, j, firstSymbolRow, firstInstRow);

	else if(type == J_INSTRUCT)
		return handleJInstruct(bufLine, labelPtr, instruct, j, firstSymbolRow, firstInstRow);

	else if(type == STOP_INSTRUCT)
		return handleStopInstruct(bufLine, labelPtr, instruct, j, firstSymbolRow, firstInstRow);
return 1;
}

/*
  this function frees the instruction linked list
  @param row- pointer to instruction table
*/
void freeInstTable(instructionRow *instRow) {

instructionRow *current = instRow;
instructionRow *next;
  
  while(current!=NULL)
  {
    next=current->next;
    free(current);
    current=next;
  }
  instRow=NULL;
        	
}

/*
  this function handels any instruct sentence in second pass, gets sentence type, check it then handles as needed
  @param bufLine - text line
  @param labelPtr - pointer to label text
  @param j - index through text line
  @param firstSymbolRow- pointer to symbol table
  @param firstInstRow- pointer to instruction table
  @param firstExternRow- pointer to extern table
  @return 1 if errors exists in file, 0 otherwise
*/
int instHandleSecPass(char *bufLine, char *labelPtr, int *j, symbolRow *firstSymbolRow, instructionRow *firstInstRow, externRow *firstExternRow) { 

	/*check type of instruction*/
	char instruct[5]; /*The longest instruction is 4 chars*/
	int i = 0; /*index*/
	unsigned int memDistance; /*memory distance or memory value*/
	int type; /*type of instruct*/
	char labelArg[MAX_LABEL_SIZE]; /*label*/
	symbolRow *symbol; /*symbol pointer*/

	while(firstInstRow->next!=NULL && firstInstRow->address!=IC)
		firstInstRow = firstInstRow->next;
        
	
	skipSpaces(bufLine, j);
	while(!isspace(bufLine[*j]) && i < 5 && bufLine[*j] != '\0') {
	
		if(i == 4) { /*Means instruct is too long*/
			fprintf(stdout, "line %d: Unrecognized instruction in sentence\n", lineNumber);
			return 0;		
		}		
		instruct[i] = bufLine[*j];
	
		i++;
		*j+=1;
	}
	
	instruct[i] = '\0';
	type = typeOfInst(instruct);
	

	if (type == I2_INSTRUCT) {
		getRegister(bufLine, j);
		skipSpaces(bufLine, j);
		*j+=1;			
		getRegister(bufLine, j);
		skipSpaces(bufLine, j);			
		*j+=1;
        	skipSpaces(bufLine,j);
				
		getLabel(bufLine, labelArg, j);
		
		symbol = searchSymbol(labelArg, firstSymbolRow);
		if(symbol == NULL) {
			fprintf(stdout, "line %d: No such symbol exist\n", lineNumber);
			return 0;		
		}

		else if(strcmp(symbol->attribute, "extern") == 0) {
			fprintf(stdout, "line %d: Extern symbol cannot be used in the same file it declared\n", lineNumber);
			return 0;		
		}

		else {
			memDistance = symbol->value - IC;
			insertI2Instruction(firstInstRow, instruct, firstInstRow->rs, firstInstRow->rt, memDistance);		
		}
	}

	else if(type == J_INSTRUCT) {
                skipSpaces(bufLine,j);
		
		if(firstInstRow->flag == 0) {			
		        getLabel(bufLine, labelArg,j);
			symbol = searchSymbol(labelArg, firstSymbolRow);
			if(symbol == NULL) {
				fprintf(stdout, "line %d: No such symbol exist\n", lineNumber);
				return 0;		
			}

			else if(strcmp(symbol->attribute, "extern") == 0) {
				memDistance = 0;
				i = 0;

				if(strlen(firstExternRow->labelName) == 0) {
					strcpy(firstExternRow->labelName, labelArg);
					firstExternRow->address = IC;
				}
				
				else {
					while(firstExternRow->next != NULL) {
						firstExternRow = firstExternRow->next;
					}
					
					firstExternRow->next = (externRow *) malloc(sizeof(externRow));
					if (firstExternRow->next == NULL) {
						printf("Cannot allocate memory for program");
						exit(0);
					}
		
					firstExternRow = firstExternRow->next;
					firstExternRow->next = NULL;
					
					strcpy(firstExternRow->labelName, labelArg);
					firstExternRow->address = IC;
				}
			}

			else
				memDistance = symbol->value;
		}

	insertJInstruction(firstInstRow, instruct, firstInstRow->rs, memDistance, firstInstRow->flag);		
	}
       return 1;
}

