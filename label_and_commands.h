#include "programvars.h"
#include "structs.h"

int getLabel(char *, char *, int *) ;
int handleLabel(char *, char *, int *);

int insertLabel(char *, symbolRow *, char *);

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

int guidenceHandle(char*, char*, int*, symbolRow*, dataRow*, externRow*);

int handleGuidenceSecPass(char*, int*, symbolRow*, dataRow*);

void freeDataTable(dataRow *);

void updateDataTable(dataRow *);

int instructionHandle(char *, char *, int*, symbolRow *, instructionRow *);

void freeInstTable(instructionRow *);

int instHandleSecPass(char*, char*, int*, symbolRow*, instructionRow*, externRow*);


