#include "programvars.h"
#include "structs.h"

int handleLabel(char *, char *, int*);

void freeSymbolTable(symbolRow *);

void updateSymbolTable(symbolRow *);

symbolRow *searchSymbol(char*, symbolRow *);

void skipLabel(char *, int*);

void insertR0Instruction(instructionRow*,char[],int,int,int);

void insertR1Instruction(instructionRow* ,char[],int ,int, int);

void insertI1I3Instruction(instructionRow* ,char[],int ,int ,int);

void insertI2Instruction(instructionRow* ,char[],unsigned int ,unsigned int ,unsigned int);

void insertJInstruction(instructionRow* , char[],int ,unsigned int ,int); 

void stop(instructionRow *);
