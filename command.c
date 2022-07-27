#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "programvars.h"
#include "structs.h"


/*
  this function coding R(add,nor,or...) type command into machine code
  @param instructionPtr- pointer to instruction table
  @param commandName[]- the name of the command
  @param rs- left register
  @param rt middle register
  @param rd right register
*/
void insertR0Instruction(instructionRow* instructionPtr,char commandName[],int rs,int rt,int rd) 
{ 
  int funct;
  int opcode;
  
  while(instructionPtr->next != NULL) /*iterate to last node*/
    instructionPtr = instructionPtr->next;

  if(instructionPtr->address != IC) { /*if not first row, allocate new row*/
    	instructionPtr->next = (instructionRow *) malloc(sizeof(instructionRow));
		if(instructionPtr->next == NULL) {
			printf("Cannot allocate memory for program");
			exit(0);
		}
	  
		/*Makes new row*/
		instructionPtr = instructionPtr->next;
	    	instructionPtr->next = NULL;
		instructionPtr->address = IC;
		strcpy(instructionPtr->instruct,commandName);		
  }

  if(strcmp(commandName,"add") == 0)
  {
     funct=1;                         
     opcode=0;
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=((rd&0x1f)<<3)+((funct>>2)&(0x07));
    instructionPtr->binary[0]=(funct&0x03)<<6;
  }
  if(strcmp(commandName,"sub") == 0)
  {
     funct=2;
     opcode=0;
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=((rd&0x1f)<<3)+((funct>>2)&(0x07));
    instructionPtr->binary[0]=(funct&0x03)<<6;
  }
  if(strcmp(commandName,"and") == 0)
  {
     funct=3;
     opcode=0;
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=((rd&0x1f)<<3)+((funct>>2)&(0x07));
    instructionPtr->binary[0]=(funct&0x03)<<6;
  }
  if(strcmp(commandName,"or") == 0)
  {
     funct=4;
     opcode=0;
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=((rd&0x1f)<<3)+((funct>>2)&(0x07));
    instructionPtr->binary[0]=(funct&0x03)<<6;
  }
  if(strcmp(commandName,"nor") == 0)
  {
     funct=5;
     opcode=0;
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=((rd&0x1f)<<3)+((funct>>2)&(0x07));
    instructionPtr->binary[0]=(funct&0x03)<<6;
  }
 }/*end of R instruction*/


/*
  this function coding R (move type) type command into machine code
  @param instructionPtr- pointer to instruction table
  @param commandName[]- the name of the command
  @param rs- left register
  @param rt middle register
  @param rd right register
*/
void insertR1Instruction(instructionRow* instructionPtr,char commandName[],int rs,int rt, int rd)
{
  int funct;
  int opcode;
  
  while(instructionPtr->next != NULL)
    instructionPtr = instructionPtr->next;

  if(instructionPtr->address != IC) { /*if not first row, allocate new row*/
    	instructionPtr->next = (instructionRow *) malloc(sizeof(instructionRow));
		if(instructionPtr->next == NULL) {
			printf("Cannot allocate memory for program");
			exit(0);
		}
	  
		/*Makes new row*/
		instructionPtr = instructionPtr->next;
	    	instructionPtr->next = NULL;
		instructionPtr->address = IC;
		strcpy(instructionPtr->instruct,commandName);				
  }


if(strcmp(commandName,"move") == 0)
  {
     funct=1;
     opcode=1;
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=((rd&0x1f)<<3)+((funct>>2)&(0x07));
    instructionPtr->binary[0]=(funct&0x03)<<6;
  }
  if(strcmp(commandName,"mvhi") == 0)
  {
     funct=2;
     opcode=1;
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=((rd&0x1f)<<3)+((funct>>2)&(0x07));
    instructionPtr->binary[0]=(funct&0x03)<<6;
  }
  if(strcmp(commandName,"mvlo") == 0)
  {
     funct=3;
     opcode=1;
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=((rd&0x1f)<<3)+((funct>>2)&(0x07));
    instructionPtr->binary[0]=(funct&0x03)<<6;
  }
 }/* end of R1 instruction*/

/*
  this function coding I(arithmetic and loading and saving in memory type) command into machine code
  @param instructionPtr- pointer to instruction table
  @param commandName[]- the name of the command
  @param rs- left register
  @param rt middle register
  @param immed the value of right 16 bits of 32 bits data structure
*/
void insertI1I3Instruction(instructionRow* instructionPtr,char commandName[],int rs,int rt,int immed)
{ 
  int opcode;
   
while(instructionPtr->next != NULL) /*iterate to last node*/
    instructionPtr = instructionPtr->next;

  if(instructionPtr->address != IC) { /*if not first row, allocate new row*/
    	instructionPtr->next = (instructionRow *) malloc(sizeof(instructionRow));
		if(instructionPtr->next == NULL) {
			printf("Cannot allocate memory for program");
			exit(0);
		}
	  
		/*Makes new row*/
		instructionPtr = instructionPtr->next;
	    	instructionPtr->next = NULL;
		instructionPtr->address = IC;
		strcpy(instructionPtr->instruct,commandName);				
  }

if(strcmp(commandName,"addi") == 0)
  {
     
     opcode=10; 
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }
  if(strcmp(commandName,"subi") == 0)
  {
     
     opcode=11; 
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }
  if(strcmp(commandName,"andi") == 0)
  {
     
     opcode=12; 
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }
  if(strcmp(commandName,"ori") == 0) 
  {
     
     opcode=13; 
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }
  if(strcmp(commandName,"nori") == 0)
  {
     
     opcode=14; 
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }

  if(strcmp(commandName,"lb") == 0)
  {
     
     opcode=19; 
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }
  if(strcmp(commandName,"sb") == 0)
  {
     
     opcode=20; 
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }
  if(strcmp(commandName,"lw") == 0)
  {
     
     opcode=21; 
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }
  if(strcmp(commandName,"sw") == 0)
  {
     
     opcode=22; 
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }
  if(strcmp(commandName,"lh") == 0)
  {
     
     opcode=23; 
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }
  if(strcmp(commandName,"sh") == 0)
  {
     
     opcode=24; 
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }
 }/* end I1I3Instruction */

/*
  this function coding I(bne, bgt, beq, blt) command into machine code
  @param instructionPtr- pointer to instruction table
  @param commandName[]- the name of the command
  @param rs- left register
  @param rt middle register
  @param immed the value of right 16 bits of 32 bits data structure
*/
void insertI2Instruction(instructionRow* instructionPtr,char commandName[], unsigned int rs, unsigned int rt,unsigned int immed)
 { 
  unsigned int opcode;
	
  if(strcmp(commandName,"bne") == 0)
  {
     
     opcode=15; 	
    instructionPtr->binary[3]=((opcode<<2)+((rs>>3)&0x03));
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }
  if(strcmp(commandName,"beq") == 0)
  {
     
     opcode=16; 
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }
  if(strcmp(commandName,"blt") == 0) 
  {
     
     opcode=17; 
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }
  if(strcmp(commandName,"bgt") == 0)
  {
     
     opcode=18; 
    instructionPtr->binary[3]=(opcode<<2)+((rs>>3)&0x03);
    instructionPtr->binary[2]= ((rs&0x07)<<5)+(rt&0x1f);
    instructionPtr->binary[1]=(immed>>8)&0xff;
    instructionPtr->binary[0]=(immed&0xff);   
  }
 }/* end of I2 */

/*
  this function coding J(jmp, la, call) command into machine code
  @param instructionPtr- pointer to instruction table
  @param commandName[]- the name of the command
  @param reg - register
  @param address - the value of 25 least significant bits  
  @param flag - 1 if we have register as argument, 0 if we have label as argument
*/
void insertJInstruction(instructionRow * instructionPtr, char commandName[],int reg,unsigned int adrress,int flag)    
  { 
    int opcode;
    
  if(strcmp(commandName,"jmp") == 0) 
  {
     opcode=30;      
     if(flag==1) /*if we have register*/
     {
       adrress=reg;
       reg=1;
       instructionPtr->binary[3]=(opcode<<2)+(reg<<1)+((adrress>>16)&0x01); 
       instructionPtr->binary[2]= adrress>>16;
       instructionPtr->binary[1]=adrress>>8;
       instructionPtr->binary[0]=adrress;

     } 
     else /*if we have label*/
     {
       reg=0;
       instructionPtr->binary[3]=(opcode<<2)+((adrress>>16)&0x80);
       instructionPtr->binary[2]= adrress>>16;
       instructionPtr->binary[1]=adrress>>8;
       instructionPtr->binary[0]=adrress;
     }
   }
   
  if(strcmp(commandName,"la") == 0)
  {
       opcode=31; 
       reg=0;
       instructionPtr->binary[3]=(opcode<<2)+((adrress>>16)&0x80);
       instructionPtr->binary[2]= adrress>>16;
       instructionPtr->binary[1]=adrress>>8;
       instructionPtr->binary[0]=adrress; 
  }

  if(strcmp(commandName,"call") == 0)
  {
       opcode=32;
       reg=0;
       instructionPtr->binary[3]=(opcode<<2)+((adrress>>16)&0x80);
       instructionPtr->binary[2]= adrress>>16;
       instructionPtr->binary[1]=adrress>>8;
       instructionPtr->binary[0]=adrress;  
  }
}

/*
  this function coding stop command into machine code
  @param instructionPtr- pointer to instruction table
*/
void stop(instructionRow *instructionPtr) 
  {    
      int opcode=63;
        
	while(instructionPtr->next != NULL) /*iterate to last node*/
   	 instructionPtr = instructionPtr->next;

  if(instructionPtr->address != IC) { /*if not first row, allocate new row*/
    	instructionPtr->next = (instructionRow *) malloc(sizeof(instructionRow));
		if(instructionPtr->next == NULL) {
			printf("Cannot allocate memory for program");
			exit(0);
		}
	  
		/*Makes new row*/
		instructionPtr = instructionPtr->next;
	    	instructionPtr->next = NULL;
		instructionPtr->address = IC;
		strcpy(instructionPtr->instruct,"stop");			

  }

      instructionPtr->binary[3]=(opcode<<2);
      instructionPtr->binary[2]=0;
      instructionPtr->binary[1]=0;
      instructionPtr->binary[0]=0;    
}

