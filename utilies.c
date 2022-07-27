#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "structs.h"

/*
  this function gets line text and pointer to position in line, then advence the position to the first char which is not white space
  @param bufLine - pointer to line of text
  @param j - stores index inside the line
*/
void skipSpaces(char *bufLine, int *j) {

	if (!(isspace(bufLine[*j]) && bufLine[*j] != '\0'))
		return;
	
	while ((bufLine[*j]==' '||bufLine[*j]=='\t')  && bufLine[*j] != '\0') {	
	  	*j+=1;	
	}
		return;
}

/*
  this function gets value, number of bytes and node in data linked list, then calculate the binary of value in 2's complements and stores it inside the node. 
  @param value - value to insert in binary
  @param bytesnumber - number of bytes to insert into
  @param row- pointer to node in data table
*/
void DecimalToBinary(int value, int bytesNumber, dataRow *row) {
	unsigned int uint;
	int i;
	
	if(value<0) { /*convert to unsigned int multiple by -1 to get 2's complement*/
		uint = value*(-1);
		uint = ~uint;
		uint+= 0x1;
	}

	else /*convert to unsigned int*/ 
		uint = (unsigned int)value;

	if(bytesNumber == 1) {
		for(i=0; i<bytesNumber; i++) {
			(row->binary_db_asciz)[i] = uint & 0xFF;
			uint = uint >> 8;	
		}
	}

	if(bytesNumber == 2) {
		for(i=0; i<bytesNumber; i++) {
			(row->binary_dh)[i] = uint & 0xFF;
			uint = uint >> 8;
		}
	}
	
	if(bytesNumber == 4) {
		for(i=0; i<bytesNumber; i++) {
			(row->binary_dw)[i] = uint & 0xFF;
			uint = uint >> 8;	
		}
	}
}
