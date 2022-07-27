#include "programvars.h"

void write(instructionRow* ,dataRow*, symbolRow*, externRow* ,char[], char[], char[]);

void writeInstruction(instructionRow*,  FILE*);  

void writeData(dataRow*,FILE*);

void writeEnt(symbolRow*, char[]);

void writeExt(externRow*, char[]);
